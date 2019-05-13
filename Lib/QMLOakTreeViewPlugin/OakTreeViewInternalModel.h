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

#include <QAbstractItemModel>

#include "OakTreeViewNodeData.h"

// =============================================================================
// Class definition
// =============================================================================
class OakTreeViewInternalModel : public QAbstractItemModel
{
    Q_OBJECT

    Q_PROPERTY(QModelIndex rootModelIndex READ rootModelIndex WRITE setRootModelIndex NOTIFY rootModelIndexChanged)
    Q_PROPERTY(QAbstractItemModel *treeModel READ treeModel WRITE setTreeModel NOTIFY treeModelChanged)


    Q_PROPERTY(int currentGlobalRow READ currentGlobalRow WRITE setCurrentGlobalRow NOTIFY currentGlobalRowChanged)
    Q_PROPERTY(QModelIndex currentTreeModelIndex MEMBER m_currentTreeModelIndex NOTIFY currentTreeModelIndexChanged)

public:
    enum Roles  {
        HasChildrenRole = Qt::UserRole + 300,
        ExpandedRole = Qt::UserRole + 301,
        DepthRole = Qt::UserRole + 302,
        ModelIndexRole = Qt::UserRole + 303
    };

    OakTreeViewInternalModel(QObject *parent = nullptr);

    bool isValid() const;

    const QModelIndex &rootModelIndex() const;
    QAbstractItemModel * treeModel() const;

    int currentGlobalRow() const;

    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    virtual QModelIndex parent(const QModelIndex& index) const override;
    virtual QModelIndex sibling(int row, int column, const QModelIndex& idx) const override;
    virtual int rowCount(const QModelIndex& parent) const override;
    virtual int columnCount(const QModelIndex& parent) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;
//    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
//    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role) override;
//    virtual QMap<int, QVariant> itemData(const QModelIndex& index) const override;
//    virtual bool setItemData(const QModelIndex& index, const QMap<int, QVariant>& roles) override;
//    virtual QStringList mimeTypes() const override;
//    virtual QMimeData*mimeData(const QModelIndexList& indexes) const override;
//    virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
//    virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
//    virtual Qt::DropActions supportedDropActions() const override;
//    virtual Qt::DropActions supportedDragActions() const override;
//    virtual bool insertRows(int row, int count, const QModelIndex& parent) override;
//    virtual bool insertColumns(int column, int count, const QModelIndex& parent) override;
//    virtual bool removeRows(int row, int count, const QModelIndex& parent) override;
//    virtual bool removeColumns(int column, int count, const QModelIndex& parent) override;
//    virtual bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;
//    virtual bool moveColumns(const QModelIndex& sourceParent, int sourceColumn, int count, const QModelIndex& destinationParent, int destinationChild) override;
//    virtual void fetchMore(const QModelIndex& parent) override;
//    virtual bool canFetchMore(const QModelIndex& parent) const override;
//    virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
//    virtual void sort(int column, Qt::SortOrder order) override;
//    virtual QModelIndex buddy(const QModelIndex& index) const override;
//    virtual QModelIndexList match(const QModelIndex& start, int role, const QVariant& value, int hits, Qt::MatchFlags flags) const override;
//    virtual QSize span(const QModelIndex& index) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

public slots:
    void setRootModelIndex(const QModelIndex &rootModelIndex);
    void setTreeModel(QAbstractItemModel * treeModel);

    void setCurrentGlobalRow(int currentGlobalRow);
    void updateCurrentGlobalRow(int currentGlobalRow);

protected:
    void modelConnect();
    void modelDisconnect();

    void resetInternateNodeData();
    void updateTreeModelIndexes();

    OakTreeViewNodeData * findNodeData(const QModelIndex &treeModelIndex) const;

protected slots:
    void onModelAboutToBeReset();
    void onModelReset();

    void onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end);
//    void onRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onRowsInserted(const QModelIndex &parent, int first, int last);
//    void onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);

    void onTreeModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

signals:
    void rootModelIndexChanged(const QModelIndex &rootModelIndex);
    void treeModelChanged(QAbstractItemModel * treeModel);

    void currentTreeModelIndexChanged(QModelIndex currentTreeModelIndex);

    void currentGlobalRowChanged(int currentGlobalRow);
    void currentGlobalRowForceUpdate(int currentGlobalRow);

protected:
    QAbstractItemModel * m_treeModel = nullptr;
    QModelIndex m_rootModelIndex;
    OakTreeViewNodeData * m_rootNodeData = nullptr;

    int m_currentGlobalRow;
    QModelIndex m_currentTreeModelIndex;

    friend class OakTreeViewNodeData;
};

