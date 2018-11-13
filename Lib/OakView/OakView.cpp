/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "OakView.h"

#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QDebug>
#include <QKeyEvent>
#include <QApplication>
#include <QPainter>
#include <QDrag>

#include "ServiceFunctions.h"


namespace Oak::View {

// =============================================================================
// (public)
OakView::OakView(QWidget* parent)
    : QTreeWidget(parent)
{
    //qDebug() << "OakView()";

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setDragDropMode(QAbstractItemView::DragDrop);

    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(onCurrentQItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
}

// =============================================================================
// (public)
OakView::~OakView()
{
    //qDebug() << "~OakView()";
    setOakModel(nullptr);
}

// =============================================================================
// (public)
void OakView::setOakModel(Model::OakModel* model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentItemChanged.remove(this);
        m_model->notifier_rootNodeDefChanged.remove(this);
        m_model->notifier_rootNodeChanged.remove(this);
        m_model->notifier_destroyed.remove(this);

        m_model->notifier_itemInserteAfter.remove(this);
        m_model->notifier_itemMoveAfter.remove(this);
        m_model->notifier_itemCloneAfter.remove(this);
        m_model->notifier_itemRemoveBefore.remove(this);

        m_model->notifier_entryTypeChangeAfter.remove(this);
        m_model->notifier_entryKeyChangeAfter.remove(this);

        if (!m_model->isNull()) {
            clearTreeStructure();
        }
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentItemChanged.add(this, &OakView::currentItemChanged);
        m_model->notifier_rootNodeDefChanged.add(this, &OakView::updateTreeStructure);
        m_model->notifier_rootNodeChanged.add(this, &OakView::updateTreeStructure);
        m_model->notifier_destroyed.add(this, &OakView::modelDestroyed);

        m_model->notifier_itemInserteAfter.add(this, &OakView::onItemInserteAfter);
        m_model->notifier_itemMoveAfter.add(this, &OakView::onItemMoveAfter);
        m_model->notifier_itemCloneAfter.add(this, &OakView::onItemCloneAfter);
        m_model->notifier_itemRemoveBefore.add(this, &OakView::onItemRemoveBefore);

        m_model->notifier_entryTypeChangeAfter.add(this, &OakView::onEntryTypeChangeAfter);
        m_model->notifier_entryKeyChangeAfter.add(this, &OakView::onEntryKeyChangeAfter);

        createTreeStructure();
    }
}

// =============================================================================
// (public)
void OakView::currentItemChanged()
{
    if (m_model && (m_model->currentItem().isNodeNull() || !m_model->isNull())) {
        setCurrentItem(m_model->currentItemIndex());
    }
}

// =============================================================================
// (public)
void OakView::setCurrentItem(const Model::ItemIndex& itemIndex)
{
    if (itemIndex.isNull()) {
        clearSelection();
        return;
    }

    QTreeWidgetItem * newQItem = widgetFromIndex(itemIndex);

    // Check if the selection changed
    if (selectedItems().count() == 1 &&
        selectedItems().first() == newQItem) {
        return;
    }

    // Update the selection
    SignalBlocker block(this);
    clearSelection();
    newQItem->setSelected(true);
    QTreeWidget::setCurrentItem(newQItem);
}

// =============================================================================
// (protected)
void OakView::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)) {
        if (event->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier)) {
            if (!m_model) {
                qDebug() << "OakView::keyPressEvent() Data model is not valid";
                return;
            }
            QTreeWidgetItem * cItem = currentItem();
            QTreeWidgetItem * parentItem = cItem->parent();

            if (parentItem) {
                Model::ItemIndexUPtr itemIndex = indexFromWidget(cItem);
                Model::Item currentItem = itemIndex->item(m_model->rootItem());
                Model::Item parentItem = currentItem.parent();
                int destIndex = itemIndex->lastItemIndex().index();
                if (event->modifiers() & Qt::ControlModifier) {
                    // Clone Item
                    if (event->key() == Qt::Key_Down) { destIndex++; }
                    parentItem.cloneChild(destIndex, currentItem);
                } else if (event->modifiers() & Qt::ShiftModifier) {
                    // Move Item
                    if (event->key() == Qt::Key_Up)   { destIndex--; }
                    if (event->key() == Qt::Key_Down) { destIndex++; }
                    if (destIndex >= 0 && destIndex < parentItem.childCount()) {
                        parentItem.moveChild(destIndex, currentItem);
                    }
                }
            }
            event->accept();
            return;
        }
    } else if (event->key() == Qt::Key_Delete) {
        Model::ItemIndexUPtr itemIndex = indexFromWidget(currentItem());
        if (!itemIndex) {
            Model::Item parentItem = itemIndex->itemParent(m_model->rootItem());
            if (!parentItem.isNull()) {
                parentItem.removeChild(itemIndex->lastItemIndex().index());
            }
        }
    }
    QTreeWidget::keyPressEvent(event);
}

