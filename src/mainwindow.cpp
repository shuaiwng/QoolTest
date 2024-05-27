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

}

MainWindow::~MainWindow()
{
}

void MainWindow::showWindow()
{
    m_mainwindow->show();
}

bool MainWindow::showTestCaseTree(std::vector<Node_info_t> node_vec, int max_level)
{
    QStandardItemModel* standardModel = new QStandardItemModel();
    QStandardItem* rootNode = standardModel->invisibleRootItem();

    std::vector<QStandardItem*> lastItemPerLevel(max_level+1);
    for(auto & elm : node_vec)
    {
        QStandardItem * testItem = new QStandardItem(elm.name.c_str());
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
