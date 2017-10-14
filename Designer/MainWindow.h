#pragma once

#include <QMainWindow>

#include "OakModel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_action_Save_triggered();

    void on_actionSave_As_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

private:
    void createMasterModel();

private:
    Ui::MainWindow *ui;
    Oak::Model::OakModel* m_oakModel;


    static std::string s_master;
    static std::string s_data;
    static std::string s_filter;
    static std::string s_version;
    static std::string s_major;
    static std::string s_minor;
    static std::string s_patch;
    static std::string s_filePath;
};