// =============================================================================
// (protected)
void OakView::mousePressEvent(QMouseEvent *event)
{
//    QTreeWidget::mousePressEvent(event);

    m_mousePressedPosition = event->pos();
    QTreeWidget::mousePressEvent(event);
}

// =============================================================================
// (protected)
void OakView::dragEnterEvent(QDragEnterEvent * event)
{
//    QTreeWidget::dragEnterEvent(event);
    if (event->source() == this) {
        setDropIndicatorShown(true);
        QTreeWidgetItem * qItem = currentItem();
        if (qItem) {
            m_dragItem = indexFromWidget(qItem)->item(m_model->rootItem());
            if (!m_dragItem.isNull()) {
                event->acceptProposedAction();
                event->accept();
                setState(DraggingState);
                return;
            }
        }
    }
    event->ignore();
}

// =============================================================================
// (protected)
void OakView::dragLeaveEvent(QDragLeaveEvent * event)
{
//    QTreeWidget::dragLeaveEvent(event);
    setDropIndicatorShown(false);
    QTreeWidget::dragLeaveEvent(event);
}

// =============================================================================
// (protected)
void OakView::dragMoveEvent(QDragMoveEvent * event)
{
    if (event->source() != this) {
        event->ignore();
        return;
    }

    QTreeWidget::dragMoveEvent(event);

    // ignore by default
    if (!event->isAccepted()) { return; }

    QAbstractItemView::DropIndicatorPosition indiPos = dropIndicatorPosition();
    if (indiPos == OnViewport) {
        event->ignore();
        return;
    }

    QTreeWidgetItem * qItem = itemAt(event->pos());
    if (qItem == nullptr) {
        event->ignore();
        return;
    }

    Model::Item item = indexFromWidget(qItem)->item(m_model->rootItem());
    if (item.isNull()) {
        event->ignore();
        return;
    }

    if (indiPos == OnItem) {
        int index = 0;
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            if (item.canCloneChild(index, m_dragItem)) {
                event->setDropAction(Qt::CopyAction);
                return;
            }
        } else {
            if (item.canMoveChild(index, m_dragItem)) {
                event->setDropAction(Qt::MoveAction);
                return;
            }
        }
        event->ignore();
    } else { // AboveItem or BelowItem
        Model::Item parentItem = item.parent();
        if (parentItem.isNull()) { return; }
        int index = parentItem.childIndex(item);
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            if (parentItem.canCloneChild(index, m_dragItem)) {
                event->setDropAction(Qt::CopyAction);
                return;
            }
        } else {
            if (parentItem.canMoveChild(index, m_dragItem)) {
                event->setDropAction(Qt::MoveAction);
                return;
            }
        }
        event->ignore();
    }
}

