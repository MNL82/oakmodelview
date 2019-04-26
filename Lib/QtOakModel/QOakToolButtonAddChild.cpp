/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QOakToolButtonAddChild.h"

#include "QOakToolbarModel.h"

#include <QIcon>

// =============================================================================
// (public)
QOakToolButtonAddChild::QOakToolButtonAddChild(QObject *parent)
    : QOakToolButton(parent)
{

}

// =============================================================================
// (public)
int QOakToolButtonAddChild::count() const
{
    if (m_toolBarModel->node().isNull()) { return 0; }
    return m_toolBarModel->node().def()->containerCount();
}

// =============================================================================
// (public)
QVariant QOakToolButtonAddChild::data(int index, int role) const
{
    const Oak::Model::NodeDef * cDef = m_toolBarModel->node().def()->childDef(index);

    switch(role)
    {
    case QOakModel::Name:
        return QString::fromStdString(cDef->name());
    case QOakToolBarModel::ToolTip:
            if (!cDef->tooltip().empty()) {
                QString toolTip = tr("Add child node: ") + m_toolBarModel->node().def()->tooltip().c_str();
                return toolTip;
            }
            break;
    case QOakToolBarModel::Icon:
            if (cDef->hasImagePath()) {
                QString imagePath = QString::fromStdString(cDef->imagePath());
                return QIcon(imagePath);
            }
            break;
    case QOakToolBarModel::Color:
            if (cDef->hasColor()) {
                Oak::Model::Color color = cDef->color();
                return toQColor(color);
            }
            break;
    case QOakToolBarModel::Enabled:
        int i = -1;
        return m_toolBarModel->node().canInsertChild(cDef->name(), i);
    }
    return QVariant();
}

// =============================================================================
// (public)
void QOakToolButtonAddChild::sourceModelConnect()
{
    const auto *model = m_toolBarModel->sourceOakModel();
    model->notifier_nodeInserteAfter.add(this, &QOakToolButtonAddChild::onNodeInserteAfter);
    model->notifier_nodeRemoveAfter.add(this, &QOakToolButtonAddChild::onNodeRemoveAfter);
    model->notifier_nodeMoveAfter.add(this, &QOakToolButtonAddChild::onNodeMoveAfter);
    model->notifier_nodeCloneAfter.add(this, &QOakToolButtonAddChild::onNodeCloneAfter);
}

// =============================================================================
// (public)
void QOakToolButtonAddChild::sourceModelDisconnect()
{
    const auto *model = m_toolBarModel->sourceOakModel();
    model->notifier_nodeInserteAfter.remove(this);
    model->notifier_nodeRemoveAfter.remove(this);
    model->notifier_nodeMoveAfter.remove(this);
    model->notifier_nodeCloneAfter.remove(this);
}

// =============================================================================
// (public)
void QOakToolButtonAddChild::trigger(int index)
{
    const std::string &name = m_toolBarModel->node().def()->container(index).containerDef()->name();
    int i = -1;
    m_toolBarModel->node().insertChild(name, i);
}

// =============================================================================
// (public)
void QOakToolButtonAddChild::onNodeInserteAfter(const Oak::Model::NodeIndex &nodeIndex)
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
void QOakToolButtonAddChild::onNodeRemoveAfter(const Oak::Model::NodeIndex &nodeIndex)
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
void QOakToolButtonAddChild::onNodeMoveAfter(const Oak::Model::NodeIndex &sourceNodeIndex, const Oak::Model::NodeIndex &targetNodeIndex)
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
void QOakToolButtonAddChild::onNodeCloneAfter(const Oak::Model::NodeIndex &sourceNodeIndex, const Oak::Model::NodeIndex &targetNodeIndex)
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
