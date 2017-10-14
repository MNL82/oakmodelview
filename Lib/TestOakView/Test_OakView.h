/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include <QMainWindow>

#include "OakModel.h"
#include "MasterOakModel.h"

namespace Ui {
class Test_OakView;
}

// =============================================================================
// Class definition
// =============================================================================
class Test_OakView : public QMainWindow
{
    Q_OBJECT
public:
    explicit Test_OakView(QWidget *parent = 0);
    ~Test_OakView();

protected slots:
    void onMasterNewTriggered();
    void onMasterLoadTriggered();
    void onMasterSaveTriggered();
    void onMasterSaveAsTriggered();
    void onMasterCloseTriggered();

    void onDataNewTriggered();
    void onDataLoadTriggered();
    void onDataSaveTriggered();
    void onDataSaveAsTriggered();
    void onDataCloseTriggered();

    void onExitTriggered();

    void onGeneratePushed();

private:
    Ui::Test_OakView *ui;
    Oak::Model::MasterOakModel* m_masterModel;
};