// =============================================================================
// (protected)
void OakView::dropEvent(QDropEvent * event)
{
    // ignore the event prevents Qt from doing any default actions
    event->ignore();

    setDropIndicatorShown(false);

    if (event->source() != this) { return; }

    QTreeWidgetItem * cItem = itemAt(event->pos());
    if (cItem == nullptr) { return; }
    int newIndex;
    Model::Item targetItem;

    Model::ItemIndexUPtr targetIndex = indexFromWidget(cItem);

    auto indiPos = dropIndicatorPosition();
    if (indiPos == OnItem) {
        newIndex = 0;
        targetItem = targetIndex->item(m_model->rootItem());
    } else {
        newIndex = targetIndex->lastItemIndex().index();
        targetItem = targetIndex->itemParent(m_model->rootItem());
        if (indiPos == BelowItem) {
            newIndex++;
        }
    }

    if (targetItem.isNull()) { event->ignore(); }

    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        targetItem.cloneChild(newIndex, m_dragItem);
    } else {
        targetItem.moveChild(newIndex, m_dragItem);
    }


    event->ignore();
}

// =============================================================================
// (protected)
void OakView::startDrag(Qt::DropActions supportedActions)
{
    Q_UNUSED(supportedActions);

    QModelIndexList indexList = selectedIndexes();
    if (indexList.isEmpty()) { return; }

    QRect rect;
    foreach (QModelIndex index, indexList) {
        rect = rect.united(visualRect(index));
    }
    if (rect.isEmpty()) { return; }

    QMimeData *data = model()->mimeData(indexList);

    rect.adjust(viewport()->pos().x(),viewport()->pos().y(),viewport()->pos().x(),viewport()->pos().y());
    QPixmap pixmap(rect.size());
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    render(&painter, QPoint(), QRegion(rect));

    QDrag *drag = new QDrag(this);
    drag->setPixmap(pixmap);
    drag->setMimeData(data);
    QPoint imagePos(m_mousePressedPosition.x() - rect.left(),rect.top() - m_mousePressedPosition.y());
    drag->setHotSpot(imagePos);
    drag->exec(Qt::CopyAction | Qt::MoveAction | Qt::IgnoreAction, Qt::MoveAction);
}

// =============================================================================
// (protected)
void OakView::modelDestroyed()
{
    m_model = nullptr;
    clearTreeStructure();
}

// =============================================================================
// (protected)
void OakView::createTreeStructure()
{
    if (m_model && !m_model->isNull()) {
        addTopLevelItem(getTreeItems(m_model->rootItem()));
        currentItemChanged();
    }
}

// =============================================================================
// (protected)
void OakView::clearTreeStructure()
{
    clear();
}

// =============================================================================
// (protected)
void OakView::updateTreeStructure()
{
    clearTreeStructure();
    reset();
    createTreeStructure();
    reset();
}

// =============================================================================
// (protected)
QTreeWidgetItem * OakView::getTreeItems(const Model::Item &item, QTreeWidgetItem *parentItem)
{
    if (item.isNull()) { return nullptr; }

    std::vector<std::string> values;
    values.push_back(item.def()->displayName());
    if (item.def()->hasKey()) { values.push_back(item.entryKey().toString()); }

    QTreeWidgetItem * elementItem;
    if (parentItem) { elementItem = new QTreeWidgetItem(parentItem, toQStringList(values)); }
    else      { elementItem = new QTreeWidgetItem(toQStringList(values)); }

    Model::Item childNode = item.firstChild();
    while (!childNode.isNull()) {
        getTreeItems(childNode, elementItem);
        childNode = item.nextChild(childNode);
    }
    return elementItem;
}

// =============================================================================
// (protected)
void OakView::onItemInserteAfter(const Model::ItemIndex &itemIndex)
{
    QTreeWidgetItem* parentWidget = widgetFromIndex(itemIndex, true);
    int insertIndex = itemIndex.lastItemIndex().index();
    parentWidget->insertChild(insertIndex, getTreeItems(itemIndex.item(m_model->rootItem())));
}

