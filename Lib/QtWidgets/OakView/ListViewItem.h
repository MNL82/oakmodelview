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

#include <QWidget>

#include "Item.h"
#include "ItemIndex.h"


class QVBoxLayout;
class QPushButton;
class QFrame;
class QLabel;


namespace Oak::View::QtWidgets {

class ListView;

// =============================================================================
// Class definition
// =============================================================================

class ListViewItem : public QWidget
{
    Q_OBJECT
public:
    ListViewItem(ListView * listView, const Model::Item &item, int depth, ListViewItem *parent = nullptr);

    const Model::Item& item() const;

    int childCount() const;
    int childViewItemIndex(const ListViewItem * childViewItem);

    ListViewItem * child(const Model::Item &item);
    ListViewItem * child(int index);
    ListViewItem * child(const Model::ItemIndex &itemIndex);

    ListViewItem * parent();
    ListViewItem * nextSibling();
    ListViewItem * previousSibling();

    int childIndex(ListViewItem* child) const;

    void giveFocus();

    bool isExspanded() const;
    void setExspanded(bool value);

    virtual QSize sizeHint() const override;

    void clearCurrent();
    void setCurrent();

    void updateLabel();

protected:
    QString createStyleSheetCurrent(QColor color);
    QString createStyleSheetNormal(QColor color);

    void updateFixedheight();

    void onItemInserteAfter(const Model::ItemIndex &itemIndex);
    void onItemRemoveBefore(const Model::ItemIndex &itemIndex);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

protected slots:
    void onHeightChanged(int change);
    void onExspandChanged();

signals:
    void heightChanged(int change);

protected:
    int m_height = -1;
    int m_depth;
    int m_childCount = 0;

    ListView * m_listView;
    ListViewItem * m_parent;
    Model::Item m_item;

    QFrame * m_itemFrame = nullptr;
    QLabel * m_label = nullptr;
    QPushButton * m_exspandbuttom = nullptr;

    QWidget * m_childItemWidget = nullptr;
    QVBoxLayout * m_childItemLayout = nullptr;

    QString m_styleSheetNormal;
    QString m_styleSheetCurrent;

    friend class ListView;
};

} // namespace Oak::View::QtWidgets

