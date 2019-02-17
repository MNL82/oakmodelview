#include "TableView.h"

#include "NodeQuery.h"

#include <QHeaderView>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDebug>


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
TableView::TableView(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);

    m_tableWidget = new QTableWidget();
    m_tableWidget->setStyleSheet(
        "QHeaderView::section {"
        "    background-color: #dddddd;"
        "    padding: 4px;"
        "    border-style: none;"
        "    border-bottom: 1px solid #fffff8;"
        "    border-right: 1px solid #fffff8;"
        "}"

        "QHeaderView::section:horizontal"
        "{"
        "    border-top: 1px solid #fffff8;"
        "}"

        "QHeaderView::section:vertical"
        "{"
        "    border-left: 1px solid #fffff8;"
        "}</string>");

    m_toolBar = new QToolBar();
    m_toolBar->setOrientation(Qt::Vertical);
    m_toolBar->setIconSize(QSize(16,16));
    m_toolBar->setVisible(false);



    // Add actions
    m_actionAdd = new QAction(QPixmap(":/OakView/Resources/add_32.png"), "Add");
    m_actionAdd->setEnabled(false);
    connect(m_actionAdd, SIGNAL(triggered()), this, SLOT(onActionAdd()));
    m_toolBar->addAction(m_actionAdd);

    m_actionDelete = new QAction(QPixmap(":/OakView/Resources/delete_32.png"), "Delete");
    m_actionDelete->setEnabled(false);
    m_actionDelete->setShortcut(QKeySequence::Delete);
    connect(m_actionDelete, SIGNAL(triggered()), this, SLOT(onActionDelete()));
    m_toolBar->addAction(m_actionDelete);

    m_toolBar->addSeparator();

    m_actionCut = new QAction(QPixmap(":/OakView/Resources/cut_32.png"), "Cut Node");
    m_actionCut->setEnabled(false);
    m_actionCut->setShortcut(QKeySequence::Cut);
    connect(m_actionCut, SIGNAL(triggered()), this, SLOT(onActionCut()));
    m_toolBar->addAction(m_actionCut);

    m_actionCopy = new QAction(QPixmap(":/OakView/Resources/copy_32.png"), "Copy Node");
    m_actionCopy->setEnabled(false);
    m_actionCopy->setShortcut(QKeySequence::Copy);
    connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(onActionCopy()));
    m_toolBar->addAction(m_actionCopy);

    m_actionPaste = new QAction(QPixmap(":/OakView/Resources/paste_32.png"), "Paste Node");
    m_actionPaste->setEnabled(false);
    m_actionPaste->setShortcut(QKeySequence::Paste);
    connect(m_actionPaste, SIGNAL(triggered()), this, SLOT(onActionPaste()));
    m_toolBar->addAction(m_actionPaste);

    m_toolBar->addSeparator();

    m_actionUp = new QAction(QPixmap(":/OakView/Resources/up_32.png"), "Move Up");
    m_actionUp->setEnabled(false);
    connect(m_actionUp, SIGNAL(triggered()), this, SLOT(onActionUp()));
    m_toolBar->addAction(m_actionUp);

    m_actionDown = new QAction(QPixmap(":/OakView/Resources/down_32.png"), "Move Down");
    m_actionDown->setEnabled(false);
    connect(m_actionDown, SIGNAL(triggered()), this, SLOT(onActionDown()));
    m_toolBar->addAction(m_actionDown);




    QHBoxLayout * layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->addWidget(m_tableWidget);
    layout->addWidget(m_toolBar);
    setLayout(layout);

    connect(m_tableWidget, SIGNAL(nodeSelectionChanged()), this, SLOT(onSelectionChanged()));
    connect(m_tableWidget, SIGNAL(nodeChanged(QTableWidgetItem *)), this, SLOT(onNodeChanged(QTableWidgetItem*)));
}

// =============================================================================
// (public)
TableView::~TableView()
{

}

