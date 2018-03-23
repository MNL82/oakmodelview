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
#include "NodeIndex.h"

namespace Oak {
namespace View {

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

        m_model->notifier_itemInserted.remove(this);
        m_model->notifier_itemMoved.remove(this);
        m_model->notifier_itemCloned.remove(this);
        m_model->notifier_itemRemoved.remove(this);

        m_model->notifier_itemValueChanged.remove(this);

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

        m_model->notifier_itemInserted.add(this, &OakView::onItemInserted);
        m_model->notifier_itemMoved.add(this, &OakView::onItemMoved);
        m_model->notifier_itemCloned.add(this, &OakView::onItemCloned);
        m_model->notifier_itemRemoved.add(this, &OakView::onItemRemoved);

        m_model->notifier_itemValueChanged.add(this, &OakView::onItemValueChanged);

        createTreeStructure();
    }
}

// =============================================================================
// (public)
void OakView::currentItemChanged()
{
    if (m_model && !m_model->isNull()) {
        setCurrentItem(m_model->currentItem());
    }
}

// =============================================================================
// (public)
void OakView::setCurrentItem(const Model::Item& item)
{
    if (item.isNull()) {
        clearSelection();
        return;
    }

    NodeIndex newIndex(item);
    QTreeWidgetItem * newQItem = newIndex.qItem(topLevelItem(0));

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
                NodeIndex currentIndex(cItem);
                Model::Item currentItem = currentIndex.item(m_model->rootItem());
                Model::Item parentItem = currentItem.parent();
                int destIndex = currentIndex.back();
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
        NodeIndex nodeIndex(currentItem());
        if (!nodeIndex.empty()) {
            int childIndex = nodeIndex.back();
            nodeIndex.pop_back();
            Model::Item parentItem = nodeIndex.item(m_model->rootItem());
            parentItem.removeChild(childIndex);
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
            m_dragItem = NodeIndex(qItem).item(m_model->rootItem());
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

    Model::Item item = NodeIndex(qItem).item(m_model->rootItem());
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

    QTreeWidgetItem * item = itemAt(event->pos());
    if (item == nullptr) { return; }
    int newIndex;
    Model::Item targetItem;

    NodeIndex targetIndex(item);
    auto indiPos = dropIndicatorPosition();
    if (indiPos == OnItem) {
        newIndex = 0;
        targetItem = targetIndex.item(m_model->rootItem());
    } else {
        newIndex = targetIndex.back();
        targetIndex.pop_back();
        targetItem = targetIndex.item(m_model->rootItem());
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
    createTreeStructure();
}

// =============================================================================
// (protected)
QTreeWidgetItem * OakView::getTreeItems(Model::Item item, QTreeWidgetItem *parentItem)
{
    if (item.isNull()) { return nullptr; }

    std::vector<std::string> values;
    values.push_back(item.def()->displayName());
    if (item.def()->hasKey()) { values.push_back(item.valueKey().toString()); }

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
void OakView::onItemInserted(const Model::Item &parentItem, int index)
{
    QTreeWidgetItem* pItem = NodeIndex(parentItem).qItem(this->topLevelItem(0));
    pItem->insertChild(index, getTreeItems(parentItem.childAt(index)));
}

// =============================================================================
// (protected)
void OakView::onItemMoved(const Model::Item &sourceParentItem, int sourceIndex, const Model::Item &targetParentItem, int targetIndex)
{
    QTreeWidgetItem* sourceQItem = NodeIndex(sourceParentItem).qItem(topLevelItem(0));
    QTreeWidgetItem* targetQItem = NodeIndex(targetParentItem).qItem(topLevelItem(0));

    blockSignals(true);
    targetQItem->insertChild(targetIndex, sourceQItem->takeChild(sourceIndex));
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onItemCloned(const Model::Item &sourceParentItem, int sourceIndex, const Model::Item &targetParentItem, int targetIndex)
{
    QTreeWidgetItem* sourceParentQItem = NodeIndex(sourceParentItem).qItem(topLevelItem(0));
    QTreeWidgetItem* targetParentQItem = NodeIndex(targetParentItem).qItem(topLevelItem(0));

    blockSignals(true);
    targetParentQItem->insertChild(targetIndex, sourceParentQItem->child(sourceIndex)->clone());
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onItemRemoved(const Model::Item &parentItem, int index)
{
    QTreeWidgetItem* parentQItem = NodeIndex(parentItem).qItem(topLevelItem(0));
    blockSignals(true);
    parentQItem->removeChild(parentQItem->child(index));
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onItemValueChanged(const Model::Item &item, int valueIndex)
{
    if (item.def()->derivedIdValueDefIndex() == valueIndex) {
        // Child items can change when the derived definition change
        QTreeWidgetItem* qItem = NodeIndex(item).qItem(topLevelItem(0));
        QTreeWidgetItem* qParentItem = qItem->parent();
        int index = qParentItem->indexOfChild(qItem);
        blockSignals(true);
        qParentItem->insertChild(index, getTreeItems(item));
        qParentItem->removeChild(qItem);
        blockSignals(false);
    } else if (item.def()->keyValueDefIndex() == valueIndex) {
        QTreeWidgetItem* qItem = NodeIndex(item).qItem(topLevelItem(0));
        qItem->setText(1, QString::fromStdString(item.valueKey().toString()));
    }
}

// =============================================================================
// (protected slots)
void OakView::onCurrentQItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (current) {
        NodeIndex index(current);
        m_model->setCurrentItem(index.item(m_model->rootItem()));
    }

}

} // namespace View
} // namespace Oak
