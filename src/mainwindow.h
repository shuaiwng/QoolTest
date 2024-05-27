#pragma once
#include <QtWidgets>
#include "project.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    void showWindow();
    bool showTestCaseTree(std::vector<Node_info_t> node_vec, int max_level);

private:
    QWidget * m_mainwindow;
    QTreeView* m_tv_testcase;
};
