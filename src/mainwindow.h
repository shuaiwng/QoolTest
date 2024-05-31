#pragma once
#include <QtWidgets>
#include "project.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

    bool showTestCaseTree(Project_data_t * proj_data);

public slots:
    void displayTestCase();
    void closeApp();
    void openProject();
    void saveProject();

private:
    QAction * m_menu_close;
    QAction * m_menu_openProj;
    QAction * m_menu_closeProj;
    QAction * m_menu_saveProj;

    QWidget * m_mainwindow;
    QTreeView* m_tv_testcase;
    QLabel* m_lb_uid;
    QTextEdit* m_te_comment;
    QComboBox* m_cb_testtype;
    QTableWidget* m_tw_testarea;

    Project * m_proj;
};
