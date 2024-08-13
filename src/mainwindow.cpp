#include "mainwindow.h"
#include <QtUiTools>
#include <QMessageBox>


MainWindow::MainWindow()
{
    QFile file("assets/gui_main.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    m_mainwindow = loader.load(&file, this);
    file.close();

    m_mainwindow->setWindowIcon(QIcon("assets/icons/app.png"));
    m_mainwindow->installEventFilter(this);

    m_menu_close = m_mainwindow->findChild<QAction*>("actionClose");
    m_menu_openProj = m_mainwindow->findChild<QAction*>("actionOpen_Project");
    m_menu_saveAsProj = m_mainwindow->findChild<QAction*>("actionSaveAs_Project");
    m_menu_clearResults = m_mainwindow->findChild<QAction*>("actionClear_Results");
    m_menu_about = m_mainwindow->findChild<QAction*>("actionAbout");

    m_tv_testcase = m_mainwindow->findChild<QTreeView*>("tv_testcase");
    m_le_tree_uid = m_mainwindow->findChild<QLineEdit*>("le_tree_uid");
    m_cb_result = m_mainwindow->findChild<QComboBox*>("cb_result");
    m_ckb_tree_as_child = m_mainwindow->findChild<QCheckBox*>("ckb_tree_as_child");
    m_btn_tree_confirm = m_mainwindow->findChild<QPushButton*>("btn_tree_confirm");
    m_cb_tree_method = m_mainwindow->findChild<QComboBox*>("cb_tree_method");
    m_cb_tree_method->addItems({"Follow","Add Subfolder","Add Testcase","Delete"});

    m_lb_uid = m_mainwindow->findChild<QLabel*>("lb_uid");
    m_le_name = m_mainwindow->findChild<QLineEdit*>("le_name");
    m_te_comment = m_mainwindow->findChild<QTextEdit*>("te_comment");
    m_cb_testtype = m_mainwindow->findChild<QComboBox*>("cb_testtype");
    m_tw_testarea = m_mainwindow->findChild<QTableWidget*>("tw_testarea");
    m_btn_up = m_mainwindow->findChild<QPushButton*>("btn_up");
    m_btn_down = m_mainwindow->findChild<QPushButton*>("btn_down");
    m_btn_plus = m_mainwindow->findChild<QPushButton*>("btn_plus");
    m_btn_minus = m_mainwindow->findChild<QPushButton*>("btn_minus");
    m_btn_up = m_mainwindow->findChild<QPushButton*>("btn_up");
    m_btn_down = m_mainwindow->findChild<QPushButton*>("btn_down");
    m_btn_savecase = m_mainwindow->findChild<QPushButton*>("btn_savecase");

    connect(m_tv_testcase, SIGNAL(clicked(QModelIndex)), this, SLOT(displayTestCase()));
    connect(m_menu_close, SIGNAL(triggered()), this, SLOT(closeApp()));
    connect(m_menu_openProj, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(m_menu_saveAsProj, SIGNAL(triggered()), this, SLOT(saveAsProject()));
    connect(m_menu_clearResults, SIGNAL(triggered()), this, SLOT(clearResults()));
    connect(m_menu_about, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(m_btn_tree_confirm, SIGNAL(clicked()), this, SLOT(execTreeConfirm()));

    connect(m_cb_tree_method, SIGNAL(currentIndexChanged(int)), this, SLOT(updateConfirmGUI()));

    connect(m_btn_plus, SIGNAL(clicked()), this, SLOT(addTestCaseStep()));
    connect(m_btn_minus, SIGNAL(clicked()), this, SLOT(deleteTestCaseStep()));
    connect(m_btn_up, SIGNAL(clicked()), this, SLOT(moveTestStepUp()));
    connect(m_btn_down, SIGNAL(clicked()), this, SLOT(moveTestStepDown()));

    connect(m_btn_savecase, SIGNAL(clicked()), this, SLOT(updateTestCase()));
    connect(m_cb_result, SIGNAL(currentIndexChanged(int)), this, SLOT(updateResultGUI()));

    clearTestarea();
    initProject();

    m_mainwindow->show();
    showTestCaseTree(m_proj->data());
}

MainWindow::~MainWindow()
{

}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == this->m_mainwindow && event->type() == QEvent::Close)
    {
        qApp->quit();
    }
    return false;
}


void MainWindow::execTreeConfirm()
{
    std::string cf_option = m_cb_tree_method->currentText().toStdString();
    bool b_asChild = m_ckb_tree_as_child->isChecked();
    if (cf_option == "Follow" && m_tv_testcase->model()->rowCount())
    {
        if (!m_tv_testcase->selectionModel()->selectedIndexes().count()){
            return;
        }
        int uid_selected = m_tv_testcase->currentIndex().data().toString().split(":").at(0).split("-").at(1).toInt();
        int uid_target = m_le_tree_uid->text().toInt();
        if(!m_proj->followNode(uid_selected, uid_target, b_asChild)){
            QMessageBox::information(m_mainwindow, tr("Info"), tr("Follow operation failed: Nodes shall not follow each other "
                                                                  "within the same branch."));
            return;
        }
    }
    if (cf_option == "Add Subfolder" || cf_option == "Add Testcase")
    {
        NodeType nodetype = (cf_option == "Add Testcase") ? NodeType::eNodeTestCase : NodeType::eNodeSubFolder;
        int uid_target = m_le_tree_uid->text().toInt();
        if(!m_proj->addNode(uid_target, nodetype, b_asChild)){
            QMessageBox::information(m_mainwindow, tr("Info"), tr("Failed to add a new Node."));
            return;
        }
    }
    if (cf_option == "Delete")
    {
        if (!m_tv_testcase->selectionModel()->selectedIndexes().count()){
            return;
        }
        int uid_selected = m_tv_testcase->currentIndex().data().toString().split(":").at(0).split("-").at(1).toInt();
        if (!isNodeDeletable(uid_selected)){
            QMessageBox::information(m_mainwindow, tr("Info"), tr("Deletion failed: there must be at least one node."));
            return;
        }
        QString msg = "Do you really want to delete [>> UID-" + QString::number(uid_selected) + " <<] and its subcases?";
        QMessageBox::StandardButton qReplay = QMessageBox::question(m_mainwindow, "Warning", msg);
        if (qReplay != QMessageBox::Yes){
            return;
        }
        if(!m_proj->deleteNode(uid_selected)){
            QMessageBox::information(m_mainwindow, tr("Info"), tr("Deletion failed!"));
            return;
        }
    }
    showTestCaseTree(m_proj->data());
}

void MainWindow::openProject()
{
    QMessageBox::StandardButton qReply = QMessageBox::question(m_mainwindow, "Warning",
                                                                "WARNING: opening a new project will close your current project without saving, are you sure?");
    if (qReply != QMessageBox::Yes){
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Project"), ".", tr("Project File (*.qlpj)"));
    if (fileName.isEmpty() || fileName.isNull()){
        return;
    }
    delete m_proj;
    m_proj = new Project;
    if (!m_proj->openProject(fileName.toStdString().c_str())){
        return;
    }
    clearTestarea();
    showTestCaseTree(m_proj->data());
}

void MainWindow::saveAsProject()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Project as"), ".", tr("Project File (*.qlpj)"));
    if(m_proj){
        m_proj->saveProject(fileName.toStdString().c_str());
    }
}

void MainWindow::clearResults()
{
    QMessageBox::StandardButton qReply = QMessageBox::question(m_mainwindow, "Warning",
                                                                "WARNING: All test results of the current project will be cleaned up, are you sure?");
    if (qReply != QMessageBox::Yes){
        return;
    }

    for (auto & data : m_proj->data()->node_data)
    {
        data.testResult = "Neutral";
    }
    clearTestarea();
    showTestCaseTree(m_proj->data());
}

void MainWindow::closeApp()
{
    m_mainwindow->close();
    qApp->quit();
}

void MainWindow::showAbout()
{
    QMessageBox::aboutQt(m_mainwindow, "About");
}

void MainWindow::displayTestCase()
{
    QString uid = m_tv_testcase->currentIndex().data().toString().split(':').at(0).split("-").at(1);
    m_lb_uid->setText(uid);
    QString name = m_tv_testcase->currentIndex().data().toString().section(':',1).trimmed();
    m_le_name->setText(name);

    Node_data_t data_selected;
    for (auto & data : m_proj->data()->node_data)
    {
        if (data.uid == std::stoi(uid.toStdString().c_str())){
            data_selected = data;
        }
    }
    if (!data_selected.testType.empty() &&
        m_cb_testtype->findText(QString::fromStdString(data_selected.testType))<0)
    {
        m_cb_testtype->addItem(QString::fromStdString(data_selected.testType));
    }
    m_cb_testtype->setCurrentText(QString::fromStdString(data_selected.testType));
    m_cb_result->setCurrentText(QString::fromStdString(data_selected.testResult));
    m_te_comment->setText(QString::fromStdString(data_selected.comment));

    m_tw_testarea->setRowCount(0);
    for (int i_row=0; i_row<data_selected.testdata.size(); i_row++){
        m_tw_testarea->insertRow(i_row);

        QTableWidgetItem* m_stepIn = new QTableWidgetItem;
        QTableWidgetItem* m_stepOut = new QTableWidgetItem;
        m_stepIn->setText(QString::fromStdString(std::get<0>(data_selected.testdata[i_row])));
        m_stepOut->setText(QString::fromStdString(std::get<1>(data_selected.testdata[i_row])));

        m_tw_testarea->setItem(i_row, 0, m_stepIn);
        m_tw_testarea->setItem(i_row, 1, m_stepOut);
    }
}


bool MainWindow::showTestCaseTree(Project_data_t * proj_data)
{
    m_tv_testcase->setModel(nullptr);

    QStandardItemModel* m_standardModel = new QStandardItemModel();
    QStandardItem* rootNode = m_standardModel->invisibleRootItem();

    std::vector<QStandardItem*> lastItemPerLevel(proj_data->max_level+1);
    for(auto & elm : proj_data->node_data)
    {
        QStandardItem* testItem = new QStandardItem(elm.full_name.c_str());
        lastItemPerLevel[elm.level] = testItem;
        if (elm.level==1){
            rootNode->appendRow(testItem);
        }
        if (elm.level>1){
            lastItemPerLevel[elm.level-1]->appendRow(testItem);
        }
        if(elm.isTestCase){
            testItem->setIcon(QIcon("assets/icons/testcase.png"));
        } else {
            testItem->setIcon(QIcon("assets/icons/folder.png"));
        }

        if (elm.testResult == "Neutral") {
            QBrush tv_brush(QColor(192,192,192), Qt::SolidPattern);
            testItem->setBackground(tv_brush);
        }
        else if (elm.testResult == "Pass") {
            QBrush tv_brush(QColor(0,255,128), Qt::SolidPattern);
            testItem->setBackground(tv_brush);
        }
        else if (elm.testResult == "Fail") {
            QBrush tv_brush(QColor(255,102,102), Qt::SolidPattern);
            testItem->setBackground(tv_brush);
        }
    }
    m_tv_testcase->setModel(m_standardModel);
    m_tv_testcase->model()->setHeaderData(0, Qt::Horizontal, "TreeView");
    m_tv_testcase->expandAll();

    return true;
}

void MainWindow::addTestCaseStep()
{
    if (!m_tv_testcase->selectionModel()->selectedIndexes().count()){
        return;
    }
    QString uid = m_tv_testcase->currentIndex().data().toString().split(':').at(0).split("-").at(1);
    if (!m_proj->addCaseStep(uid.toInt())){
        return;
    }
    m_tw_testarea->insertRow(m_tw_testarea->rowCount());
}

void MainWindow::deleteTestCaseStep()
{
    if (!m_tv_testcase->selectionModel()->selectedIndexes().count()){
        return;
    }
    if (m_tw_testarea->selectionModel()->selectedIndexes().count() != 1){
        return;
    }
    QString uid = m_tv_testcase->currentIndex().data().toString().split(':').at(0).split("-").at(1);
    if (!m_proj->deleteCaseStep(uid.toInt(), m_tw_testarea->currentRow()))
    {
        return;
    }
    m_tw_testarea->removeRow(m_tw_testarea->currentRow());
}

void MainWindow::moveTestStepUp()
{
    if (!m_tv_testcase->selectionModel()->selectedIndexes().count()){
        return;
    }
    if (m_tw_testarea->selectionModel()->selectedIndexes().count() != 1){
        return;
    }
    QString uid = m_tv_testcase->currentIndex().data().toString().split(':').at(0).split("-").at(1);
    if (!m_proj->moveUpCaseStep(uid.toInt(), m_tw_testarea->currentRow())){
        return;
    }
    int current_row = m_tw_testarea->currentRow();
    for (int i_col=0; i_col<2; i_col++){
        QTableWidgetItem *item = m_tw_testarea->takeItem(current_row,i_col);
        QTableWidgetItem *itemAbove = m_tw_testarea->takeItem(current_row-1,i_col);
        m_tw_testarea->setItem(current_row,i_col,itemAbove);
        m_tw_testarea->setItem(current_row-1,i_col,item);
    }
}

void MainWindow::moveTestStepDown()
{
    if (!m_tv_testcase->selectionModel()->selectedIndexes().count()){
        return;
    }
    if (m_tw_testarea->selectionModel()->selectedIndexes().count() != 1){
        return;
    }
    QString uid = m_tv_testcase->currentIndex().data().toString().split(':').at(0).split("-").at(1);
    if(!m_proj->moveDownCaseStep(uid.toInt(), m_tw_testarea->rowCount(), m_tw_testarea->currentRow())){
        return;
    }
    int current_row = m_tw_testarea->currentRow();
    for (int i_col=0; i_col<2; i_col++){
        QTableWidgetItem *item = m_tw_testarea->takeItem(current_row,i_col);
        QTableWidgetItem *itemAbove = m_tw_testarea->takeItem(current_row+1,i_col);
        m_tw_testarea->setItem(current_row,i_col,itemAbove);
        m_tw_testarea->setItem(current_row+1,i_col,item);
    }
}

void MainWindow::updateTestCase()
{
    if (!m_tv_testcase->selectionModel()->selectedIndexes().count()){
        return;
    }
    int uid_selected = m_tv_testcase->currentIndex().data().toString().split(":").at(0).split("-").at(1).toInt();
    int idx_got;
    m_proj->getVecIndex(uid_selected, idx_got);

    Node_data_t mod_data;
    mod_data.uid = m_lb_uid->text().toInt();
    mod_data.name = m_le_name->text().toStdString();
    mod_data.full_name = "UID-"+m_lb_uid->text().toStdString()+": "+mod_data.name;
    mod_data.testType = m_cb_testtype->currentText().toStdString();
    mod_data.testResult = m_cb_result->currentText().toStdString();
    mod_data.isTestCase = m_proj->data()->node_data[idx_got].isTestCase;
    mod_data.comment = m_te_comment->toPlainText().toStdString();
    mod_data.level = m_proj->data()->node_data[idx_got].level;

    for (int i_step=0; i_step < m_tw_testarea->rowCount(); i_step++){
        // Qt6 has Segfault-Bug, if some cells remain empty aka. tablewidget.item.text() causes crash.
        std::string stepIn=(m_tw_testarea->item(i_step,0)==nullptr)?"":m_tw_testarea->item(i_step,0)->text().toStdString();
        std::string stepOut=(m_tw_testarea->item(i_step,1)==nullptr)?"":m_tw_testarea->item(i_step,1)->text().toStdString();
        mod_data.testdata.push_back(std::make_tuple(stepIn, stepOut));
    }
    m_proj->data()->node_data[idx_got] = mod_data;

    showTestCaseTree(m_proj->data());
}

void MainWindow::initProject()
{
    m_proj = new Project;
    Node_data_t node_add {99, "Default", "UID-99: Default", 1, true, "", "Neutral", "", {{"", ""}}};
    m_proj->data()->node_data.push_back(node_add);
    m_proj->data()->max_level = 1;
}

bool MainWindow::isNodeDeletable(int uid_select)
{
    bool b_match = true;
    if (!m_proj->compareSubMainEqual(uid_select, b_match)){
        return false;   // compare operation has failed
    }
    if (!b_match){
        return true;
    }
    return false;
}

void MainWindow::clearTestarea()
{
    m_cb_testtype->clear();
    m_cb_testtype->addItems({"Acceptance Test", "Destructive Test", "Functional Test", "Performance Test",
                            "Regression Test", "Smoke Test", "Unit Test"});
    m_cb_result->clear();
    m_cb_result->addItems({"Neutral", "Pass", "Fail"});
    updateResultGUI();

    m_lb_uid->clear();
    m_le_name->clear();
    m_te_comment->clear();
    m_tw_testarea->clear();
    m_tw_testarea->setRowCount(0);

    m_tw_testarea->setColumnCount(2);
    QTableWidgetItem *headerIn = new QTableWidgetItem();
    headerIn->setText("Step:");
    QTableWidgetItem *headerOut = new QTableWidgetItem();
    headerOut->setText("Test Result:");
    m_tw_testarea->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tw_testarea->setHorizontalHeaderItem(0, headerIn);
    m_tw_testarea->setHorizontalHeaderItem(1, headerOut);
    m_btn_up->setIcon(QIcon("assets/icons/up.png"));
    m_btn_down->setIcon(QIcon("assets/icons/down.png"));
    m_btn_plus->setIcon(QIcon("assets/icons/plus.png"));
    m_btn_minus->setIcon(QIcon("assets/icons/minus.png"));
}

void MainWindow::updateConfirmGUI()
{
    if (m_cb_tree_method->currentText() == "Delete"){
        m_le_tree_uid->setEnabled(false);
    } else {
        m_le_tree_uid->setEnabled(true);
    }
}


void MainWindow::updateResultGUI()
{
    if (m_cb_result->currentText() == "Neutral"){
        m_cb_result->setStyleSheet("QComboBox { background-color: rgb(192, 192, 192);}");
    }
    else if (m_cb_result->currentText() == "Pass"){
        m_cb_result->setStyleSheet("QComboBox { background-color: rgb(0, 255, 128);}");
    }
    else if (m_cb_result->currentText() == "Fail"){
        m_cb_result->setStyleSheet("QComboBox { background-color:  rgb(255, 102, 102);}");
    }
}