// =============================================================================
// (public)
void TableView::setBaseRef(Model::NodeQueryUPtr baseRef)
{
    m_tableQuery.setNodeQuery(std::move(baseRef));
    updateTable();

}

// =============================================================================
// (public)
void TableView::addValueRef(Model::LeafQuerySPtr valueRef)
{
    m_tableQuery.addValueQuery(valueRef);
    updateTable();
}

// =============================================================================
// (public)
void TableView::updateTable()
{
    m_tableWidget->clearContents();
    disableAllActions();

    if (m_model == nullptr) { return; }
    if (m_model->rootNode().isNull()) { return; }

    m_rootNode = m_model->rootNode();

    int rowCount = m_tableQuery.count(m_rootNode);
    int columnCount = m_tableQuery.columnCount();

    m_tableWidget->blockSignals(true);
    m_tableWidget->setRowCount(rowCount);
    m_tableWidget->setColumnCount(columnCount);
    int row = 0;

    auto it = m_tableQuery.iterator(m_rootNode);
    while (it->next()) {
        // Add Table Header
        if (row == 0) {
            for (int column = 0; column < columnCount; column++)
            {
                m_tableWidget->setHorizontalHeaderItem(column, new QTableWidgetItem(QString::fromStdString(it->leaf(column).displayName())));
            }
            m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        // Add table values
        for (int column = 0; column < columnCount; column++)
        {
            std::string value = it->value<std::string>(column);
            QTableWidgetItem *node = new QTableWidgetItem(QString::fromStdString(value));
            m_tableWidget->setItem(row, column, node);
        }
        row++;
    }
    m_tableWidget->blockSignals(false);
    updateAllActions();
}

// =============================================================================
// (public)
void TableView::setOakModel(Model::OakModel *model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
//        m_model->notifier_currentNodeChanged.remove(this);
        m_model->notifier_rootNodeDefChanged.remove(this);
        m_model->notifier_rootNodeDataChanged.remove(this);
//        m_model->notifier_destroyed.remove(this);

        m_model->notifier_nodeInserteAfter.remove(this);
        m_model->notifier_nodeMoveAfter.remove(this);
        m_model->notifier_nodeCloneAfter.remove(this);
        m_model->notifier_nodeRemoveAfter.remove(this);

        m_model->notifier_leafChangeAfter.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
//        m_model->notifier_currentNodeChanged.add(this, &TableView::currentNodeChanged);
        m_model->notifier_rootNodeDefChanged.add(this, &TableView::updateTable);
        m_model->notifier_rootNodeDataChanged.add(this, &TableView::updateTable);
//        m_model->notifier_destroyed.add(this, &ListView::modelDestroyed);

        m_model->notifier_nodeInserteAfter.add(this, &TableView::onNodeInserteAfter);
        m_model->notifier_nodeMoveAfter.add(this, &TableView::onNodeMoveAfter);
        m_model->notifier_nodeCloneAfter.add(this, &TableView::onNodeCloneAfter);
        m_model->notifier_nodeRemoveBefore.add(this, &TableView::onNodeRemoveBefore);

        m_model->notifier_leafChangeAfter.add(this, &TableView::onEntryChangeAfter);
    }
}

// =============================================================================
// (protected slots)
void TableView::onSelectionChanged()
{
    updateAllActions();
}

// =============================================================================
// (protected slots)
void TableView::onActionAdd()
{
    m_tableQuery.nodeQuery().insertNode(m_rootNode, -1);
    updateTable();
}

// =============================================================================
// (protected slots)
void TableView::onActionDelete()
{
    QList<int> rows = selectedRows();
    if (rows.empty()) { return; }
    m_tableQuery.nodeQuery().removeNode(m_rootNode, rows.first());
    updateTable();
}

// =============================================================================
// (protected slots)
void TableView::onActionUp()
{

}

