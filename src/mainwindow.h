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
    bool showTestCaseTree(std::vector<Node_data_t> data_vec, int max_level);
    void getProjectData(std::vector<Node_data_t>);

public slots:
    void displayTestCase();

private:
    QWidget * m_mainwindow;
    QTreeView* m_tv_testcase;
    QLabel* m_lb_uid;
    QTextEdit* m_te_comment;
    QComboBox* m_cb_testtype;
    QGroupBox* m_gb_testarea;

    std::vector<Node_data_t> m_data_vec;
};
