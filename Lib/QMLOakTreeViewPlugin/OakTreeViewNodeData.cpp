/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "OakTreeViewNodeData.h"

#include "OakTreeViewInternalModel.h"

#include "Trace.h"

// =============================================================================
// (public)
OakTreeViewNodeData::OakTreeViewNodeData(OakTreeViewInternalModel *model)
{
    ASSERT(model);
    ASSERT(model->isValid());

    m_model = model;
    m_treeModelIndex = model->rootModelIndex();
    m_localRowInParent = m_treeModelIndex.row();

    m_depth = 0;

    m_firstRow = 0;
    m_rowCountDirect = m_model->treeModel()->rowCount(m_treeModelIndex);
    // No nodes are expanded at initilization
    m_rowCount = m_rowCountDirect;
}

// =============================================================================
// (public)
OakTreeViewNodeData::OakTreeViewNodeData(int firstRow, const QModelIndex &modelIndex, OakTreeViewNodeData *parent)
{
    ASSERT(parent);
    m_parent = parent;

    m_model = parent->m_model;
    m_treeModelIndex = modelIndex;
    m_localRowInParent = m_treeModelIndex.row();

    m_depth = m_parent->m_depth + 1;

    m_firstRow = firstRow;
    m_rowCountDirect = m_model->treeModel()->rowCount(m_treeModelIndex);
    // No nodes are expanded at initilization
    m_rowCount = m_rowCountDirect;
}

// =============================================================================
// (public)
OakTreeViewNodeData::~OakTreeViewNodeData()
{
    for(OakTreeViewNodeData *cNode: m_childNodes)
    {
        delete cNode;
    }
    m_childNodes.clear();
}

// =============================================================================
// (public)
bool OakTreeViewNodeData::expanded() const
{
    return m_expanded;
}

// =============================================================================
// (public)
bool OakTreeViewNodeData::setExpanded(bool value)
{
    if (m_expanded == value) { return false; }
    m_expanded = value;
    return true;
}

// =============================================================================
// (public)
int OakTreeViewNodeData::depth() const
{
    return m_depth;
}

// =============================================================================
// (public)
int OakTreeViewNodeData::firstRow() const
{
    return m_firstRow;
}

// =============================================================================
// (public)
int OakTreeViewNodeData::lastRow() const
{
    return m_firstRow + m_rowCount - 1;
}

// =============================================================================
// (public)
int OakTreeViewNodeData::rowCountDirect() const
{
    return m_rowCountDirect;
}

// =============================================================================
// (public)
int OakTreeViewNodeData::rowCount() const
{
    return m_rowCount;
}

// =============================================================================
// (public)
int OakTreeViewNodeData::localRowInParent() const
{
    return m_localRowInParent;
}

// =============================================================================
// (public)
const QModelIndex &OakTreeViewNodeData::treeModelIndex() const
{
    return m_treeModelIndex;
}

// =============================================================================
// (public)
void OakTreeViewNodeData::updateFirstRow(int change)
{
    m_firstRow += change;
    for (OakTreeViewNodeData *cNode: m_childNodes)
    {
        cNode->updateFirstRow(change);
    }
}

// =============================================================================
// (public)
void OakTreeViewNodeData::updateGlobalRowCount(int globalRow, int change)
{
    for (OakTreeViewNodeData *cNode: m_childNodes)
    {
        if (cNode->firstRow() > globalRow) {
            cNode->updateFirstRow(change);
        }
    }
    m_rowCount += change;
    if (m_parent) {
        m_parent->updateGlobalRowCount(globalRow, change);
    }
}

// =============================================================================
// (public)
void OakTreeViewNodeData::updateLocalRowData(int globalRow, int change)
{
    m_rowCountDirect += change;
    for (OakTreeViewNodeData *cNode: m_childNodes)
    {
        if (cNode->firstRow() > globalRow + 1) {
            cNode->m_localRowInParent += change;;
        }
    }
}

