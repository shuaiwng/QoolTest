#include "mainwindow.h"
#include <QtUiTools/QtUiTools>
#include <QtUiTools/QUiLoader>


MainWindow::MainWindow()
{
    QFile file("assets/main.ui");
    file.open(QIODevice::ReadOnly);
    QUiLoader loader;
    QWidget *mainWindow = loader.load(&file, this);
    file.close();

    mainWindow->show();
}

MainWindow::~MainWindow()
{
}
