#pragma once

#include <QMainWindow>
#include <QDir>

#include "OakModel.h"
#include "NodeDef.h"

#define FILE_NAME "global-settings.xml"
#define PROD_SETTINGS_FILE_PATH "//sc19/CultivationProcess/data/global-settings.xml"

using namespace Oak::Model;

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

private:
    QByteArray hashFromFile(const QString &file) const;

private slots:
    void on_actionNew_triggered();

    void on_actionLoad_triggered();

    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();

    void on_actionClose_triggered();


private:
    Ui::MainWindow *ui;
    OakModel* m_oakModel;
};