// =============================================================================
// (public)
void OakTreeViewNodeData::removeChildNodeData(int first, int last)
{
    for (int i = 0; i < m_childNodes.size(); i++)
    {
        OakTreeViewNodeData *childNode = m_childNodes[i];
        if (childNode->localRowInParent() >= first &&
            childNode->localRowInParent() <= last) {
            m_childNodes.removeAt(i);
            i--;
            delete childNode;
        }
    }
}

// =============================================================================
// (public)
bool OakTreeViewNodeData::containsRow(int globalRow) const
{
    return globalRow >= m_firstRow && globalRow < (m_firstRow + m_rowCount);
}

// =============================================================================
// (public)
bool OakTreeViewNodeData::expanded(int globalRow) const
{
    int row = globalRow - m_firstRow;
    ASSERT(row >= 0);
    ASSERT(row < m_rowCount);
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        if (cNode->treeModelIndex().row() == row) {
            return cNode->expanded();
        }

        // Ignore collapesed nodes
        if (!cNode->expanded()) { continue; }

        if (globalRow < cNode->firstRow()) {
            // Passed the globalRow (global row is a direct child of this node)
            break;
        }
        if (globalRow <= cNode->lastRow()) {
            ASSERT(false);
            return expanded(globalRow);
        }
        row -= cNode->rowCount();
    }
    ASSERT(row < m_rowCountDirect);
    return false;
}

// =============================================================================
// (public)
bool OakTreeViewNodeData::setExpanded(const QModelIndex &treeModelIndex, int globalRow, bool value)
{
    int row = globalRow - m_firstRow;
    ASSERT(row >= 0);
    ASSERT(row < m_rowCount);
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        if (cNode->treeModelIndex() == treeModelIndex) {
            // Data Node already exist (state changed)
            if (cNode->expanded() == value) { return false; }
            if (value) {
                m_model->beginInsertRows(QModelIndex(), globalRow+1, globalRow + cNode->rowCount());
                ASSERT(cNode->setExpanded(value));
                updateGlobalRowCount(globalRow+1, cNode->rowCount());
                m_model->endInsertRows();
                //TRACE("Row count after expanding node: %i\n", m_model->rowCount(QModelIndex()));
            } else {
                m_model->beginRemoveRows(QModelIndex(), globalRow+1, globalRow + cNode->rowCount());
                ASSERT(cNode->setExpanded(value));
                updateGlobalRowCount(globalRow+1, -cNode->rowCount());
                m_model->endRemoveRows();
                //TRACE("Row count after collapsing node: %i\n", m_model->rowCount(QModelIndex()));
            }
            return true;
        }

        // Ignore collapesed nodes
        if (!cNode->expanded()) { continue; }

        if (globalRow < cNode->firstRow()) {
            // Passed the globalRow (global row is a direct child of this node)
            break;
        }
        if (globalRow <= cNode->lastRow()) {
            ASSERT(false);
            return setExpanded(treeModelIndex, globalRow, value);
        }
        row -= cNode->rowCount();
    }
    ASSERT(row < m_rowCountDirect); //
    ASSERT(value);
    ASSERT(m_model->treeModel()->hasChildren(treeModelIndex));

    // Create a new TreeViewNodeData for the first time expanded node
    OakTreeViewNodeData *cNode = new OakTreeViewNodeData(globalRow+1, treeModelIndex, this);
    auto it = m_childNodes.begin();
    while (it != m_childNodes.end()) {
        if ((*it)->firstRow() > cNode->firstRow()) {
            break;
        }
        it++;
    }

    m_model->beginInsertRows(QModelIndex(), globalRow+1, globalRow + cNode->rowCount());
    m_childNodes.insert(it, cNode);
    updateGlobalRowCount(globalRow+1, cNode->rowCount());
    m_model->endInsertRows();
    //TRACE("Row count after expanding(2) node: %i\n", m_model->rowCount(QModelIndex()));

    return true;
}

