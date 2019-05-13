/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "TreeViewInternalModel.h"

#include "../ServiceFunctions/Trace.h"

#include <QTimer>

// =============================================================================
// (public)
TreeViewInternalModel::TreeViewInternalModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

// =============================================================================
// (public)
bool TreeViewInternalModel::isValid() const
{
    if (m_treeModel == nullptr) { return false; }
    if (m_rootModelIndex.isValid() && m_rootModelIndex.model() != m_treeModel) { return false; }
    return true;
}

// =============================================================================
// (public)
const QModelIndex &TreeViewInternalModel::rootModelIndex() const
{
    return m_rootModelIndex;
}

// =============================================================================
// (public)
QAbstractItemModel *TreeViewInternalModel::treeModel() const
{
    return m_treeModel;
}

// =============================================================================
// (public)
int TreeViewInternalModel::currentGlobalRow() const
{
    return m_currentGlobalRow;
}

// =============================================================================
// (public)
QModelIndex TreeViewInternalModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (!isValid()) { return QModelIndex(); }
    auto * pNodeData = m_rootNodeData->parentNodeData(row);
    return createIndex(row, column, pNodeData);
}

// =============================================================================
// (public)
QModelIndex TreeViewInternalModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

// =============================================================================
// (public)
QModelIndex TreeViewInternalModel::sibling(int row, int column, const QModelIndex &idx) const
{
    if (!isValid()) { return QModelIndex(); }
    TreeViewNodeData *nodeData = static_cast<TreeViewNodeData*>(idx.internalPointer());
    ASSERT(nodeData);
    if (nodeData->containsRow(row)) {
        auto * pNodeData = nodeData->parentNodeData(row);
        return createIndex(row, column, pNodeData);
    } else {
        auto * pNodeData = m_rootNodeData->parentNodeData(row);
        return createIndex(row, column, pNodeData);
    }
}

// =============================================================================
// (public)
int TreeViewInternalModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int count = isValid() ? m_rootNodeData->rowCount() : 0;
    //TRACE("rowCount: %i\n", count);
    return count;
}

// =============================================================================
// (public)
int TreeViewInternalModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return isValid() ? m_treeModel->columnCount(m_rootModelIndex) : 0;
}

// =============================================================================
// (public)
QVariant TreeViewInternalModel::data(const QModelIndex &index, int role) const
{
    if (!isValid()) { return QVariant(); }
    ASSERT(index.isValid());

    const TreeViewNodeData *nodeData = static_cast<const TreeViewNodeData*>(index.internalPointer());
    ASSERT(nodeData);

    switch (role) {
    case HasChildrenRole: {
        QModelIndex treeModelIndex = nodeData->treeModelIndex(index.row(), index.column());
        return m_treeModel->hasChildren(treeModelIndex);
    }
    case ExpandedRole:
        return nodeData->expanded(index.row());
    case DepthRole:
        return nodeData->depth();
    case ModelIndexRole:
        return nodeData->treeModelIndex(index.row(), index.column());
    }

    QModelIndex treeModelIndex = nodeData->treeModelIndex(index.row(), index.column());
    return m_treeModel->data(treeModelIndex, role);
}

// =============================================================================
// (public)
bool TreeViewInternalModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!isValid()) { return false; }
    ASSERT(index.isValid());

    TreeViewNodeData *nodeData = static_cast<TreeViewNodeData*>(index.internalPointer());
    ASSERT(nodeData);

    switch (role) {
    case HasChildrenRole:
        return false;
    case ExpandedRole: {
        bool expanded = value.toBool();
        QModelIndex treeModelIndex = nodeData->treeModelIndex(index.row(), index.column());
        if (nodeData->setExpanded(treeModelIndex, index.row(), expanded)) {
            dataChanged(index, index, QVector<int>() <<  ExpandedRole);
        }
        return true;
    }
    case DepthRole:
        return false;
    case ModelIndexRole:
        return false;
    }

    QModelIndex treeModelIndex = nodeData->treeModelIndex(index.row(), index.column());
    return m_treeModel->setData(treeModelIndex, role);
}

// =============================================================================
// (public)
QHash<int, QByteArray> TreeViewInternalModel::roleNames() const
{
    if (!isValid()) { return QHash<int, QByteArray>(); }
    QHash<int, QByteArray> result = treeModel()->roleNames();
    result.insert(HasChildrenRole, QByteArrayLiteral("hasChildren"));
    result.insert(ExpandedRole, QByteArrayLiteral("expanded"));
    result.insert(DepthRole, QByteArrayLiteral("depth"));
    result.insert(ModelIndexRole, QByteArrayLiteral("modelIndex"));
    return result;
}

