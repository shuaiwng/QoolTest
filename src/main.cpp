#include <QtCore>
#include <QtWidgets>
#include "mainwindow.h"
#include "project.h"


int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    Project prj;
    std::vector<Node_data_t> data_vec = prj.openProject();

    int max_level = prj.getMaxLevel();

    MainWindow main_window;
    main_window.getProjectData(data_vec);


    main_window.showWindow();

    main_window.showTestCaseTree(data_vec, max_level);




    return app.exec();
}

