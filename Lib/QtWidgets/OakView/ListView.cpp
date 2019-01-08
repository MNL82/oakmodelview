/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ListView.h"

#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QResizeEvent>
#include <QScrollBar>

#include <QDebug>

#include "OakModel.h"
#include "ListViewNode.h"


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
ListView::ListView(QWidget *parent)
    : QFrame(parent)
{
    m_scrollArea = new QScrollArea();
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Update stylesheet
    QString styleSheet = ".QFrame {"
                         "  border: 1px solid rgba(0, 0, 0, 255);"
                         "  border-radius: 4px;"
                         "}"
                         ".QFrame:focus {"
                         "  border: 1px solid rgba(140, 0, 0, 255);"
                         "}"
                         ".QPushButton {"
                         "  background: transparent;"
                         "}"
                         ".QPushButton:hover { "
                         "  border-radius: 3px;"
                         "  border: 1px solid rgba(120, 120, 120, 255);"
                         "}";

    m_scrollArea->setStyleSheet(styleSheet);

    // Horizontal line between scrollable existing nodes and draggable new nodes
    auto lineA = new QFrame;
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);
    lineA->setHidden(true);

    // Available new nodes that can be dragged into place
    auto dragNodes = new QWidget();
    dragNodes->setHidden(true);
    m_dragLayout = new QGridLayout();
    m_dragLayout->setMargin(0);
    dragNodes->setLayout(m_dragLayout);

    // TEST Buttons - Begin
    auto dragButton1 = new QPushButton("Drag 1");
    auto dragButton2 = new QPushButton("Drag 2");
    auto dragButton3 = new QPushButton("Drag 3");
    auto dragButton4 = new QPushButton("Drag 4");
    auto dragButton5 = new QPushButton("Drag 5");
    auto dragButton6 = new QPushButton("Drag 6");
    m_dragLayout->addWidget(dragButton1, 0, 0);
    m_dragLayout->addWidget(dragButton2, 0, 1);
    m_dragLayout->addWidget(dragButton3, 0, 2);
    m_dragLayout->addWidget(dragButton4, 1, 0);
    m_dragLayout->addWidget(dragButton5, 1, 1);
    m_dragLayout->addWidget(dragButton6, 1, 2);
    // TEST Buttons - End

    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_scrollArea, 1);
    mainLayout->addWidget(lineA);
    mainLayout->addWidget(dragNodes, 0);
    setLayout(mainLayout);
}

// =============================================================================
// (public)
ListView::~ListView()
{

}

// =============================================================================
// (public)
void ListView::setOakModel(Model::OakModel *model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentNodeChanged.remove(this);

        m_model->notifier_nodeInserteAfter.remove(this);
        m_model->notifier_nodeMoveBefore.remove(this);
        m_model->notifier_nodeMoveAfter.remove(this);
        m_model->notifier_nodeCloneAfter.remove(this);
        m_model->notifier_nodeRemoveBefore.remove(this);
        m_model->notifier_variantLeafChangeAfter.remove(this);
        m_model->notifier_keyLeafChangeAfter.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentNodeChanged.add(this, &ListView::currentNodeChanged);

        m_model->notifier_nodeInserteAfter.add(this, &ListView::onNodeInserteAfter);
        m_model->notifier_nodeMoveBefore.add(this, &ListView::onNodeMoveBefore);
        m_model->notifier_nodeMoveAfter.add(this, &ListView::onNodeMoveAfter);
        m_model->notifier_nodeCloneAfter.add(this, &ListView::onNodeCloneAfter);
        m_model->notifier_nodeRemoveBefore.add(this, &ListView::onNodeRemoveBefore);
        m_model->notifier_variantLeafChangeAfter.add(this, &ListView::onVariantLeafChangeAfter);
        m_model->notifier_keyLeafChangeAfter.add(this, &ListView::onKeyLeafChangeAfter);
    }
}

// =============================================================================
// (public)
void ListView::setRootNode(const Model::Node &node)
{
    if (m_rootNode != nullptr) {
        // Do nothing if the root node is the same
        if (m_rootNode->node() == node) { return; }

        disconnect(m_rootNode, SIGNAL(heightChanged()), this, SLOT(adjustNodeWidth()));
        //Clear existing node before creating a new one
    }
    m_rootNode = new ListViewNode(this, node, 0);
    m_rootNodeIndex = Model::NodeIndex::create(node);

    m_scrollArea->setWidget(m_rootNode);
    m_rootNode->setFixedWidth(m_scrollArea->viewport()->width());

    connect(m_rootNode, SIGNAL(heightChanged(int)), this, SLOT(adjustNodeWidth()), Qt::QueuedConnection);
}

