#include <QtCore>
#include <QtWidgets>
#include "mainwindow.h"
#include "project.h"


int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    MainWindow main_window;

    Project prj;
    prj.openProject();

    // QTreeView* treeView = new QTreeView();
    // QStandardItemModel* standardModel = new QStandardItemModel();
    // QStandardItem *rootNode = standardModel->invisibleRootItem();

    // //defining a couple of items
    // QStandardItem *americaItem = new QStandardItem("America");
    // americaItem->setIcon(QIcon("assets/testIcon.png"));
    // QStandardItem *mexicoItem =  new QStandardItem("Canada");
    // QStandardItem *usaItem =     new QStandardItem("USA");
    // QStandardItem *bostonItem =  new QStandardItem("Boston");
    // QStandardItem *europeItem =  new QStandardItem("Europe");
    // QStandardItem *italyItem =   new QStandardItem("Italy");
    // QStandardItem *romeItem =    new QStandardItem("Rome");
    // QStandardItem *veronaItem =  new QStandardItem("Verona");

    // //building up the hierarchy
    // rootNode->    appendRow(americaItem);
    // rootNode->    appendRow(europeItem);
    // americaItem-> appendRow(mexicoItem);
    // americaItem-> appendRow(usaItem);
    // usaItem->     appendRow(bostonItem);
    // europeItem->  appendRow(italyItem);
    // italyItem->   appendRow(romeItem);
    // italyItem->   appendRow(veronaItem);

    // //register the model
    // treeView->setModel(standardModel);
    // // treeView->expandAll();




    return app.exec();
}

