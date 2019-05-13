/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QOakToolButtonRemove.h"

#include "QOakToolbarModel.h"

#include <QIcon>

// =============================================================================
// (public)
QOakToolButtonRemove::QOakToolButtonRemove(QObject *parent)
    : QOakToolButton(parent)
{

}

// =============================================================================
// (public)
int QOakToolButtonRemove::count() const
{
    if (m_toolBarModel->node().isNull()) { return 0; }
    return 1;
}

// =============================================================================
// (public)
QVariant QOakToolButtonRemove::data(int index, int role) const
{
    Q_UNUSED(index)
    const Oak::Model::NodeDef * def = m_toolBarModel->node().def();

    switch(role)
    {
    case Qt::DisplayRole:
        return QString::fromStdString(def->name());
    case Qt::ToolTipRole: {
        QString toolTip = tr("Remove ") + def->displayName().c_str();
        return toolTip;
    }
    case QOakToolBarModel::ImagePath:
        return "delete_32.png";
    case QOakToolBarModel::Color:
        if (def->hasColor()) {
            Oak::Model::Color color = def->color();
            return toQColor(color);
        }
        break;
    case QOakToolBarModel::Enabled: {
        Oak::Model::Node parent = m_toolBarModel->node().parent();
        int i = parent.childIndex(m_toolBarModel->node());
        return parent.canRemoveChild(i);
    }
    }
    return QVariant();
}

// =============================================================================
// (public)
void QOakToolButtonRemove::sourceModelConnect()
{
    const auto *model = m_toolBarModel->sourceOakModel();
    model->notifier_nodeInserteAfter.add(this, &QOakToolButtonRemove::onNodeInserteAfter);
    model->notifier_nodeRemoveAfter.add(this, &QOakToolButtonRemove::onNodeRemoveAfter);
    model->notifier_nodeMoveAfter.add(this, &QOakToolButtonRemove::onNodeMoveAfter);
    model->notifier_nodeCloneAfter.add(this, &QOakToolButtonRemove::onNodeCloneAfter);
}

// =============================================================================
// (public)
void QOakToolButtonRemove::sourceModelDisconnect()
{
    const auto *model = m_toolBarModel->sourceOakModel();
    model->notifier_nodeInserteAfter.remove(this);
    model->notifier_nodeRemoveAfter.remove(this);
    model->notifier_nodeMoveAfter.remove(this);
    model->notifier_nodeCloneAfter.remove(this);
}

// =============================================================================
// (public)
void QOakToolButtonRemove::trigger(int index)
{
    Q_UNUSED(index)
    Oak::Model::Node parent = m_toolBarModel->node().parent();
    int i = parent.childIndex(m_toolBarModel->node());
    parent.removeChild(i);
}

// =============================================================================
// (public)
void QOakToolButtonRemove::onNodeInserteAfter(const Oak::Model::NodeIndex &nodeIndex)
{
    // Node index not set
    if (!m_toolBarModel || !m_toolBarModel->nodeIndex()) { return; }
    // Do nothing if the change is not related to the child of the node
    if (m_toolBarModel->nodeIndex()->depth()+1 != nodeIndex.depth()) { return; }
    if (!nodeIndex.contains(*m_toolBarModel->nodeIndex())) { return; }

    // Find the button that might have changed
    int index = m_toolBarModel->node().def()->containerIndex(nodeIndex.lastNodeIndex().name());
    if (index == -1) { return; }

    // Emit the signal that the button have changed(it might not have...)
    emit buttonChanged(index, QVector<int>() << QOakToolBarModel::Enabled);
}

// =============================================================================
// (public)
void QOakToolButtonRemove::onNodeRemoveAfter(const Oak::Model::NodeIndex &nodeIndex)
{
    // Node index not set
    if (!m_toolBarModel || !m_toolBarModel->nodeIndex()) { return; }
    // Do nothing if the change is not related to the child of the node
    if (m_toolBarModel->nodeIndex()->depth()+1 != nodeIndex.depth()) { return; }
    if (!nodeIndex.contains(*m_toolBarModel->nodeIndex())) { return; }

    // Find the button that might have changed
    int index = m_toolBarModel->node().def()->containerIndex(nodeIndex.lastNodeIndex().name());
    if (index == -1) { return; }

    // Emit the signal that the button have changed(it might not have...)
    emit buttonChanged(index, QVector<int>() << QOakToolBarModel::Enabled);
}

// =============================================================================
// (public)
void QOakToolButtonRemove::onNodeMoveAfter(const Oak::Model::NodeIndex &sourceNodeIndex, const Oak::Model::NodeIndex &targetNodeIndex)
{
   // Node index not set
   if (!m_toolBarModel || !m_toolBarModel->nodeIndex()) { return; }
   // Ignore move if source and target parents are equal
   if (sourceNodeIndex.depthWhereEqual(targetNodeIndex) + 1 == sourceNodeIndex.depth())
   // Do nothing if the change is not related to the child of the node
   if (m_toolBarModel->nodeIndex()->depth()+1 != targetNodeIndex.depth()) { return; }
   if (!targetNodeIndex.contains(*m_toolBarModel->nodeIndex())) { return; }

   // Find the button that might have changed
   int index = m_toolBarModel->node().def()->containerIndex(targetNodeIndex.lastNodeIndex().name());
   if (index == -1) { return; }

   // Emit the signal that the button have changed(it might not have...)
   emit buttonChanged(index, QVector<int>() << QOakToolBarModel::Enabled);
}

// =============================================================================
// (public)
void QOakToolButtonRemove::onNodeCloneAfter(const Oak::Model::NodeIndex &sourceNodeIndex, const Oak::Model::NodeIndex &targetNodeIndex)
{
    Q_UNUSED(sourceNodeIndex)

    // Node index not set
   if (!m_toolBarModel || !m_toolBarModel->nodeIndex()) { return; }
   // Do nothing if the change is not related to the child of the node
   if (m_toolBarModel->nodeIndex()->depth()+1 != targetNodeIndex.depth()) { return; }
   if (!targetNodeIndex.contains(*m_toolBarModel->nodeIndex())) { return; }

   // Find the button that might have changed
   int index = m_toolBarModel->node().def()->containerIndex(targetNodeIndex.lastNodeIndex().name());
   if (index == -1) { return; }

   // Emit the signal that the button have changed(it might not have...)
   emit buttonChanged(index, QVector<int>() << QOakToolBarModel::Enabled);
}
