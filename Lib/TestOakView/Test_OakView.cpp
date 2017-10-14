/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Test_OakView.h"
#include "ui_Test_OakView.h"

#include "Test_ServiceFunctions.h"

#include "OakView.h"
#include "ParentDataView.h"

#include "MasterOakModel.h"

#include <QFileDialog>

// =============================================================================
// (public)
Test_OakView::Test_OakView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Test_OakView)
{
    ui->setupUi(this);

    connect(ui->actionMaster_New, SIGNAL(triggered(bool)), SLOT(onMasterNewTriggered()));
    connect(ui->actionMaster_Load, SIGNAL(triggered(bool)), SLOT(onMasterLoadTriggered()));
    connect(ui->actionMaster_Save, SIGNAL(triggered(bool)), SLOT(onMasterSaveTriggered()));
    connect(ui->actionMaster_SaveAs, SIGNAL(triggered(bool)), SLOT(onMasterSaveAsTriggered()));
    connect(ui->actionMaster_Close, SIGNAL(triggered(bool)), SLOT(onMasterCloseTriggered()));

    connect(ui->actionData_New, SIGNAL(triggered(bool)), SLOT(onDataNewTriggered()));
    connect(ui->actionData_Load, SIGNAL(triggered(bool)), SLOT(onDataLoadTriggered()));
    connect(ui->actionData_Save, SIGNAL(triggered(bool)), SLOT(onDataSaveTriggered()));
    connect(ui->actionData_SaveAs, SIGNAL(triggered(bool)), SLOT(onDataSaveAsTriggered()));
    connect(ui->actionData_Close, SIGNAL(triggered(bool)), SLOT(onDataCloseTriggered()));

    connect(ui->actionExit, SIGNAL(triggered(bool)), SLOT(onExitTriggered()));

    connect(ui->pushButton, SIGNAL(pressed()), SLOT(onGeneratePushed()));

    m_masterModel = new MasterOakModel();
    //m_masterModel->createNewRootDocument(Model::Node::Type::XML);
    ui->DesignView->setModel(m_masterModel->designModel());
    ui->DesignView_parent->setModel(m_masterModel->designModel());


    ui->DataView->setModel(m_masterModel->dataModel());
    ui->DataView_parent->setModel(m_masterModel->dataModel());
}

// =============================================================================
// (public)
Test_OakView::~Test_OakView()
{
    //m_masterModel->saveXMLRootNode("master.xml");
    delete m_masterModel;

    delete ui;
}

// =============================================================================
// (protected slots)
void Test_OakView::onMasterNewTriggered()
{
    m_masterModel->createNewRootDocument(Oak::Model::Node::Type::XML);
}

// =============================================================================
// (protected slots)
void Test_OakView::onMasterLoadTriggered()
{
    QString fileName = QString::fromStdString(m_masterModel->xmlDocFilePath());
    fileName = QFileDialog::getOpenFileName(this, "Load Master Database", fileName);
    if (fileName.isEmpty()) { return; }
    m_masterModel->loadXMLRootNode(fileName.toStdString());
}

// =============================================================================
// (protected slots)
void Test_OakView::onMasterSaveTriggered()
{
    m_masterModel->saveXMLRootNode();
}

// =============================================================================
// (protected slots)
void Test_OakView::onMasterSaveAsTriggered()
{
    QString fileName = QString::fromStdString(m_masterModel->xmlDocFilePath());
    fileName = QFileDialog::getSaveFileName(this, "Save Master Database", fileName);
    if (fileName.isEmpty()) { return; }
    m_masterModel->saveXMLRootNode(fileName.toStdString());
}

// =============================================================================
// (protected slots)
void Test_OakView::onMasterCloseTriggered()
{
    m_masterModel->clearRoot();
}

// =============================================================================
// (protected slots)
void Test_OakView::onDataNewTriggered()
{

}

// =============================================================================
// (protected slots)
void Test_OakView::onDataLoadTriggered()
{
    if (m_masterModel->isNull()) { return; }
    QString fileName = QString::fromStdString(m_masterModel->dataFilePath());
    fileName = QFileDialog::getOpenFileName(this, "Load Master Database", fileName);
    if (fileName.isEmpty()) { return; }
    m_masterModel->LoadDataFilePath(fileName.toStdString());
}

// =============================================================================
// (protected slots)
void Test_OakView::onDataSaveTriggered()
{

}

// =============================================================================
// (protected slots)
void Test_OakView::onDataSaveAsTriggered()
{

}

// =============================================================================
// (protected slots)
void Test_OakView::onDataCloseTriggered()
{

}

// =============================================================================
// (protected slots)
void Test_OakView::onExitTriggered()
{
    close();
}

// =============================================================================
// (protected slots)
void Test_OakView::onGeneratePushed()
{
    m_masterModel->generateDataNodeDefinition();
}
