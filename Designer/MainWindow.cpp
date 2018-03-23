#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

#include "NodeDefBuilder.h"
#include "ContainerDefBuilder.h"
#include "ValueDefBuilder.h"
#include "XMLRefFactory.h"

#include "ModelDesignDef.h"

using namespace Oak::Model;

std::string MainWindow::s_master    = "master";
std::string MainWindow::s_data      = "data";
std::string MainWindow::s_filter    = "filter";
std::string MainWindow::s_version   = "version";
std::string MainWindow::s_major     = "major";
std::string MainWindow::s_minor     = "minor";
std::string MainWindow::s_patch     = "patch";
std::string MainWindow::s_filePath  = "filePath";

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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    m_oakModel->createNewRootDocument(Node::Type::XML);
    m_oakModel->setRootItem(m_oakModel->rootItem());
    ui->OakView->expandAll();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QString::fromStdString(m_oakModel->xmlDocFilePath());
    fileName = QFileDialog::getOpenFileName(this, "Load Database", fileName);
    if (fileName.isEmpty()) { return; }
    m_oakModel->loadXMLRootNode(fileName.toStdString());
    m_oakModel->setRootItem(m_oakModel->rootItem());
    ui->OakView->expandAll();
}

void MainWindow::on_action_Save_triggered()
{
    m_oakModel->saveXMLRootNode();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QString::fromStdString(m_oakModel->xmlDocFilePath());
    fileName = QFileDialog::getSaveFileName(this, "Save Database", fileName);
    if (fileName.isEmpty()) { return; }
    m_oakModel->saveXMLRootNode(fileName.toStdString());
}

void MainWindow::on_actionClose_triggered()
{
    m_oakModel->clearRoot();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
