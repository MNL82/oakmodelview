#pragma once

#include <QMainWindow>

#include "OakModel.h"

namespace Ui {
    class MainWindow;
}

// =============================================================================
// Class definition
// =============================================================================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_action_Save_triggered();
    void on_actionSave_As_triggered();
    void on_actionClose_triggered();
    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    Oak::Model::OakModel* m_oakModel;
};
