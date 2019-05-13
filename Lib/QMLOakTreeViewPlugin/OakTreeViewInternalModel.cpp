/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "OakTreeViewInternalModel.h"

#include "Trace.h"

#include <QTimer>

// =============================================================================
// (public)
OakTreeViewInternalModel::OakTreeViewInternalModel(QObject *parent)
    : QAbstractItemModel(parent)
{

}

// =============================================================================
// (public)
bool OakTreeViewInternalModel::isValid() const
{
    if (m_treeModel == nullptr) { return false; }
    if (m_rootModelIndex.isValid() && m_rootModelIndex.model() != m_treeModel) { return false; }
    return true;
}

// =============================================================================
// (public)
const QModelIndex &OakTreeViewInternalModel::rootModelIndex() const
{
    return m_rootModelIndex;
}

// =============================================================================
// (public)
QAbstractItemModel *OakTreeViewInternalModel::treeModel() const
{
    return m_treeModel;
}

// =============================================================================
// (public)
int OakTreeViewInternalModel::currentGlobalRow() const
{
    return m_currentGlobalRow;
}

// =============================================================================
// (public)
QModelIndex OakTreeViewInternalModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (!isValid()) { return QModelIndex(); }
    auto * pNodeData = m_rootNodeData->parentNodeData(row);
    return createIndex(row, column, pNodeData);
}

// =============================================================================
// (public)
QModelIndex OakTreeViewInternalModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}

// =============================================================================
// (public)
QModelIndex OakTreeViewInternalModel::sibling(int row, int column, const QModelIndex &idx) const
{
    if (!isValid()) { return QModelIndex(); }
    OakTreeViewNodeData *nodeData = static_cast<OakTreeViewNodeData*>(idx.internalPointer());
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
int OakTreeViewInternalModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int count = isValid() ? m_rootNodeData->rowCount() : 0;
    //TRACE("rowCount: %i\n", count);
    return count;
}

// =============================================================================
// (public)
int OakTreeViewInternalModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return isValid() ? m_treeModel->columnCount(m_rootModelIndex) : 0;
}

