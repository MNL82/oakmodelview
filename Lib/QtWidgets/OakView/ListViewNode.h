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

#include "Node.h"
#include "NodeIndex.h"


class QVBoxLayout;
class QPushButton;
class QFrame;
class QLabel;


namespace Oak::View::QtWidgets {

class ListView;

// =============================================================================
// Class definition
// =============================================================================

class ListViewNode : public QWidget
{
    Q_OBJECT
public:
    ListViewNode(ListView * listView, const Model::Node &node, int depth, ListViewNode *parent = nullptr);

    const Model::Node& node() const;

    int childCount() const;
    int childViewNodeIndex(const ListViewNode * childViewNode);

    ListViewNode * child(const Model::Node &node);
    ListViewNode * child(int index);
    ListViewNode * child(const Model::NodeIndex &nodeIndex);

    ListViewNode * parent();
    ListViewNode * nextSibling();
    ListViewNode * previousSibling();

    int childIndex(ListViewNode* child) const;

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

    void onNodeInserteAfter(const Model::NodeIndex &nodeIndex);
    void onNodeRemoveBefore(const Model::NodeIndex &nodeIndex);

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
    ListViewNode * m_parent;
    Model::Node m_node;

    QFrame * m_nodeFrame = nullptr;
    QLabel * m_label = nullptr;
    QPushButton * m_exspandbuttom = nullptr;

    QWidget * m_childNodeWidget = nullptr;
    QVBoxLayout * m_childNodeLayout = nullptr;

    QString m_styleSheetNormal;
    QString m_styleSheetCurrent;

    friend class ListView;
};

} // namespace Oak::View::QtWidgets

