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
#include "NodeIndex.h"

class QScrollArea;
class QGridLayout;
class QVBoxLayout;


namespace Oak::View::QtWidgets {

class ListViewNode;

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
    void setRootNode(const Model::Node& node);

    void currentNodeChanged();

    int maxDepth() const;
    void setMaxDepth(int maxDepth);

    void resizeEvent(QResizeEvent *event) override;

protected:
    ListViewNode * getViewNode(const Model::NodeIndex &nodeIndex);

    void createDragNodes() const;
    void clearDragNodes() const;

    void onNodeInserteAfter(const Model::NodeIndex &nodeIndex);
    void onNodeMoveAfter(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex);
    void onNodeMoveBefore(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex);
    void onNodeCloneAfter(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex);
    void onNodeRemoveBefore(const Model::NodeIndex &nodeIndex);
    void onVariantLeafChangeAfter(const Model::NodeIndex &nodeIndex);
    void onKeyLeafChangeAfter(const Model::NodeIndex &nodeIndex);

protected slots:
    void adjustNodeWidth();
    void onCurrentNodeViewDestoyed();

protected:
    Model::OakModel * m_model = nullptr;
    ListViewNode * m_rootNode = nullptr;
    Model::NodeIndexUPtr m_rootNodeIndex;

    ListViewNode * m_currentViewNode = nullptr;

    int m_maxDepth = 2;

    QScrollArea * m_scrollArea;
    QGridLayout * m_dragLayout;
};

} // namespace Oak::View::QtWidgets