// =============================================================================
// (protected)
void OakView::onItemMoveAfter(const Model::ItemIndex &sourceItemIndex, const Model::ItemIndex &targetItemIndex)
{
    QTreeWidgetItem* sourceParentWidget = widgetFromIndex(sourceItemIndex, true);
    QTreeWidgetItem* targetParentWidget = widgetFromIndex(targetItemIndex, true);

    int sourceIndex = sourceItemIndex.lastItemIndex().index();
    int targetIndex = targetItemIndex.lastItemIndex().index();

    blockSignals(true);
    targetParentWidget->insertChild(targetIndex, sourceParentWidget->takeChild(sourceIndex));
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onItemCloneAfter(const Model::ItemIndex &sourceItemIndex, const Model::ItemIndex &targetItemIndex)
{
    QTreeWidgetItem* sourceWidget = widgetFromIndex(sourceItemIndex);
    QTreeWidgetItem* targetParentWidget = widgetFromIndex(targetItemIndex, true);

    int targetIndex = targetItemIndex.lastItemIndex().index();

    blockSignals(true);
    targetParentWidget->insertChild(targetIndex, sourceWidget->clone());
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onItemRemoveBefore(const Model::ItemIndex &itemIndex)
{
    QTreeWidgetItem* removeWidget = widgetFromIndex(itemIndex);

    blockSignals(true);
    removeWidget->parent()->removeChild(removeWidget);
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onEntryTypeChangeAfter(const Model::ItemIndex &itemIndex)
{
    // Child items can change when the variant definition change
    QTreeWidgetItem* qItem = widgetFromIndex(itemIndex);
    QTreeWidgetItem* qParentItem = qItem->parent();
    int index = qParentItem->indexOfChild(qItem);
    blockSignals(true);
    qParentItem->insertChild(index, getTreeItems(itemIndex.item(m_model->rootItem())));
    qParentItem->removeChild(qItem);
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onEntryKeyChangeAfter(const Model::ItemIndex &itemIndex)
{
    QTreeWidgetItem* qItem = widgetFromIndex(itemIndex);
    Model::Item item = itemIndex.item(m_model->rootItem());
    qItem->setText(1, QString::fromStdString(item.entryKey().toString()));
}

// =============================================================================
// (protected)
QTreeWidgetItem *OakView::widgetFromIndex(const Model::ItemIndex &itemIndex, bool parentWidget)
{
    QTreeWidgetItem * currentWidget = this->topLevelItem(0);
    Model::ItemIndexUPtr unnamedIndex = m_model->convertItemIndexToUnnamed(itemIndex);
    const Model::ItemIndex *currentIndex = unnamedIndex.get();
    while (!currentIndex->isNull()) {
        if (parentWidget && !currentIndex->hasChildItemIndex()) { break; } // Return the next to last widget
        currentWidget = currentWidget->child(currentIndex->index());
        currentIndex = &currentIndex->childItemIndex();
    }
    return currentWidget;
}

// =============================================================================
// (protected)
Model::ItemIndexUPtr OakView::indexFromWidget(QTreeWidgetItem *itemWidget)
{
    if (itemWidget == nullptr) { return Model::ItemIndexUPtr(); }
    QTreeWidgetItem * currentWidget = itemWidget;
    QTreeWidgetItem * parentWidget = currentWidget->parent();

    Model::ItemIndex *currentIndex = nullptr;
    Model::ItemIndex *parentIndex = nullptr;
    while(parentWidget) {
        // Create next item index
        parentIndex = new Model::ItemIndex(parentWidget->indexOfChild(currentWidget));
        parentIndex->setChildItemIndex(currentIndex);

        // Prepare for next iteration
        currentIndex = parentIndex;
        currentWidget = parentWidget;
        parentWidget = currentWidget->parent();
    }
    return Model::ItemIndexUPtr(currentIndex);
}

// =============================================================================
// (protected slots)
void OakView::onCurrentQItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (current) {
        m_model->setCurrentItem(indexFromWidget(current)->item(m_model->rootItem()));
    }

}

} // namespace Oak::View

