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


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
OakView::OakView(QWidget* parent)
    : QTreeWidget(parent)
{
    //qDebug() << "OakView()";

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    setDragDropMode(QAbstractItemView::DragDrop);

    connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(onCurrentQNodeChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
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
        m_model->notifier_currentNodeChanged.remove(this);
        m_model->notifier_rootNodeDefChanged.remove(this);
        m_model->notifier_rootNodeDataChanged.remove(this);
        m_model->notifier_destroyed.remove(this);

        m_model->notifier_nodeInserteAfter.remove(this);
        m_model->notifier_nodeMoveAfter.remove(this);
        m_model->notifier_nodeCloneAfter.remove(this);
        m_model->notifier_nodeRemoveBefore.remove(this);

        m_model->notifier_variantLeafChangeAfter.remove(this);
        m_model->notifier_keyLeafChangeAfter.remove(this);

        if (!m_model->isNull()) {
            clearTreeStructure();
        }
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentNodeChanged.add(this, &OakView::currentNodeChanged);
        m_model->notifier_rootNodeDefChanged.add(this, &OakView::updateTreeStructure);
        m_model->notifier_rootNodeDataChanged.add(this, &OakView::updateTreeStructure);
        m_model->notifier_destroyed.add(this, &OakView::modelDestroyed);

        m_model->notifier_nodeInserteAfter.add(this, &OakView::onNodeInserteAfter);
        m_model->notifier_nodeMoveAfter.add(this, &OakView::onNodeMoveAfter);
        m_model->notifier_nodeCloneAfter.add(this, &OakView::onNodeCloneAfter);
        m_model->notifier_nodeRemoveBefore.add(this, &OakView::onNodeRemoveBefore);

        m_model->notifier_variantLeafChangeAfter.add(this, &OakView::onVariantLeafChangeAfter);
        m_model->notifier_keyLeafChangeAfter.add(this, &OakView::onKeyLeafChangeAfter);

        createTreeStructure();
    }
}

// =============================================================================
// (public)
void OakView::currentNodeChanged()
{
    if (m_model && (m_model->currentNode().isNodeNull() || !m_model->isNull())) {
        setCurrentNode(m_model->currentNodeIndex());
    }
}

