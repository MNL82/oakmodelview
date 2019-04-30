#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QDateTime>
#include <QCryptographicHash>
#include <QMessageBox>

#include "NodeDefBuilder.h"
#include "ContainerDefBuilder.h"
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
    m_oakModel->createNewRootDocument(NodeData::Type::XML);
    m_oakModel->setRootNode(m_oakModel->rootNode());
    ui->OakView->expandAll();
    m_oakModel->setCurrentNode(m_oakModel->rootNode());
}

// =============================================================================
// (private slots)
void MainWindow::on_actionLoad_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Load From Archive", QString::fromStdString(m_oakModel->docFilePathXML()));
    if (filePath.isEmpty()) { return; }
    if (m_oakModel->loadRootNodeXML(filePath.toStdString())) {
        ui->OakView->expandAll();
        setWindowTitle("OData Browser - " + filePath);
        ui->OakView->resizeColumnToContents(0);
    }
}

// =============================================================================
// (private slots)
void MainWindow::on_actionSave_triggered()
{
    if (m_oakModel->docFilePathXML().empty()) {
        on_actionSaveAs_triggered();
        return;
    }
    if (!m_oakModel->saveRootNodeXML()) {
        QMessageBox::warning(this, "Save failed", "Could not save file");
    }
    return;
}

// =============================================================================
// (private slots)
void MainWindow::on_actionSaveAs_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save As", QString::fromStdString(m_oakModel->docFilePathXML()));
    if (!m_oakModel->saveRootNodeXML(filePath.toStdString())) {
        QMessageBox::warning(this, "Save Failed", "Faild to save file to:\n\n" + filePath);
    }
}

// =============================================================================
// (private slots)
void MainWindow::on_actionClose_triggered()
{
    m_oakModel->clearRoot();
}
