#include "mainwindow.h"
#include <QtUiTools>


MainWindow::MainWindow()
{
    QFile file("assets/gui_main.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    m_mainwindow = loader.load(&file, this);
    file.close();

    m_menu_close = m_mainwindow->findChild<QAction*>("actionClose");
    m_menu_openProj = m_mainwindow->findChild<QAction*>("actionOpen_Project");
    m_menu_saveProj = m_mainwindow->findChild<QAction*>("actionSave_Project");

    m_tv_testcase = m_mainwindow->findChild<QTreeView*>("tv_testcase");
    m_lb_uid = m_mainwindow->findChild<QLabel*>("lb_uid");
    m_te_comment = m_mainwindow->findChild<QTextEdit*>("te_comment");
    m_cb_testtype = m_mainwindow->findChild<QComboBox*>("cb_testtype");
    m_tw_testarea = m_mainwindow->findChild<QTableWidget*>("tw_testarea");
    m_tw_testarea->setColumnCount(2);
    QTableWidgetItem *headerIn = new QTableWidgetItem();
    headerIn->setText("Step:");
    QTableWidgetItem *headerOut = new QTableWidgetItem();
    headerOut->setText("Test Result:");
    m_tw_testarea->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tw_testarea->setHorizontalHeaderItem(0, headerIn);
    m_tw_testarea->setHorizontalHeaderItem(1, headerOut);

    connect(m_tv_testcase,SIGNAL(clicked(QModelIndex)), this, SLOT(displayTestCase()));
    connect(m_menu_close, SIGNAL(triggered()), this, SLOT(closeApp()));
    connect(m_menu_openProj, SIGNAL(triggered()), this, SLOT(openProject()));
    connect(m_menu_saveProj, SIGNAL(triggered()), this, SLOT(saveProject()));

    m_mainwindow->show();
}

MainWindow::~MainWindow()
{

}


void MainWindow::openProject()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Project"), ".", tr("Project File (*.qlpj)"));
    m_proj = new Project;
    m_proj->openProject(fileName.toStdString().c_str());
    this->showTestCaseTree(m_proj->data());
}

void MainWindow::saveProject()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Project"), ".", tr("Project File (*.qlpj)"));
    if(m_proj){
        m_proj->saveProject(fileName.toStdString().c_str());
    }
}

void MainWindow::closeApp()
{
    m_mainwindow->close();
    qApp->quit();
}


void MainWindow::displayTestCase()
{
    QString uid = m_tv_testcase->currentIndex().data().toString().split(":").at(0).split("-").at(1);
    m_lb_uid->setText(uid);

    Node_data_t data_selected;
    for (auto & data : m_proj->data()->node_data)
    {
        if (data.uid == std::stoi(uid.toStdString().c_str())){
            data_selected = data;
        }
    }
    m_cb_testtype->clear();
    m_cb_testtype->addItem(QString::fromStdString(data_selected.testType));
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

    QStandardItemModel* standardModel = new QStandardItemModel();
    QStandardItem* rootNode = standardModel->invisibleRootItem();

    std::vector<QStandardItem*> lastItemPerLevel(proj_data->max_level+1);
    for(auto & elm : proj_data->node_data)
    {
        QStandardItem * testItem = new QStandardItem(elm.full_name.c_str());
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
    }

    m_tv_testcase->setModel(standardModel);

    return true;
}
