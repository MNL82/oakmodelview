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

#include "QOakAbstractNodeModel.h"

class TreeViewData;

// =============================================================================
// Class definition
// =============================================================================
class TreeViewListModel : public QAbstractItemModel
{
    Q_OBJECT

    // Output
    Q_PROPERTY(int delegateHeight READ delegateHeight WRITE setDelegateHeight NOTIFY delegateHeightChanged)
    Q_PROPERTY(bool shown READ shown WRITE setShown NOTIFY shownChanged)

    // Indput
    Q_PROPERTY(QModelIndex parentModelIndex READ parentModelIndex WRITE setParentModelIndex NOTIFY parentModelIndexChanged)
    Q_PROPERTY(TreeViewData * treeViewData READ treeViewData WRITE setTreeViewData NOTIFY treeViewDataChanged)

public:
    enum Roles  {
        HasChildren = Qt::UserRole + 300,
        Expanded = Qt::UserRole + 301,
        ViewHeight = Qt::UserRole + 302,
        OakModel = Qt::UserRole + 303,
        IsCurrent = Qt::UserRole + 304,
        RefModelIndex = Qt::UserRole + 305
    };
    Q_ENUM(Roles)

    TreeViewListModel(QObject *parent = nullptr);
    virtual ~TreeViewListModel() override;

    bool isNull() const;

    bool shown() const;
    int delegateHeight() const;
    QModelIndex parentModelIndex() const;
    TreeViewData * treeViewData() const;


public slots:
    void setShown(bool shown);
    void setDelegateHeight(int delegateHeight);
    void setParentModelIndex(const QModelIndex &parentModelIndex);
    void setTreeViewData(TreeViewData * treeViewData);

    void addChildModel(int row, TreeViewListModel *model);

signals:
    void shownChanged(bool shown);
    void parentModelIndexChanged(QModelIndex parentModelIndex);
    void delegateHeightChanged(int delegateHeight);
    void treeViewDataChanged(TreeViewData * treeViewData);

protected:
    int modelHeight() const;
    TreeViewListModel * childModel(int row) const;
    QAbstractItemModel *refModel() const;

protected slots:
    void onModelAboutToBeReset();
    void onModelReset();
    void updateDelegateHeight(bool recursive = false);
    void onCurrentIndexChanged(const QModelIndex &currentIndex);

public:
    virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override;
    virtual QModelIndex parent(const QModelIndex& index) const override;
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

protected:
    bool m_shown = false;
    int m_delegateHeight = 30;
    QModelIndex m_parentModelIndex;
    int m_currentIndexRow = -1;

    TreeViewData * m_data = nullptr;

    QMap<int, TreeViewListModel *> m_childModelMap;

    friend class TreeViewData;
};

Q_DECLARE_METATYPE(TreeViewListModel*);
