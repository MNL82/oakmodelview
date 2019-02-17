/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ListViewNode.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

#include <QDebug>

#include "ListView.h"

#include "../ServiceFunctions/Assert.h"

#define SPACING_H 4
#define BEFORE 10
#define CONTENT_HEIGHT 24
#define CONTENT_MARGIN 1
#define CONTENT_BORDER 1


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
ListViewNode::ListViewNode(ListView * listView, const Model::Node &node, int depth, ListViewNode * parent)
    : QWidget(nullptr)
{
    ASSERT(listView != nullptr);

    m_listView = listView;
    m_node = node;
    m_depth = depth;
    m_parent = parent;

    setObjectName(QString::fromStdString(node.def()->name()));

    int deltaDepth = m_listView->maxDepth() - m_depth;
    bool canHaveChildren = deltaDepth > 0 && m_node.def()->containerCount() > 0;

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(SPACING_H);

    if (m_depth > 0) {
        QColor qColor(120,120,120);
        if (m_node.def()->hasColor()) {
            auto color = m_node.def()->color();
            qColor = QColor(color.red(), color.green(), color.blue());
        }
        m_styleSheetNormal = createStyleSheetNormal(qColor);
        m_styleSheetCurrent = createStyleSheetCurrent(qColor);

        m_nodeFrame = new QFrame();
        m_nodeFrame->setFocusPolicy(Qt::StrongFocus);
        m_nodeFrame->setObjectName("level_" + QString::number(depth));
        m_nodeFrame->setFrameShape(QFrame::StyledPanel);
        m_nodeFrame->setLineWidth(CONTENT_BORDER);
        m_nodeFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_nodeFrame->installEventFilter(this);
        m_nodeFrame->setStyleSheet(m_styleSheetNormal);
        auto nodeHLayout = new QHBoxLayout();
        nodeHLayout->setContentsMargins(5, CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN);
        nodeHLayout->setSpacing(5);

        if (m_node.def()->hasImagePath()) {
            QPixmap image(QString::fromStdString(m_node.def()->imagePath()));
            if (!image.isNull()) {
                if (image.width() != 24 || image.height() != 24) {
                    image = image.scaled(24, 24);
                }
                QLabel *imageLabel = new QLabel();
                imageLabel->setFixedHeight(24);
                imageLabel->setFixedWidth(24);
                imageLabel->setPixmap(image);
                nodeHLayout->addWidget(imageLabel);
            }
        }

        QString name = QString::fromStdString(m_node.def()->displayName()) + ": " + QString::fromStdString(m_node.leaf("name").toString());
        m_label = new QLabel(name);
        m_label->setStyleSheet("Text-align:left");
        m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        m_label->setFixedHeight(CONTENT_HEIGHT);

        m_nodeFrame->setFixedHeight(CONTENT_HEIGHT + 2 * (CONTENT_MARGIN + CONTENT_BORDER));
        m_nodeFrame->setLayout(nodeHLayout);
        nodeHLayout->addWidget(m_label);

        if (canHaveChildren) {
            m_exspandbuttom = new QPushButton("+");
            m_exspandbuttom->setFocusPolicy(Qt::NoFocus);
            m_exspandbuttom->setObjectName("level_" + QString::number(depth));
            m_exspandbuttom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            m_exspandbuttom->setFixedHeight(CONTENT_HEIGHT);
            m_exspandbuttom->setFixedWidth(CONTENT_HEIGHT);
            nodeHLayout->addWidget(m_exspandbuttom);
            connect(m_exspandbuttom, SIGNAL(clicked()), this, SLOT(onExspandChanged()));
        }

        layout->addWidget(m_nodeFrame);
    }

    if (canHaveChildren) {
        m_childNodeWidget = new QWidget();
        m_childNodeLayout = new QVBoxLayout();
        m_childNodeLayout->setContentsMargins((m_depth == 0) ? 0 : BEFORE, 0, 0, 0);
        m_childNodeLayout->setSpacing(SPACING_H);

        Model::Node cNode = m_node.firstChild();
        while (!cNode.isNull()) {
            QWidget * w = new ListViewNode(m_listView, cNode, m_depth+1, this);
            connect(w, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)));
            m_childNodeLayout->addWidget(w);
            cNode = m_node.nextChild(cNode);
        }

        m_childNodeWidget->setLayout(m_childNodeLayout);
        layout->addWidget(m_childNodeWidget);

        if (m_exspandbuttom != nullptr) {
            m_exspandbuttom->setEnabled(m_childNodeLayout->count() > 0);
        }
    }
    setLayout(layout);
    setExspanded(false);
}

// =============================================================================
// (public)
const Model::Node& ListViewNode::node() const
{
    return m_node;
}

// =============================================================================
// (public)
int ListViewNode::childCount() const
{
    return m_childNodeLayout->count();
}