// =============================================================================
// (public)
void ListView::currentNodeChanged()
{
    ListViewNode * currentViewNode = getViewNode(m_model->currentNodeIndex());

    if (currentViewNode == m_currentViewNode) { return; }

    if (m_currentViewNode) {
        m_currentViewNode->clearCurrent();
        disconnect(m_currentViewNode, SIGNAL(destroyed()), this, SLOT(onCurrentNodeViewDestoyed()));
    }

    m_currentViewNode = currentViewNode;

    if (m_currentViewNode) {
        m_currentViewNode->setCurrent();
        connect(m_currentViewNode, SIGNAL(destroyed()), this, SLOT(onCurrentNodeViewDestoyed()));

        // Make current nodes visible
        ListViewNode * pNode = m_currentViewNode->parent();
        if (pNode != nullptr) {
            while (pNode != m_rootNode) {
                pNode->setExspanded(true);
                pNode = pNode->parent();
            }
        }
    }
}

// =============================================================================
// (public)
int ListView::maxDepth() const
{
    return m_maxDepth;
}

// =============================================================================
// (public)
void ListView::setMaxDepth(int depth)
{
    if (m_maxDepth == depth) { return; }

    m_maxDepth = depth;

    // TODO: Update view and drag nodes when depth changes
}

// =============================================================================
// (public)
void ListView::resizeEvent(QResizeEvent *event)
{
    if (m_rootNode != nullptr) {
        m_rootNode->setFixedWidth(m_scrollArea->viewport()->width());
    }
    QWidget::resizeEvent(event);
}

// =============================================================================
// (protected)
ListViewNode *ListView::getViewNode(const Model::NodeIndex &nodeIndex)
{
    // Return if change is outside root node
    if (!nodeIndex.contains(*m_rootNodeIndex.get())) { return nullptr; }

    // Get the node index relative to the root node
    const Model::NodeIndex &relNodeIndex = nodeIndex.childNodeIndex(m_rootNodeIndex->depth());

    return m_rootNode->child(relNodeIndex);
}

// =============================================================================
// (protected)
void ListView::createDragNodes() const
{

}

// =============================================================================
// (protected)
void ListView::clearDragNodes() const
{

}

// =============================================================================
// (protected)
void ListView::onNodeInserteAfter(const Model::NodeIndex &nodeIndex)
{
    // Return if change is outside root node
    if (!nodeIndex.contains(*m_rootNodeIndex.get())) { return; }

    // Get the node index relative to the root node
    const Model::NodeIndex &relNodeIndex = nodeIndex.childNodeIndex(m_rootNodeIndex->depth());

    m_rootNode->onNodeInserteAfter(relNodeIndex);
}

// =============================================================================
// (protected)
void ListView::onNodeMoveAfter(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex)
{
    Q_UNUSED(sourceNodeIndex);
    onNodeInserteAfter(targetNodeIndex);
}

// =============================================================================
// (protected)
void ListView::onNodeMoveBefore(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex)
{
    Q_UNUSED(targetNodeIndex);
    onNodeRemoveBefore(sourceNodeIndex);
}

// =============================================================================
// (protected)
void ListView::onNodeCloneAfter(const Model::NodeIndex &sourceNodeIndex, const Model::NodeIndex &targetNodeIndex)
{
    Q_UNUSED(sourceNodeIndex);
    onNodeInserteAfter(targetNodeIndex);
}

// =============================================================================
// (protected)
void ListView::onNodeRemoveBefore(const Model::NodeIndex &nodeIndex)
{
    // Return if change is outside root node
    if (!nodeIndex.contains(*m_rootNodeIndex.get())) { return; }

    // Get the node index relative to the root node
    const Model::NodeIndex &relNodeIndex = nodeIndex.childNodeIndex(m_rootNodeIndex->depth());

    m_rootNode->onNodeRemoveBefore(relNodeIndex);
}

// =============================================================================
// (protected)
void ListView::onVariantLeafChangeAfter(const Model::NodeIndex &nodeIndex)
{
    // Child nodes can change when the variant definition change
    onNodeRemoveBefore(nodeIndex);
    onNodeInserteAfter(nodeIndex);

    currentNodeChanged();
}

// =============================================================================
// (protected)
void ListView::onKeyLeafChangeAfter(const Model::NodeIndex &nodeIndex)
{
    // Child nodes can change when the variant definition change
    ListViewNode * viewNode = getViewNode(nodeIndex);
    if (viewNode != nullptr) {
        viewNode->updateLabel();
    }
}

// =============================================================================
// (protected slots)
void ListView::adjustNodeWidth()
{
    if (m_rootNode != nullptr) {
        m_rootNode->setFixedWidth(m_scrollArea->viewport()->width());
    }
}

// =============================================================================
// (protected slots)
void ListView::onCurrentNodeViewDestoyed()
{
    m_currentViewNode = nullptr;
}

} // namespace Oak::View::QtWidgets