// =============================================================================
// (public slots)
void TreeViewInternalModel::setRootModelIndex(const QModelIndex &rootModelIndex)
{
    if (m_rootModelIndex == rootModelIndex) { return; }

    m_rootModelIndex = rootModelIndex;
    resetInternateNodeData();

    emit rootModelIndexChanged(m_rootModelIndex);
}

// =============================================================================
// (public slots)
void TreeViewInternalModel::setTreeModel(QAbstractItemModel *treeModel)
{
    if (m_treeModel == treeModel) { return; }

    modelDisconnect();

    m_treeModel = treeModel;
    resetInternateNodeData();

    modelConnect();

    emit treeModelChanged(m_treeModel);
}

// =============================================================================
// (public slots)
void TreeViewInternalModel::setCurrentGlobalRow(int currentGlobalRow)
{
    if (m_currentGlobalRow != currentGlobalRow) {
        m_currentGlobalRow = currentGlobalRow;
        emit currentGlobalRowChanged(m_currentGlobalRow);
    }

    if (isValid() && currentGlobalRow >= 0) {
        TreeViewNodeData *nodeData = m_rootNodeData->parentNodeData(currentGlobalRow);

        ASSERT(nodeData);
        QModelIndex treeModelIndex = nodeData->treeModelIndex(currentGlobalRow, 0);
        if (m_currentTreeModelIndex != treeModelIndex) {
            m_currentTreeModelIndex = treeModelIndex;
            emit currentTreeModelIndexChanged(m_currentTreeModelIndex);
        }
    } else if (m_currentTreeModelIndex.isValid()) {
        m_currentTreeModelIndex = QModelIndex();
        emit currentTreeModelIndexChanged(m_currentTreeModelIndex);
    }
}

// =============================================================================
// (public slots)
void TreeViewInternalModel::updateCurrentGlobalRow(int currentGlobalRow)
{
    if (m_currentGlobalRow != currentGlobalRow) {
        m_currentGlobalRow = currentGlobalRow;
        emit currentGlobalRowChanged(m_currentGlobalRow);
    }

    if (isValid() && currentGlobalRow >= 0) {
        TreeViewNodeData *nodeData = m_rootNodeData->parentNodeData(currentGlobalRow);

        ASSERT(nodeData);
        QModelIndex treeModelIndex = nodeData->treeModelIndex(currentGlobalRow, 0);
        m_currentTreeModelIndex = treeModelIndex;
        emit currentTreeModelIndexChanged(m_currentTreeModelIndex);
    } else {
        m_currentTreeModelIndex = QModelIndex();
        emit currentTreeModelIndexChanged(m_currentTreeModelIndex);
    }

    emit currentGlobalRowForceUpdate(currentGlobalRow);
}

// =============================================================================
// (protected)
void TreeViewInternalModel::modelConnect()
{
    if (m_treeModel == nullptr) { return; }
    connect(m_treeModel, SIGNAL(modelAboutToBeReset()), this, SLOT(onModelAboutToBeReset()));
    connect(m_treeModel, SIGNAL(modelReset()), this, SLOT(onModelReset()));

    connect(m_treeModel, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)), this, SLOT(onRowsAboutToBeInserted(const QModelIndex &, int, int)));
    connect(m_treeModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(onRowsInserted(const QModelIndex &, int, int)));

    connect(m_treeModel, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)), this, SLOT(onRowsAboutToBeRemoved(const QModelIndex &, int, int)));
    connect(m_treeModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(onRowsRemoved(const QModelIndex &, int, int)));
}

// =============================================================================
// (protected)
void TreeViewInternalModel::modelDisconnect()
{
    if (m_treeModel == nullptr) { return; }
    disconnect(m_treeModel, SIGNAL(modelAboutToBeReset()), this, SLOT(onModelAboutToBeReset()));
    disconnect(m_treeModel, SIGNAL(modelReset()), this, SLOT(onModelReset()));

    disconnect(m_treeModel, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)), this, SLOT(onRowsAboutToBeInserted(const QModelIndex &, int, int)));
    disconnect(m_treeModel, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(onRowsInserted(const QModelIndex &, int, int)));

    disconnect(m_treeModel, SIGNAL(rowsAboutToBeRemoved(const QModelIndex &, int, int)), this, SLOT(onRowsAboutToBeRemoved(const QModelIndex &, int, int)));
    disconnect(m_treeModel, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(onRowsRemoved(const QModelIndex &, int, int)));
}

