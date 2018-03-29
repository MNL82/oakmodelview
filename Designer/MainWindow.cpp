#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

#include "ModelDesignDef.h"

using namespace Oak::Model;

// =============================================================================
// (public)
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->OakView->setColumnCount(2);
    ui->OakView->setHeaderLabels(QStringList() << "Type" << "Name");

    m_oakModel = new OakModel();

    ui->NodeDataView->setModel(m_oakModel);
    ui->ParentDataView->setModel(m_oakModel);
    ui->OakView->setOakModel(m_oakModel);

    ui->splitter->setSizes(QList<int>() << 300 << 700);

    //createMasterModel();
    m_oakModel->setRootNodeDef(ModelDesignDef::MakeSPtr());
}

// =============================================================================
// (public)
MainWindow::~MainWindow()
{
    delete ui;
}

// =============================================================================
// (public)
void MainWindow::on_actionNew_triggered()
{
    m_oakModel->createNewRootDocument(Node::Type::XML);
    m_oakModel->setRootItem(m_oakModel->rootItem());
    ui->OakView->expandAll();
}

// =============================================================================
// (public)
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QString::fromStdString(m_oakModel->xmlDocFilePath());
    fileName = QFileDialog::getOpenFileName(this, "Load Database", fileName);
    if (fileName.isEmpty()) { return; }
    m_oakModel->loadXMLRootNode(fileName.toStdString());
    m_oakModel->setRootItem(m_oakModel->rootItem());
    ui->OakView->expandAll();
}

// =============================================================================
// (public)
void MainWindow::on_action_Save_triggered()
{
    m_oakModel->saveXMLRootNode();
}

// =============================================================================
// (public)
void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QString::fromStdString(m_oakModel->xmlDocFilePath());
    fileName = QFileDialog::getSaveFileName(this, "Save Database", fileName);
    if (fileName.isEmpty()) { return; }
    m_oakModel->saveXMLRootNode(fileName.toStdString());
}

// =============================================================================
// (public)
void MainWindow::on_actionClose_triggered()
{
    m_oakModel->clearRoot();
}

// =============================================================================
// (public)
void MainWindow::on_actionExit_triggered()
{
    close();
}