// =============================================================================
// (public)
int ListViewNode::childViewNodeIndex(const ListViewNode *childViewNode)
{
    for (int i = 0; i < m_childNodeLayout->count(); i++)
    {
        if (childViewNode == m_childNodeLayout->itemAt(i)->widget()) {
            return i;
        }
    }
    return -1;
}

// =============================================================================
// (public)
ListViewNode *ListViewNode::child(const Model::Node &node)
{
    for (int i = 0; i < m_childNodeLayout->count(); i++)
    {
        ListViewNode * cNode = static_cast<ListViewNode*>(m_childNodeLayout->itemAt(i)->widget());
        if (cNode->node().nodeData() == node.nodeData()) {
            return cNode;
        }
    }
    return nullptr;
}

// =============================================================================
// (public)
ListViewNode *ListViewNode::child(int index)
{
    if (index < 0 || index >= m_childNodeLayout->count()) { return nullptr; }

    return static_cast<ListViewNode*>(m_childNodeLayout->itemAt(index)->widget());
}

// =============================================================================
// (public)
ListViewNode *ListViewNode::child(const Model::NodeIndex &nodeIndex)
{
    int index = nodeIndex.convertIndexToUnnamed(m_node);
    ASSERT(index >= 0);

    ListViewNode * childNode = child(index);
    ASSERT(childNode);
    if (nodeIndex.hasChildNodeIndex()) { // Look deeper
        return childNode->child(nodeIndex.childNodeIndex());
    } else { // Found the child ListViewNode
        return childNode;
    }
}

// =============================================================================
// (public)
ListViewNode *ListViewNode::parent()
{
    return m_parent;
}

// =============================================================================
// (public)
ListViewNode *ListViewNode::nextSibling()
{
    if (m_parent == nullptr) { return nullptr; }
    int index = m_parent->childViewNodeIndex(this);
    ASSERT(index != -1);
    return m_parent->child(index+1);
}

// =============================================================================
// (public)
ListViewNode *ListViewNode::previousSibling()
{
    if (m_parent == nullptr) { return nullptr; }
    int index = m_parent->childViewNodeIndex(this);
    ASSERT(index != -1);
    return m_parent->child(index-1);
}

// =============================================================================
// (public)
int ListViewNode::childIndex(ListViewNode *child) const
{
    int count = m_childNodeLayout->count();
    for (int i = 0; i < count; i++) {
        if (m_childNodeLayout->itemAt(i)->widget() == child) {
            return i;
        }
    }
    return false;
}

// =============================================================================
// (public)
void ListViewNode::giveFocus()
{
    if (m_nodeFrame) { m_nodeFrame->setFocus(); }
}

// =============================================================================
// (public)
bool ListViewNode::isExspanded() const
{
    if (m_exspandbuttom == nullptr) { return false; }
    return m_exspandbuttom->text() == "-";
}

// =============================================================================
// (public)
void ListViewNode::setExspanded(bool value)
{
    if (m_childNodeWidget != nullptr && m_exspandbuttom != nullptr) {
        m_childNodeWidget->setHidden(!value);
        m_exspandbuttom->setText(value ? "-" : "+");
    }
    updateFixedheight();
}

// =============================================================================
// (public)
QSize ListViewNode::sizeHint() const
{
    return QSize(200, m_height);
}

// =============================================================================
// (public)
void ListViewNode::clearCurrent()
{
    if (m_nodeFrame) {
        m_nodeFrame->setStyleSheet(m_styleSheetNormal);
    }
}

// =============================================================================
// (public)
void ListViewNode::setCurrent()
{
    if (m_nodeFrame) {
        m_nodeFrame->setStyleSheet(m_styleSheetCurrent);
    }
}

// =============================================================================
// (public)
void ListViewNode::updateLabel()
{
    if (m_label) {
        QString name = QString::fromStdString(m_node.def()->displayName()) + ": " + QString::fromStdString(m_node.leaf("name").toString());
        m_label->setText(name);
    }
}

// =============================================================================
// (protected)
QString ListViewNode::createStyleSheetCurrent(QColor color)
{
    QString colorStr = QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());
    QString stylesheet = QString(".QFrame { "
                                 "  background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop: 0 rgba(%1, 30), stop: 0.4 rgba(%1, 30), stop: 1 white); "
                                 "}"
                                 ".QPushButton:hover { "
                                 "  background: rgba(%1, 100);"
                                 "}").arg(colorStr);
    return stylesheet;
}

