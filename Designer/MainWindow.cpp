#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

#include "NodeDefinitionBuilder.h"
#include "ContainerDefinitionBuilder.h"
#include "ValueDefinitionBuilder.h"
#include "XMLRefFactory.h"

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
    ui->OakView->setModel(m_oakModel);

    ui->splitter->setSizes(QList<int>() << 300 << 700);

    createMasterModel();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    m_oakModel->createNewRootDocument(Node::Type::XML);
    m_oakModel->setRootItem(m_oakModel->rootItem().firstChild("process"));
    ui->OakView->expandAll();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QString::fromStdString(m_oakModel->xmlDocFilePath());
    fileName = QFileDialog::getOpenFileName(this, "Load Database", fileName);
    if (fileName.isEmpty()) { return; }
    m_oakModel->loadXMLRootNode(fileName.toStdString());
    m_oakModel->setRootItem(m_oakModel->rootItem().firstChild("process"));
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

// =============================================================================
// (protected)
void MainWindow::createMasterModel()
{
    auto nodeProject = NDB::Make("project");
    auto nodeProcess = NDB::Make("process");
    auto nodeColumn = NDB::Make("Column");
    NDB::addValueDefAsKey(nodeColumn, VDB::MakeXML("", "", "Name", "Noname"));
    NDB::addValueDef(nodeColumn, VDB::MakeXML(0.0, "ColumnVolume", "", 1.0));

    NDB::addValueDef(nodeColumn, VDB::MakeXML("", "ColumnPosition", ""));

    auto nodeStep = NDB::MakeRoot("Step", "");
    NDB::addValueDefAsKey(nodeStep, VDB::MakeXML("", "", "Name", "Noname"));
    NDB::addValueDefAsDerivedId(nodeStep, VDB::MakeXML("", "", "Type", "Wash"));

    auto valueFlowrate = VDB::MakeXML(0.0, "Flowrate","");
    VDB::setValueId(valueFlowrate, "Flow rate");
    VDB::settings(valueFlowrate).setUnit("mL/min");
    NDB::addValueDef(nodeStep, std::move(valueFlowrate));

    auto valueInletA = VDB::MakeXML("", "InletA", "", "A1");
    std::vector<std::string> optionsA = {"A1","A2","A3","A4","A5","A6","A7","A8"};
    VDB::setOptions(valueInletA, optionsA);
    NDB::addValueDef(nodeStep, std::move(valueInletA));

    auto valueInletB = VDB::MakeXML("", "InletB", "", "B1");
    std::vector<std::string> optionsB = {"B1","B2","B3","B4","B5","B6","B7","B8"};
    VDB::setOptions(valueInletB, optionsB);
    NDB::addValueDef(nodeStep, std::move(valueInletB));

    NDB::addValueDef(nodeStep, VDB::MakeXML(Bool(), "PumpWash","", Bool(true)));

    auto valuePctB = VDB::MakeXML(0.0, "pctB","");
    VDB::setValueId(valuePctB, "%B");
    NDB::addValueDef(nodeStep, std::move(valuePctB));

    NDB::addValueDef(nodeStep, VDB::MakeXML(0.0, "CV",""));


    auto nodeStep_equil = NDB::MakeDerived(nodeStep, "Equil");
    NDB::addValueDef(nodeStep_equil, VDB::MakeXML("", "","TEST1"));

    auto nodeStep_load = NDB::MakeDerived(nodeStep, "Load");
    NDB::addValueDef(nodeStep_load, VDB::MakeXML("", "","TEST2"));

    auto nodeStep_wash = NDB::MakeDerived(nodeStep, "Wash");
    NDB::addValueDef(nodeStep_wash, VDB::MakeXML("", "","TEST3"));

    auto nodeStep_elute = NDB::MakeDerived(nodeStep, "Elute");
    NDB::addValueDef(nodeStep_elute, VDB::MakeXML("", "","TEST4"));

    auto nodeStep_cip = NDB::MakeDerived(nodeStep, "CIP");
    NDB::addValueDef(nodeStep_cip, VDB::MakeXML("", "","TEST5"));

    NDB::addContainerDef(nodeProject, CDB::Make(nodeProcess, 1, 1));
    NDB::addContainerDef(nodeProcess, CDB::Make(nodeColumn));
    NDB::addContainerDef(nodeColumn, CDB::Make(nodeStep));

    m_oakModel->setRootNodeDefinition(nodeProject);
}