// =============================================================================
// (public)
void OakView::setCurrentNode(const Model::NodeIndex& nodeIndex)
{
    if (nodeIndex.isNull()) {
        clearSelection();
        return;
    }

    QTreeWidgetItem * newQItem = widgetFromIndex(nodeIndex);

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
            QTreeWidgetItem * cNode = currentItem();
            QTreeWidgetItem * parentNode = cNode->parent();

            if (parentNode) {
                Model::NodeIndexUPtr nodeIndex = indexFromWidget(cNode);
                Model::Node currentNode = nodeIndex->node(m_model->rootNode());
                Model::Node parentNode = currentNode.parent();
                int destIndex = nodeIndex->lastNodeIndex().index();
                if (event->modifiers() & Qt::ControlModifier) {
                    // Clone Node
                    if (event->key() == Qt::Key_Down) { destIndex++; }
                    parentNode.cloneChild(destIndex, currentNode);
                } else if (event->modifiers() & Qt::ShiftModifier) {
                    // Move Node
                    if (event->key() == Qt::Key_Up)   { destIndex--; }
                    if (event->key() == Qt::Key_Down) { destIndex++; }
                    if (destIndex >= 0 && destIndex < parentNode.childCount()) {
                        parentNode.moveChild(destIndex, currentNode);
                    }
                }
            }
            event->accept();
            return;
        }
    } else if (event->key() == Qt::Key_Delete) {
        Model::NodeIndexUPtr nodeIndex = indexFromWidget(currentItem());
        if (!nodeIndex) {
            Model::Node parentNode = nodeIndex->nodeParent(m_model->rootNode());
            if (!parentNode.isNull()) {
                parentNode.removeChild(nodeIndex->lastNodeIndex().index());
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
        QTreeWidgetItem * qNode = currentItem();
        if (qNode) {
            m_dragNode = indexFromWidget(qNode)->node(m_model->rootNode());
            if (!m_dragNode.isNull()) {
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

    QTreeWidgetItem * qNode = itemAt(event->pos());
    if (qNode == nullptr) {
        event->ignore();
        return;
    }

    Model::Node node = indexFromWidget(qNode)->node(m_model->rootNode());
    if (node.isNull()) {
        event->ignore();
        return;
    }

    if (indiPos == OnItem) {
        int index = 0;
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            if (node.canCloneChild(index, m_dragNode)) {
                event->setDropAction(Qt::CopyAction);
                return;
            }
        } else {
            if (node.canMoveChild(index, m_dragNode)) {
                event->setDropAction(Qt::MoveAction);
                return;
            }
        }
        event->ignore();
    } else { // AboveNode or BelowNode
        Model::Node parentNode = node.parent();
        if (parentNode.isNull()) { return; }
        int index = parentNode.childIndex(node);
        if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
            if (parentNode.canCloneChild(index, m_dragNode)) {
                event->setDropAction(Qt::CopyAction);
                return;
            }
        } else {
            if (parentNode.canMoveChild(index, m_dragNode)) {
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

    QTreeWidgetItem * cNode = itemAt(event->pos());
    if (cNode == nullptr) { return; }
    int newIndex;
    Model::Node targetNode;

    Model::NodeIndexUPtr targetIndex = indexFromWidget(cNode);

    auto indiPos = dropIndicatorPosition();
    if (indiPos == OnItem) {
        newIndex = 0;
        targetNode = targetIndex->node(m_model->rootNode());
    } else {
        newIndex = targetIndex->lastNodeIndex().index();
        targetNode = targetIndex->nodeParent(m_model->rootNode());
        if (indiPos == BelowItem) {
            newIndex++;
        }
    }

    if (targetNode.isNull()) { event->ignore(); }

    if (QApplication::keyboardModifiers() & Qt::ControlModifier) {
        targetNode.cloneChild(newIndex, m_dragNode);
    } else {
        targetNode.moveChild(newIndex, m_dragNode);
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
        addTopLevelItem(getTreeNodes(m_model->rootNode()));
        currentNodeChanged();
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
QTreeWidgetItem * OakView::getTreeNodes(const Model::Node &node, QTreeWidgetItem *parentNode)
{
    if (node.isNull()) { return nullptr; }

    std::vector<std::string> values;
    values.push_back(node.def()->displayName());
    if (node.def()->hasKey()) { values.push_back(node.keyLeaf().toString()); }

    QTreeWidgetItem * elementNode;
    if (parentNode) { elementNode = new QTreeWidgetItem(parentNode, toQStringList(values)); }
    else      { elementNode = new QTreeWidgetItem(toQStringList(values)); }

    Model::Node childNode = node.firstChild();
    while (!childNode.isNull()) {
        getTreeNodes(childNode, elementNode);
        childNode = node.nextChild(childNode);
    }
    return elementNode;
}

// =============================================================================
// (protected)
void OakView::onNodeInserteAfter(const Model::NodeIndex &nodeIndex)
{
    QTreeWidgetItem* parentWidget = widgetFromIndex(nodeIndex, true);
    int insertIndex = nodeIndex.lastNodeIndex().index();
    parentWidget->insertChild(insertIndex, getTreeNodes(nodeIndex.node(m_model->rootNode())));
}

// =============================================================================
// (protected)
void OakView::onNodeMoveAfter(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex)
{
    QTreeWidgetItem* sourceParentWidget = widgetFromIndex(sourceNodeIndex, true);
    QTreeWidgetItem* targetParentWidget = widgetFromIndex(targetNodeIndex, true);

    int sourceIndex = sourceNodeIndex.lastNodeIndex().index();
    int targetIndex = targetNodeIndex.lastNodeIndex().index();

    blockSignals(true);
    targetParentWidget->insertChild(targetIndex, sourceParentWidget->takeChild(sourceIndex));
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onNodeCloneAfter(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex)
{
    QTreeWidgetItem* sourceWidget = widgetFromIndex(sourceNodeIndex);
    QTreeWidgetItem* targetParentWidget = widgetFromIndex(targetNodeIndex, true);

    int targetIndex = targetNodeIndex.lastNodeIndex().index();

    blockSignals(true);
    targetParentWidget->insertChild(targetIndex, sourceWidget->clone());
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onNodeRemoveBefore(const Model::NodeIndex &nodeIndex)
{
    QTreeWidgetItem* removeWidget = widgetFromIndex(nodeIndex);

    blockSignals(true);
    removeWidget->parent()->removeChild(removeWidget);
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onVariantLeafChangeAfter(const Model::NodeIndex &nodeIndex)
{
    // Child nodes can change when the variant definition change
    QTreeWidgetItem* qNode = widgetFromIndex(nodeIndex);
    QTreeWidgetItem* qParentNode = qNode->parent();
    int index = qParentNode->indexOfChild(qNode);
    blockSignals(true);
    qParentNode->insertChild(index, getTreeNodes(nodeIndex.node(m_model->rootNode())));
    qParentNode->removeChild(qNode);
    blockSignals(false);
}

// =============================================================================
// (protected)
void OakView::onKeyLeafChangeAfter(const Model::NodeIndex &nodeIndex)
{
    QTreeWidgetItem* qNode = widgetFromIndex(nodeIndex);
    Model::Node node = nodeIndex.node(m_model->rootNode());
    qNode->setText(1, QString::fromStdString(node.keyLeaf().toString()));
}

// =============================================================================
// (protected)
QTreeWidgetItem *OakView::widgetFromIndex(const Model::NodeIndex &nodeIndex, bool parentWidget)
{
    QTreeWidgetItem * currentWidget = this->topLevelItem(0);
    Model::NodeIndexUPtr unnamedIndex = m_model->convertNodeIndexToUnnamed(nodeIndex);
    const Model::NodeIndex *currentIndex = unnamedIndex.get();
    while (!currentIndex->isNull()) {
        if (parentWidget && !currentIndex->hasChildNodeIndex()) { break; } // Return the next to last widget
        currentWidget = currentWidget->child(currentIndex->index());
        currentIndex = &currentIndex->childNodeIndex();
    }
    return currentWidget;
}

// =============================================================================
// (protected)
Model::NodeIndexUPtr OakView::indexFromWidget(QTreeWidgetItem *nodeWidget)
{
    if (nodeWidget == nullptr) { return Model::NodeIndexUPtr(); }
    QTreeWidgetItem * currentWidget = nodeWidget;
    QTreeWidgetItem * parentWidget = currentWidget->parent();

    Model::NodeIndex *currentIndex = nullptr;
    Model::NodeIndex *parentIndex = nullptr;
    while(parentWidget) {
        // Create next node index
        parentIndex = new Model::NodeIndex(parentWidget->indexOfChild(currentWidget));
        parentIndex->setChildNodeIndex(currentIndex);

        // Prepare for next iteration
        currentIndex = parentIndex;
        currentWidget = parentWidget;
        parentWidget = currentWidget->parent();
    }
    return Model::NodeIndexUPtr(currentIndex);
}

// =============================================================================
// (protected slots)
void OakView::onCurrentQNodeChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (current) {
        Model::NodeIndexUPtr nodeIndex = indexFromWidget(current);
        if (nodeIndex) {
            m_model->setCurrentNode(nodeIndex->node(m_model->rootNode()));
        }
    }

}

} // namespace Oak::View::QtWidgets

