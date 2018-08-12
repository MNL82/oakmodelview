#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>
#include <QMessageBox>

#include "NodeDefBuilder.h"
#include "ContainerDefBuilder.h"
#include "QueryRef.h"
#include "ODataMetadataDef.h"

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
    ui->OakView->setOakModel(m_oakModel);
    ui->toolBar_ModelActions->setModel(m_oakModel);

    ui->splitter->setSizes(QList<int>() << 350 << 650 << 0);

    //m_oakModel->setRootNodeDef(createModel());
    m_oakModel->setRootNodeDef(Oak::Model::ODataMetadataDef::MakeSPtr());
}

// =============================================================================
// (public)
MainWindow::~MainWindow()
{
    delete ui;
}

// =============================================================================
// (private)
NodeDefSPtr MainWindow::createModel()
{
    auto edmx = NDB::create("edmx:Edmx")
        ->setDisplayName("Edmx")
        ->addValueDef(VDB::create(UnionType::String, "Version")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Version")))
        ->addValueDef(VDB::create(UnionType::String, "Edmx")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("xmlns:edmx")));

    auto dataServices = NDB::create("edmx:DataServices")
        ->setDisplayName("DataSercices");

    edmx->addContainerDef(CDB::create(dataServices, 1, 1));

    auto schema = NDB::create("Schema")
        ->addValueKey(VDB::create(UnionType::String, "Namespace")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Namespace")))
        ->addValueDef(VDB::create(UnionType::String, "xmlns")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("xmlns")));

    dataServices->addContainerDef(CDB::create(schema, 1));

    auto entityType = NDB::create("EntityType")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")));

    schema->addContainerDef(CDB::create(entityType));

    auto propertyKey = NDB::create("PropertyRef")
        ->setDisplayName("Property Key")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")));

    entityType->addContainerDef(CDB::create(propertyKey)
        ->setElementListRef(Oak::XML::ListRef(Oak::XML::ChildRef::MakeUPtr("Key"), "PropertyRef")));

    auto property = NDB::create("Property")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")))
        ->addValueDef(VDB::create(UnionType::Bool, "Nullable")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Nullable"))
            ->setDefaultValue(true));

    entityType->addContainerDef(CDB::create(property));

    auto navigationProperty = NDB::create("NavigationProperty")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")));

    entityType->addContainerDef(CDB::create(navigationProperty));

    auto action = NDB::create("Action")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::Bool, "IsBound")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("IsBound"))
            ->setDefaultValue(false));

    schema->addContainerDef(CDB::create(action));

    auto parameter = NDB::create("Parameter")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type"))
            ->setDefaultValue(false));

    action->addContainerDef(CDB::create(parameter));

    auto returnType = NDB::create("ReturnType")
        ->addValueKey(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")));

    action->addContainerDef(CDB::create(returnType));

    auto entityContainer = NDB::create("EntityContainer")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")));

    schema->addContainerDef(CDB::create(entityContainer, 1, 1));

    auto entitySet = NDB::create("EntitySet")
        ->addValueKey(VDB::create(UnionType::String, "Name")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::String, "EntityType")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("EntityType")));

    entityContainer->addContainerDef(CDB::create(entitySet));

    auto navigationPropertyBinding = NDB::create("NavigationPropertyBinding")
        ->addValueKey(VDB::create(UnionType::String, "Path")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Path")))
        ->addValueKey(VDB::create(UnionType::String, "Target")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Target")));

    entitySet->addContainerDef(CDB::create(navigationPropertyBinding));

    return edmx->get();
}

// =============================================================================
// (private)
QByteArray MainWindow::hashFromFile(const QString &filePath) const
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly)) {
        hash.addData(file.readAll());
    } else {
        qDebug() << "Faild to create hash of file: " << filePath;
        return QByteArray();
        // Handle "cannot open file" error
    }

    // Retrieve the SHA1 signature of the file
    return hash.result();

}

// =============================================================================
// (private slots)
void MainWindow::on_actionNew_triggered()
{
    m_oakModel->createNewRootDocument(Node::Type::XML);
    m_oakModel->setRootItem(m_oakModel->rootItem());
    ui->OakView->expandAll();
    m_oakModel->setCurrentItem(m_oakModel->rootItem().firstChild("columns").firstChild("column"));
}

// =============================================================================
// (private slots)
void MainWindow::on_actionLoad_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Load From Archive", QString::fromStdString(m_oakModel->xmlDocFilePath()));
    if (filePath.isEmpty()) { return; }
    if (m_oakModel->loadXMLRootNode(filePath.toStdString())) {
        ui->OakView->expandAll();
        setWindowTitle("Continuous Chromatography - " + filePath);
        ui->OakView->resizeColumnToContents(0);
    }
}

// =============================================================================
// (private slots)
void MainWindow::on_actionSave_triggered()
{
    if (m_oakModel->xmlDocFilePath().empty()) {
        on_actionSaveAs_triggered();
        return;
    }
    if (!m_oakModel->saveXMLRootNode()) {
        QMessageBox::warning(this, "Save failed", "Could not save file");
    }
    return;
}

// =============================================================================
// (private slots)
void MainWindow::on_actionSaveAs_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save As", QString::fromStdString(m_oakModel->xmlDocFilePath()));
    if (!m_oakModel->saveXMLRootNode(filePath.toStdString())) {
        QMessageBox::warning(this, "Save Failed", "Faild to save file to:\n\n" + filePath);
    }
}

// =============================================================================
// (private slots)
void MainWindow::on_actionClose_triggered()
{
    m_oakModel->clearRoot();
}