// =============================================================================
// (public)
QModelIndex OakTreeViewNodeData::treeModelIndex(int globalRow, int column) const
{
    int row = globalRow - m_firstRow;
    ASSERT(row >= 0);
    ASSERT(row < m_rowCount);
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        // Ignore collapesed nodes
        if (!cNode->expanded()) { continue; }

        if (globalRow < cNode->firstRow()) {
            // Passed the globalRow (global row is a direct child of this node)
            break;
        }
        if (globalRow <= cNode->lastRow()) {
            ASSERT(false);
            return treeModelIndex(globalRow, column);
        }
        row -= cNode->rowCount();
    }
    ASSERT(row < m_rowCountDirect);
    return m_model->treeModel()->index(row, column, m_treeModelIndex);
}

// =============================================================================
// (public)
const OakTreeViewNodeData *OakTreeViewNodeData::parentNodeData(int globalRow) const
{
    int row = globalRow - m_firstRow;
    ASSERT(row >= 0);
    ASSERT(row < m_rowCount);
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        // Ignore collapesed nodes
        if (!cNode->expanded()) { continue; }

        if (globalRow < cNode->firstRow()) {
            // Passed the globalRow (global row is a direct child of this node)
            break;
        }
        if (globalRow <= cNode->lastRow()) {
            return parentNodeData(globalRow);
        }
        row -= cNode->rowCount();
    }
    ASSERT(row < m_rowCountDirect);
    return this;
}

// =============================================================================
// (public)
OakTreeViewNodeData *OakTreeViewNodeData::parentNodeData(int globalRow)
{
    int row = globalRow - m_firstRow;
    ASSERT(row >= 0);
    ASSERT(row < m_rowCount);
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        // Ignore collapesed nodes
        if (!cNode->expanded()) { continue; }

        if (globalRow < cNode->firstRow()) {
            // Passed the globalRow (global row is a direct child of this node)
            break;
        }
        if (globalRow <= cNode->lastRow()) {
            return cNode->parentNodeData(globalRow);
        }
        row -= cNode->rowCount();
    }
    ASSERT(row < m_rowCountDirect);
    return this;
}

// =============================================================================
// (public)
OakTreeViewNodeData *OakTreeViewNodeData::childNodeData(const QModelIndex &index)
{
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        if (cNode->treeModelIndex() == index) {
            return cNode;
        }
    }
    return nullptr;
}

// =============================================================================
// (public)
int OakTreeViewNodeData::localToGlobalRow(int row) const
{
    int globalRow = m_firstRow + row;
    ASSERT(row >= 0);
    ASSERT(row < m_rowCount+1);
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        // Ignore collapesed nodes
        if (!cNode->expanded()) { continue; }

        if (cNode->firstRow() <= globalRow) {
            globalRow += cNode->rowCount();
        } else {
            break;
        }
    }
    return globalRow;
}

// =============================================================================
// (public)
std::tuple<int,int> OakTreeViewNodeData::localToGlobalRows(int first, int last)
{
    ASSERT(last >= first);
    int gFirst = m_firstRow + first;
    int gLast = m_firstRow + last;
    ASSERT(first >= 0);
    ASSERT(first < m_rowCount+1);
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        // Ignore collapesed nodes
        if (!cNode->expanded()) { continue; }

        if (cNode->localRowInParent() < first) {
            gFirst += cNode->rowCount();
        }
        if (cNode->localRowInParent() <= last) {
            gLast += cNode->rowCount();
        } else {
            break;
        }
    }
    return std::make_tuple(gFirst, gLast);
}

// =============================================================================
// (public)
void OakTreeViewNodeData::updateTreeModelIndexes(const QModelIndex &newTreeModelIndex)
{
    if (newTreeModelIndex != m_treeModelIndex) {
        TRACE("TreeModelIndex changed!\n");
    }
    m_treeModelIndex = newTreeModelIndex;
    //TRACE("Node key value: %s\n", m_treeModelIndex.data(QOakModel::KeyValue).toString().toStdString().c_str());
    for(OakTreeViewNodeData *cNode: m_childNodes) {
        QModelIndex i = m_model->treeModel()->index(cNode->localRowInParent(), 0, newTreeModelIndex);
        if (!i.isValid()) {
            i = m_model->treeModel()->index(cNode->localRowInParent(), 0, newTreeModelIndex);
            return;
        }
        cNode->updateTreeModelIndexes(i);
    }
}
