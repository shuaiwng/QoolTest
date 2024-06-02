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
    void saveAsProject();
    void execTreeConfirm();


private:
    QAction * m_menu_close;
    QAction * m_menu_openProj;
    QAction * m_menu_closeProj;
    QAction * m_menu_saveAsProj;

    QWidget * m_mainwindow;
    QTreeView* m_tv_testcase;
    QLabel* m_lb_uid;
    QTextEdit* m_te_comment;
    QComboBox* m_cb_testtype;
    QTableWidget* m_tw_testarea;

    QComboBox* m_cb_tree_method;
    QLineEdit* m_le_tree_uid;
    QCheckBox* m_ckb_tree_as_child;
    QPushButton* m_btn_tree_confirm;

    Project * m_proj;
};
