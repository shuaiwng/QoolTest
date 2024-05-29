#include "mainwindow.h"
#include <QtUiTools/QtUiTools>
#include <QtUiTools/QUiLoader>


MainWindow::MainWindow()
{
    QFile file("assets/main.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    m_mainwindow = loader.load(&file, this);
    file.close();

    m_tv_testcase = m_mainwindow->findChild<QTreeView*>("tv_testcase");
    m_lb_uid = m_mainwindow->findChild<QLabel*>("lb_uid");
    m_te_comment = m_mainwindow->findChild<QTextEdit*>("te_comment");
    m_cb_testtype = m_mainwindow->findChild<QComboBox*>("cb_testtype");
    m_gb_testarea = m_mainwindow->findChild<QGroupBox*>("gb_testarea");

    connect(m_tv_testcase,SIGNAL(clicked(QModelIndex)), this, SLOT(displayTestCase()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::getProjectData(std::vector<Node_data_t> data)
{
    m_data_vec = data;
}

void MainWindow::displayTestCase()
{
    QString uid = m_tv_testcase->currentIndex().data().toString().split(":").at(0).split("-").at(1);
    m_lb_uid->setText(uid);

    Node_data_t data_selected;
    for (auto & data : m_data_vec)
    {
        if (data.uid == std::stoi(uid.toStdString().c_str())){
            data_selected = data;
        }
    }
    m_cb_testtype->clear();
    m_cb_testtype->addItem(QString::fromStdString(data_selected.testType));
    m_te_comment->setText(QString::fromStdString(data_selected.comment));

    // display detailed test steps
    QVBoxLayout * lay_total = new QVBoxLayout;
    delete m_gb_testarea->layout();
    for (auto & teststep : data_selected.testdata){
        QHBoxLayout* lay_step = new QHBoxLayout;
        QTextEdit * stepIn = new QTextEdit;
        QTextEdit * stepOut = new QTextEdit;
        stepIn->setText(QString::fromStdString(std::get<0>(teststep)));
        stepOut->setText(QString::fromStdString(std::get<1>(teststep)));
        lay_step->addWidget(stepIn);
        lay_step->addSpacing(30);
        lay_step->addWidget(stepOut);

        lay_total->addLayout(lay_step);
        lay_total->addSpacing(30);
    }
    m_gb_testarea->setLayout(lay_total);
}

void MainWindow::showWindow()
{
    m_mainwindow->show();
}

bool MainWindow::showTestCaseTree(std::vector<Node_data_t> data_vec, int max_level)
{
    QStandardItemModel* standardModel = new QStandardItemModel();
    QStandardItem* rootNode = standardModel->invisibleRootItem();

    std::vector<QStandardItem*> lastItemPerLevel(max_level+1);
    for(auto & elm : data_vec)
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