// =============================================================================
// (protected)
QString ListViewNode::createStyleSheetNormal(QColor color)
{
    QString colorStr = QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());
    QString stylesheet = QString(".QFrame { "
                                 "  background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop: 0 rgba(%1, 100), stop: 0.4 rgba(%1, 100), stop: 1 white); "
                                 "}"
                                 ".QFrame:hover {"
                                 "  background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop: 0 rgba(%1, 180), stop: 0.4 rgba(%1, 180), stop: 1 white);"
                                 "}"
                                 ".QFrame:focus {"
                                 "  background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop: 0 rgba(%1, 180), stop: 0.4 rgba(%1, 180), stop: 1 white);"
                                 "}"
                                 ".QPushButton:hover { "
                                 "  background: rgba(%1, 100);"
                                 "}").arg(colorStr);
    return stylesheet;
}

// =============================================================================
// (protected)
void ListViewNode::updateFixedheight()
{
    int height = 0;
    if (m_nodeFrame != nullptr) {
        height = m_nodeFrame->height();
    }
    if (m_childNodeWidget != nullptr && !m_childNodeWidget->isHidden()) {
        for (int i = 0; i < m_childNodeLayout->count(); i++)
        {
            height += m_childNodeLayout->itemAt(i)->widget()->sizeHint().height() +  SPACING_H;
        }
    }
    if (height == m_height) { return; }
    int change = height - m_height;
    m_height = height;
    setFixedHeight(m_height);
    emit heightChanged(change);
}

// =============================================================================
// (protected)
void ListViewNode::onNodeInserteAfter(const Model::NodeIndex &nodeIndex)
{
    int index = nodeIndex.convertIndexToUnnamed(m_node);
    ASSERT(index >= 0);

    if (nodeIndex.hasChildNodeIndex()) {
        child(index)->onNodeInserteAfter(nodeIndex.childNodeIndex());
    } else {
        Model::Node cNode = m_node.childAt(index);
        QWidget * w = new ListViewNode(m_listView, cNode, m_depth+1, this);
        connect(w, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)));

        m_childNodeLayout->insertWidget(index, w);

        if (m_childNodeLayout->count() == 1) {
            m_exspandbuttom->setEnabled(true);
            setExspanded(isExspanded());
        } else {
            onHeightChanged(w->sizeHint().height() + SPACING_H);
        }
    }
}

// =============================================================================
// (protected)
void ListViewNode::onNodeRemoveBefore(const Model::NodeIndex &nodeIndex)
{
    int index = nodeIndex.convertIndexToUnnamed(m_node);
    ASSERT(index >= 0);

    if (nodeIndex.hasChildNodeIndex()) {
        child(index)->onNodeRemoveBefore(nodeIndex.childNodeIndex());
    } else {
        // Remove the child ListViewNode
        QLayoutItem * layoutNode = m_childNodeLayout->takeAt(index);
        QWidget * w = layoutNode->widget();

        onHeightChanged(-w->sizeHint().height() - SPACING_H);
        if (m_childNodeLayout->count() == 0) {
            m_exspandbuttom->setEnabled(false);
            m_childNodeWidget->setHidden(true);
        }

        delete layoutNode;
        delete w;
    }
}

// =============================================================================
// (protected)
bool ListViewNode::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_nodeFrame) {
        if (event->type() == QEvent::FocusIn) {
            m_node.model()->setCurrentNode(m_node);
            return true;
        }
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent * kEvent = dynamic_cast<QKeyEvent*>(event);
            if (kEvent->key() == Qt::Key_Down) {
                if (isExspanded() && childCount() > 0) {
                    child(0)->giveFocus();
                    return true;
                }
                ListViewNode * node1 = this;
                while(node1 != nullptr) {
                    ListViewNode * node2 = node1->nextSibling();
                    if (node2) {
                        node2->giveFocus();
                        return true;
                    }
                    node1 = node1->parent();
                }
                return true;
            } else if (kEvent->key() == Qt::Key_Up) {
                ListViewNode * sNode = previousSibling();
                if (sNode) {
                    while (sNode->isExspanded() && sNode->childCount() > 0) {
                        sNode = sNode->child(sNode->childCount()-1);
                    }
                    sNode->giveFocus();
                } else {
                    ListViewNode * pNode = parent();
                    if (pNode != nullptr) {
                        pNode->giveFocus();
                    }
                }
                return true;
            } else if (kEvent->key() == Qt::Key_Right) {
                if (m_childNodeLayout && m_childNodeLayout->count() > 0) {
                    setExspanded(true);
                }
            } else if (kEvent->key() == Qt::Key_Left) {
                if (m_childNodeLayout && m_childNodeLayout->count() > 0) {
                    setExspanded(false);
                }
            }
        }
    }
    return false;
}

// =============================================================================
// (protected slots)
void ListViewNode::onHeightChanged(int change)
{
    if (m_childNodeWidget->isHidden()) { return; }
    m_height += change;
    setFixedHeight(m_height);
    emit heightChanged(change);
}

// =============================================================================
// (protected slots)
void ListViewNode::onExspandChanged()
{
    setExspanded(!isExspanded());
}

} // namespace Oak::View::QtWidgets

