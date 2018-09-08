#include "TableView.h"

#include <QHeaderView>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDebug>

namespace Oak {
namespace View {

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

    m_actionCut = new QAction(QPixmap(":/OakView/Resources/cut_32.png"), "Cut Item");
    m_actionCut->setEnabled(false);
    m_actionCut->setShortcut(QKeySequence::Cut);
    connect(m_actionCut, SIGNAL(triggered()), this, SLOT(onActionCut()));
    m_toolBar->addAction(m_actionCut);

    m_actionCopy = new QAction(QPixmap(":/OakView/Resources/copy_32.png"), "Copy Item");
    m_actionCopy->setEnabled(false);
    m_actionCopy->setShortcut(QKeySequence::Copy);
    connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(onActionCopy()));
    m_toolBar->addAction(m_actionCopy);

    m_actionPaste = new QAction(QPixmap(":/OakView/Resources/paste_32.png"), "Paste Item");
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

    connect(m_tableWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onSelectionChanged()));
    connect(m_tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(onItemChanged(QTableWidgetItem*)));
}

// =============================================================================
// (public)
TableView::~TableView()
{

}

// =============================================================================
// (public)
void TableView::setBaseRef(Model::ItemQueryUPtr baseRef)
{
    m_tableQuery.setItemQuery(std::move(baseRef));
    updateTable();

}

// =============================================================================
// (public)
void TableView::addValueRef(Model::EntryQuerySPtr valueRef)
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
    if (m_model->rootItem().isNull()) { return; }

    m_rootItem = m_model->rootItem();

    int rowCount = m_tableQuery.count(m_rootItem);
    int columnCount = m_tableQuery.columnCount();

    m_tableWidget->blockSignals(true);
    m_tableWidget->setRowCount(rowCount);
    m_tableWidget->setColumnCount(columnCount);
    int row = 0;

    auto it = m_tableQuery.iterator(m_rootItem);
    while (it->next()) {
        // Add Table Header
        if (row == 0) {
            for (int column = 0; column < columnCount; column++)
            {
                m_tableWidget->setHorizontalHeaderItem(column, new QTableWidgetItem(QString::fromStdString(it->entry(column).displayName())));
            }
            m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
        // Add table values
        for (int column = 0; column < columnCount; column++)
        {
            std::string value = it->value<std::string>(column);
            QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(value));
            m_tableWidget->setItem(row, column, item);
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
//        m_model->notifier_currentItemChanged.remove(this);
        m_model->notifier_rootNodeDefChanged.remove(this);
        m_model->notifier_rootNodeChanged.remove(this);
//        m_model->notifier_destroyed.remove(this);

        m_model->notifier_itemInserted.remove(this);
        m_model->notifier_itemMoved.remove(this);
        m_model->notifier_itemCloned.remove(this);
        m_model->notifier_itemRemoved.remove(this);

        m_model->notifier_entryChanged.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
//        m_model->notifier_currentItemChanged.add(this, &TableView::currentItemChanged);
        m_model->notifier_rootNodeDefChanged.add(this, &TableView::updateTable);
        m_model->notifier_rootNodeChanged.add(this, &TableView::updateTable);
//        m_model->notifier_destroyed.add(this, &ListView::modelDestroyed);

        m_model->notifier_itemInserted.add(this, &TableView::onItemInserted);
        m_model->notifier_itemMoved.add(this, &TableView::onItemMoved);
        m_model->notifier_itemCloned.add(this, &TableView::onItemCloned);
        m_model->notifier_itemRemoved.add(this, &TableView::onItemRemoved);

        m_model->notifier_entryChanged.add(this, &TableView::onEntryChanged);
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
    m_tableQuery.itemQuery().addItem(m_rootItem);
    updateTable();
}

// =============================================================================
// (protected slots)
void TableView::onActionDelete()
{

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
void TableView::onItemChanged(QTableWidgetItem *item)
{
    int row = 0;
    auto it = m_tableQuery.iterator(m_rootItem);
    while (it->next()) {
        if (row == item->row()) {
            auto entry = it->entry(item->column());
            if (!entry.setValue(item->text().toStdString())) {
                m_tableWidget->blockSignals(true);
                item->setText(QString::fromStdString(entry.value<std::string>(item->column())));
                m_tableWidget->blockSignals(false);
            }
            return;
        }
        row++;
    }
}

// =============================================================================
// (protected)
void TableView::onItemInserted(const Model::Item &parentItem, int index)
{
    Q_UNUSED(parentItem)
    Q_UNUSED(index)
}

// =============================================================================
// (protected)
void TableView::onItemMoved(const Model::Item &sourceParentItem, int sourceIndex, const Model::Item &targetParentItem, int targetIndex)
{
    Q_UNUSED(sourceParentItem)
    Q_UNUSED(sourceIndex)
    Q_UNUSED(targetParentItem)
    Q_UNUSED(targetIndex)
}

// =============================================================================
// (protected)
void TableView::onItemCloned(const Model::Item &sourceParentItem, int sourceIndex, const Model::Item &targetParentItem, int targetIndex)
{
    Q_UNUSED(sourceParentItem)
    Q_UNUSED(sourceIndex)
    Q_UNUSED(targetParentItem)
    Q_UNUSED(targetIndex)
}

// =============================================================================
// (protected)
void TableView::onItemRemoved(const Model::Item &parentItem, int index)
{
    Q_UNUSED(parentItem)
    Q_UNUSED(index)
}

// =============================================================================
// (protected)
void TableView::onEntryChanged(const Model::Item &item, int valueIndex)
{
    Q_UNUSED(item)
    Q_UNUSED(valueIndex)
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
    QList<int> selectedRows;
    QList<QTableWidgetItem*> selectedItems = m_tableWidget->selectedItems();
    for (QTableWidgetItem *item: selectedItems)
    {
        if (!selectedRows.contains(item->row())) {
            selectedRows.push_back(item->row());
        }
    }
    m_actionAdd->setEnabled(true);

    if (selectedRows.count() == 0) {
        m_actionDelete->setEnabled(false);
        m_actionUp->setEnabled(false);
        m_actionDown->setDisabled(false);
        m_actionCut->setEnabled(false);
        m_actionCopy->setEnabled(false);
    } else {
        m_actionDelete->setEnabled(true);
        m_actionUp->setDisabled(selectedRows.contains(0));
        m_actionDown->setDisabled(selectedRows.contains(m_tableWidget->rowCount()-1));
        m_actionCut->setEnabled(true);
        m_actionCopy->setEnabled(true);
    }

    m_actionPaste->setDisabled(m_cutItems.empty() && m_copyItems.empty());
}

} // namespace View
} // namespace Oak
