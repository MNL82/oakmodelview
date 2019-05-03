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

#include <QObject>
#include "TreeViewListModel.h"

// =============================================================================
// Class definition
// =============================================================================
class TreeViewData : public QObject
{
    Q_OBJECT

    // Output
    Q_PROPERTY(int itemHeight READ itemHeight WRITE setItemHeight NOTIFY itemHeightChanged)
    Q_PROPERTY(int spacing READ spacing WRITE setSpacing NOTIFY spacingChanged)

    // Input/Output
    Q_PROPERTY(QModelIndex rootModelIndex READ rootModelIndex WRITE setRootModelIndex NOTIFY rootModelIndexChanged)
    Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged)
    Q_PROPERTY(TreeViewListModel *rootListModel READ rootListModel WRITE setRootListModel NOTIFY rootListModelChanged)
    Q_PROPERTY(QModelIndex currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    explicit TreeViewData(QObject *parent = nullptr);

    bool isValid() const;

    int itemHeight() const;
    int spacing() const;
    QModelIndex rootModelIndex() const;
    QAbstractItemModel * model() const;

    TreeViewListModel * rootListModel() const;

    QModelIndex currentIndex() const;

signals:
    void rootModelIndexChanged(const QModelIndex &rootModelIndex);
    void modelChanged(QAbstractItemModel * model);
    void itemHeightChanged(int itemHeight);
    void rootListModelChanged(TreeViewListModel * rootListModel);

    void spacingChanged(int spacing);

    void currentIndexChanged(const QModelIndex &currentIndex);

public slots:
    void setItemHeight(int itemHeight);
    void setSpacing(int spacing);
    void setRootModelIndex(QModelIndex rootModelIndex);
    void setModel(QAbstractItemModel * model);

    void setRootListModel(TreeViewListModel * rootListModel);

    void setCurrentIndex(QModelIndex currentIndex);

protected:
    void modelConnect();
    void modelDisconnect();

    TreeViewListModel *modelFromIndex(const QModelIndex &index);

protected slots:
    void onModelAboutToBeReset();
    void onModelReset();

    void onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end);
//    void onRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
//    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onRowsInserted(const QModelIndex &parent, int first, int last);
//    void onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
//    void onRowsRemoved(const QModelIndex &parent, int first, int last);

protected:
    int m_spacing = 0;
    int m_itemHeight = 30;
    QModelIndex m_rootModelIndex;
    QAbstractItemModel * m_model = nullptr;
    TreeViewListModel * m_rootListModel = nullptr;
    QModelIndex m_currentIndex;
};