// =============================================================================
// (protected slots)
void TableView::onActionDown()
{

}

// =============================================================================
// (protected slots)
void TableView::onActionCut()
{

}
// =============================================================================
// (protected slots)

void TableView::onActionCopy()
{

}

// =============================================================================
// (protected slots)
void TableView::onActionPaste()
{

}

// =============================================================================
// (protected slots)
void TableView::onNodeChanged(QTableWidgetItem *node)
{
    int row = 0;
    auto it = m_tableQuery.iterator(m_rootNode);
    while (it->next()) {
        if (row == node->row()) {
            auto leaf = it->leaf(node->column());
            if (!leaf.setValue(node->text().toStdString())) {
                m_tableWidget->blockSignals(true);
                node->setText(QString::fromStdString(leaf.value<std::string>(node->column())));
                m_tableWidget->blockSignals(false);
            }
            return;
        }
        row++;
    }
}

// =============================================================================
// (protected)
void TableView::onNodeInserteAfter(const Model::NodeIndex &nodeIndex)
{
    Q_UNUSED(nodeIndex)
}

// =============================================================================
// (protected)
void TableView::onNodeMoveAfter(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex)
{
    Q_UNUSED(sourceNodeIndex)
    Q_UNUSED(targetNodeIndex)
}

// =============================================================================
// (protected)
void TableView::onNodeCloneAfter(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex)
{
    Q_UNUSED(sourceNodeIndex)
    Q_UNUSED(targetNodeIndex)
}

// =============================================================================
// (protected)
void TableView::onNodeRemoveBefore(const Model::NodeIndex &nodeIndex)
{
    Q_UNUSED(nodeIndex)
}

// =============================================================================
// (protected)
void TableView::onEntryChangeAfter(const Model::NodeIndex &nodeIndex, const std::string &valueName)
{
    Q_UNUSED(nodeIndex)
    Q_UNUSED(valueName)
}

// =============================================================================
// (protected)
bool TableView::event(QEvent *event)
{
    if (event->type()==QEvent::Enter) {
        m_toolBar->setVisible(true);
    } else if (event->type()==QEvent::Leave) {
        m_toolBar->setVisible(false);
    }

    return QWidget::event(event);
}

// =============================================================================
// (protected)
void TableView::disableAllActions()
{
    m_actionAdd->setEnabled(false);
    m_actionDelete->setEnabled(false);
    m_actionUp->setEnabled(false);
    m_actionDown->setEnabled(false);
    m_actionCut->setEnabled(false);
    m_actionCopy->setEnabled(false);
    m_actionPaste->setEnabled(false);
}

// =============================================================================
// (protected)
void TableView::updateAllActions()
{
    QList<int> rows = selectedRows();

    m_actionAdd->setEnabled(true);

    if (rows.count() == 0) {
        m_actionDelete->setEnabled(false);
        m_actionUp->setEnabled(false);
        m_actionDown->setDisabled(false);
        m_actionCut->setEnabled(false);
        m_actionCopy->setEnabled(false);
    } else {
        m_actionDelete->setEnabled(m_tableQuery.nodeQuery().canRemoveNode(m_rootNode, rows.first()));
        m_actionUp->setDisabled(rows.contains(0));
        m_actionDown->setDisabled(rows.contains(m_tableWidget->rowCount()-1));
        m_actionCut->setEnabled(true);
        m_actionCopy->setEnabled(true);
    }

    m_actionPaste->setDisabled(m_cutNodes.empty() && m_copyNodes.empty());
}

// =============================================================================
// (protected)
QList<int> TableView::selectedRows() const
{
    QList<int> list;
    QList<QTableWidgetItem*> selectedItems = m_tableWidget->selectedItems();
    for (QTableWidgetItem *node: selectedItems)
    {
        if (!list.contains(node->row())) {
            list.push_back(node->row());
        }
    }
    return list;
}

} // namespace Oak::View::QtWidgets