// =============================================================================
// (public)
QVariant OakTreeViewInternalModel::data(const QModelIndex &index, int role) const
{
    if (!isValid()) { return QVariant(); }
    ASSERT(index.isValid());

    const OakTreeViewNodeData *nodeData = static_cast<const OakTreeViewNodeData*>(index.internalPointer());
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
bool OakTreeViewInternalModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!isValid()) { return false; }
    ASSERT(index.isValid());

    OakTreeViewNodeData *nodeData = static_cast<OakTreeViewNodeData*>(index.internalPointer());
    ASSERT(nodeData);

    switch (role) {
    case HasChildrenRole:
        return false;
    case ExpandedRole: {
        bool expanded = value.toBool();
        QModelIndex treeModelIndex = nodeData->treeModelIndex(index.row(), index.column());
        if (nodeData->setExpanded(treeModelIndex, index.row(), expanded)) {
            dataChanged(index, index, QVector<int>() << ExpandedRole);
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
QHash<int, QByteArray> OakTreeViewInternalModel::roleNames() const
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
void OakTreeViewInternalModel::setRootModelIndex(const QModelIndex &rootModelIndex)
{
    if (m_rootModelIndex == rootModelIndex) { return; }

    m_rootModelIndex = rootModelIndex;
    resetInternateNodeData();

    emit rootModelIndexChanged(m_rootModelIndex);
}

// =============================================================================
// (public slots)
void OakTreeViewInternalModel::setTreeModel(QAbstractItemModel *treeModel)
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
void OakTreeViewInternalModel::setCurrentGlobalRow(int currentGlobalRow)
{
    if (m_currentGlobalRow != currentGlobalRow) {
        m_currentGlobalRow = currentGlobalRow;
        emit currentGlobalRowChanged(m_currentGlobalRow);
    }

    if (isValid() && currentGlobalRow >= 0) {
        OakTreeViewNodeData *nodeData = m_rootNodeData->parentNodeData(currentGlobalRow);

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
void OakTreeViewInternalModel::updateCurrentGlobalRow(int currentGlobalRow)
{
    if (m_currentGlobalRow != currentGlobalRow) {
        m_currentGlobalRow = currentGlobalRow;
        emit currentGlobalRowChanged(m_currentGlobalRow);
    }

    if (isValid() && currentGlobalRow >= 0) {
        OakTreeViewNodeData *nodeData = m_rootNodeData->parentNodeData(currentGlobalRow);

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
void OakTreeViewInternalModel::modelConnect()
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
void OakTreeViewInternalModel::modelDisconnect()
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
void OakTreeViewInternalModel::resetInternateNodeData()
{
    if (m_rootNodeData) { delete m_rootNodeData; }
    if (isValid()) {
        m_rootNodeData = new OakTreeViewNodeData(this);
    }
}

// =============================================================================
// (protected)
void OakTreeViewInternalModel::updateTreeModelIndexes()
{
    // TODO: Make sure the rootModelIndex can not get out of skope too?
    m_rootNodeData->updateTreeModelIndexes(m_rootModelIndex);
}

// =============================================================================
// (protected)
OakTreeViewNodeData *OakTreeViewInternalModel::findNodeData(const QModelIndex &treeModelIndex) const
{
    QList<QModelIndex> parentList;
    QModelIndex cIndex = treeModelIndex;
    while (cIndex != m_rootModelIndex) {
        if (!cIndex.isValid()) { return nullptr; }
        parentList.push_back(cIndex);
        cIndex = cIndex.parent();
    }
    OakTreeViewNodeData *cNodeData = m_rootNodeData;
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
void OakTreeViewInternalModel::onModelAboutToBeReset()
{
    beginResetModel();
}

// =============================================================================
// (protected slots)
void OakTreeViewInternalModel::onModelReset()
{
    resetInternalData();
    resetInternateNodeData();
    endResetModel();
}

// =============================================================================
// (protected slots)
void OakTreeViewInternalModel::onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    if (!isValid()) { return; }
    OakTreeViewNodeData *nodeData = findNodeData(parent);
    if (!nodeData) { return; }
    int globalRow = nodeData->localToGlobalRow(start);

    beginInsertRows(QModelIndex(), globalRow, globalRow + end - start);
}

// =============================================================================
// (protected slots)
void OakTreeViewInternalModel::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    if (!isValid()) { return; }
    OakTreeViewNodeData *nodeData = findNodeData(parent);
    if (!nodeData) { return; }

    int gFirst, gLast;
    std::tie(gFirst, gLast) = nodeData->localToGlobalRows(first, last);
    //int globalRow = nodeData->localToGlobalRow(first);

    beginRemoveRows(QModelIndex(), gFirst, gLast);
}

// =============================================================================
// (protected slots)
void OakTreeViewInternalModel::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    if (!isValid()) { return; }
    OakTreeViewNodeData *nodeData = findNodeData(parent);
    if (!nodeData) {
        // Check if hasChildren changed for parent
        if (m_treeModel->rowCount(parent) == last - first + 1) {
            nodeData = findNodeData(parent.parent());
            if (nodeData) {
                QModelIndex i = index(nodeData->localToGlobalRow(parent.row()), 0, QModelIndex());
                dataChanged(i, i, QVector<int>() << HasChildrenRole);
            }
        }
        return;
    }
    int globalRow = nodeData->localToGlobalRow(first);

    int insertCount = last - first + 1;

    nodeData->updateLocalRowData(globalRow, insertCount);
    nodeData->updateGlobalRowCount(globalRow, insertCount);
    updateTreeModelIndexes();

    endInsertRows();
    updateCurrentGlobalRow(m_currentGlobalRow);
}

// =============================================================================
// (protected slots)
void OakTreeViewInternalModel::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    if (!isValid()) { return; }

    // Check if hasChildren changed for parent
    if (!m_treeModel->hasChildren(parent)) {
        OakTreeViewNodeData *pNodeData = findNodeData(parent.parent());
        if (pNodeData) {
            QModelIndex i = index(pNodeData->localToGlobalRow(parent.row()), 0, QModelIndex());
            dataChanged(i, i, QVector<int>() << HasChildrenRole);
        }
    }

    OakTreeViewNodeData *nodeData = findNodeData(parent);
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

    if (!m_treeModel->hasChildren(parent)) {
        OakTreeViewNodeData *pNodeData = findNodeData(parent.parent());
        if (pNodeData) {
            pNodeData->removeChildNodeData(nodeData->localRowInParent(), nodeData->localRowInParent());
        }
    }
}