// =============================================================================
// (protected)
void TreeViewInternalModel::resetInternateNodeData()
{
    if (m_rootNodeData) { delete m_rootNodeData; }
    if (isValid()) {
        m_rootNodeData = new TreeViewNodeData(this);
    }
}

// =============================================================================
// (protected)
void TreeViewInternalModel::updateTreeModelIndexes()
{
    // TODO: Make sure the rootModelIndex can not get out of skope too?
    m_rootNodeData->updateTreeModelIndexes(m_rootModelIndex);
}

// =============================================================================
// (protected)
TreeViewNodeData *TreeViewInternalModel::findNodeData(const QModelIndex &treeModelIndex) const
{
    QList<QModelIndex> parentList;
    QModelIndex cIndex = treeModelIndex;
    while (cIndex != m_rootModelIndex) {
        if (!cIndex.isValid()) { return nullptr; }
        parentList.push_back(cIndex);
        cIndex = cIndex.parent();
    }
    TreeViewNodeData *cNodeData = m_rootNodeData;
    auto it = parentList.rbegin();
    while (it != parentList.rend()) {
        cNodeData = cNodeData->childNodeData(*it);
        if (!cNodeData) { return nullptr; }
        it++;
    }
    return cNodeData;
}

// =============================================================================
// (protected slots)
void TreeViewInternalModel::onModelAboutToBeReset()
{
    beginResetModel();
}

// =============================================================================
// (protected slots)
void TreeViewInternalModel::onModelReset()
{
    resetInternalData();
    resetInternateNodeData();
    endResetModel();
}

// =============================================================================
// (protected slots)
void TreeViewInternalModel::onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    if (!isValid()) { return; }
    TreeViewNodeData *nodeData = findNodeData(parent);
    if (!nodeData) { return; }
    int globalRow = nodeData->localToGlobalRow(start);

    beginInsertRows(QModelIndex(), globalRow, globalRow + end - start);
}

// =============================================================================
// (protected slots)
void TreeViewInternalModel::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    if (!isValid()) { return; }
    TreeViewNodeData *nodeData = findNodeData(parent);
    if (!nodeData) { return; }

    int gFirst, gLast;
    std::tie(gFirst, gLast) = nodeData->localToGlobalRows(first, last);
    //int globalRow = nodeData->localToGlobalRow(first);

    beginRemoveRows(QModelIndex(), gFirst, gLast);
}

// =============================================================================
// (protected slots)
void TreeViewInternalModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    if (!isValid()) { return; }
    TreeViewNodeData *nodeData = findNodeData(parent);
    if (!nodeData) { return; }
    int globalRow = nodeData->localToGlobalRow(first);

    nodeData->updateLocalRowData(globalRow, last - first + 1);
    nodeData->updateGlobalRowCount(globalRow, last - first + 1);
    updateTreeModelIndexes();

    endInsertRows();
    updateCurrentGlobalRow(m_currentGlobalRow);
}

// =============================================================================
// (protected slots)
void TreeViewInternalModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    if (!isValid()) { return; }
    TreeViewNodeData *nodeData = findNodeData(parent);
    if (!nodeData) { return; }

    int gFirst, gLast;
    std::tie(gFirst, gLast) = nodeData->localToGlobalRows(first, last);

    int nextCurrentRow = m_currentGlobalRow;
    if (nextCurrentRow >= gFirst && nextCurrentRow <= gLast) {
        // Current row is being deleted
        if (nodeData->rowCountDirect() > last + 1) {
            // Select next sibling
            nextCurrentRow = gFirst;
        } else if(first > 0) {
            // Select previous sibling
            nextCurrentRow = nodeData->localToGlobalRow(first-1);
        } else {
            // Select parent (-1 is no selection and it is fine)
            nextCurrentRow = nodeData->firstRow() - 1;
        }
    }

    nodeData->removeChildNodeData(first, last);
    nodeData->updateLocalRowData(gFirst, -(last - first + 1));
    nodeData->updateGlobalRowCount(gFirst, -(gLast - gFirst + 1));
    updateTreeModelIndexes();

    endRemoveRows();
    updateCurrentGlobalRow(nextCurrentRow);
}
