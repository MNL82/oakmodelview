/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include <vector>
#include <QModelIndex>

class TreeViewInternalModel;

// =============================================================================
// Class definition
// =============================================================================
class TreeViewNodeData
{
public:
    TreeViewNodeData(TreeViewInternalModel *model);
    TreeViewNodeData(int firstRow, const QModelIndex &modelIndex, TreeViewNodeData *parent = nullptr);
    ~TreeViewNodeData();

    bool expanded() const;
    bool setExpanded(bool value);
    int depth() const;
    int firstRow() const;
    int lastRow() const;
    int rowCountDirect() const;
    int rowCount() const;
    int localRowInParent() const;
    const QModelIndex &treeModelIndex() const;

    void updateFirstRow(int change);
    void updateGlobalRowCount(int globalRow, int change);
    void updateLocalRowData(int globalRow, int change);

    void removeChildNodeData(int first, int last);

    bool containsRow(int globalRow) const;

    bool expanded(int globalRow) const;
    bool setExpanded(const QModelIndex &treeModelIndex, int globalRow, bool value);

    QModelIndex treeModelIndex(int globalRow, int column) const;

    const TreeViewNodeData *parentNodeData(int globalRow) const;
    TreeViewNodeData *parentNodeData(int globalRow);

    TreeViewNodeData *childNodeData(const QModelIndex &index);

    int localToGlobalRow(int row) const;
    std::tuple<int,int> localToGlobalRows(int first, int last);

    void updateTreeModelIndexes(const QModelIndex &newTreeModelIndex);



protected:
    TreeViewNodeData *m_parent = nullptr;
    TreeViewInternalModel * m_model;
    QList<TreeViewNodeData*> m_childNodes;
    bool m_expanded = true;
    int m_depth;
    int m_firstRow;
    int m_rowCountDirect;
    int m_rowCount;

    int m_localRowInParent;
    QModelIndex m_treeModelIndex;
};

