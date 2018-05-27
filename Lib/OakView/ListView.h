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

class QScrollArea;
class QGridLayout;
class QVBoxLayout;

namespace Oak {
namespace View {

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

    void onItemInserted(const Model::Item& parentItem, int index);
    void onItemMoved(const Model::Item& sourceParentItem, int sourceIndex, const Model::Item& targetParentItem, int targetIndex);
    void onItemCloned(const Model::Item& sourceParentItem, int sourceIndex, const Model::Item& targetParentItem, int targetIndex);
    void onItemRemoved(const Model::Item& parentItem, int index);
    void onItemValueChanged(const Model::Item &item, int valueIndex);

    int maxDepth() const;
    void setMaxDepth(int maxDepth);

    void resizeEvent(QResizeEvent *event) override;

protected:
    ListViewItem * getViewItem(const Model::Item &item);

    void createDragItems() const;
    void clearDragItems() const;

    QString createStyleSheep(QColor color, int level);

protected slots:
    void adjustItemWidth();
    void onCurrentItemViewDestoyed();

protected:
    Model::OakModel * m_model = nullptr;
    ListViewItem * m_rootItem = nullptr;

    ListViewItem * m_currentViewItem = nullptr;

    int m_maxDepth = 2;

    QScrollArea * m_scrollArea;
    QGridLayout * m_dragLayout;
};

} // namespace View
} // namespace Oak
