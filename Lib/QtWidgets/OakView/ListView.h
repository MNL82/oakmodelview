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

#include <QFrame>

#include "OakModel.h"
#include "ItemIndex.h"

class QScrollArea;
class QGridLayout;
class QVBoxLayout;


namespace Oak::View::QtWidgets {

class ListViewItem;

// =============================================================================
// Class definition
// =============================================================================
class ListView : public QFrame
{
    Q_OBJECT

public:
    ListView(QWidget *parent = nullptr);
    virtual ~ListView() override;

    void setOakModel(Model::OakModel* model);
    void setRootItem(const Model::Item& item);

    void currentItemChanged();

    int maxDepth() const;
    void setMaxDepth(int maxDepth);

    void resizeEvent(QResizeEvent *event) override;

protected:
    ListViewItem * getViewItem(const Model::ItemIndex &itemIndex);

    void createDragItems() const;
    void clearDragItems() const;

    void onItemInserteAfter(const Model::ItemIndex &itemIndex);
    void onItemMoveAfter(const Model::ItemIndex &sourceItemIndex, const Model::ItemIndex &targetItemIndex);
    void onItemMoveBefore(const Model::ItemIndex &sourceItemIndex, const Model::ItemIndex &targetItemIndex);
    void onItemCloneAfter(const Model::ItemIndex &sourceItemIndex, const Model::ItemIndex &targetItemIndex);
    void onItemRemoveBefore(const Model::ItemIndex &itemIndex);
    void onVariantLeafChangeAfter(const Model::ItemIndex &itemIndex);
    void onKeyLeafChangeAfter(const Model::ItemIndex &itemIndex);

protected slots:
    void adjustItemWidth();
    void onCurrentItemViewDestoyed();

protected:
    Model::OakModel * m_model = nullptr;
    ListViewItem * m_rootItem = nullptr;
    Model::ItemIndexUPtr m_rootItemIndex;

    ListViewItem * m_currentViewItem = nullptr;

    int m_maxDepth = 2;

    QScrollArea * m_scrollArea;
    QGridLayout * m_dragLayout;
};

} // namespace Oak::View::QtWidgets

