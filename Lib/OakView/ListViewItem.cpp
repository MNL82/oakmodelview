/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ListViewItem.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

#include <QDebug>

#include "ListView.h"

#define SPACING_H 4
#define BEFORE 10
#define CONTENT_HEIGHT 24
#define CONTENT_MARGIN 1
#define CONTENT_BORDER 1

namespace Oak {
namespace View {

// =============================================================================
// (public)
ListViewItem::ListViewItem(ListView * listView, const Model::Item &item, int depth, ListViewItem * parent)
    : QWidget(nullptr)
{
    assert(listView != nullptr);

    m_listView = listView;
    m_item = item;
    m_depth = depth;
    m_parent = parent;

    int deltaDepth = m_listView->maxDepth() - m_depth;
    bool canHaveChildren = deltaDepth > 0 && m_item.def()->containerCount() > 0;

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(SPACING_H);

    if (m_depth > 0) {
        QColor qColor(120,120,120);
        if (m_item.def()->hasColor()) {
            auto color = m_item.def()->color();
            qColor = QColor(color.red(), color.green(), color.blue());
        }
        m_styleSheetNormal = createStyleSheetNormal(qColor);
        m_styleSheetCurrent = createStyleSheetCurrent(qColor);

        m_itemFrame = new QFrame();
        m_itemFrame->setFocusPolicy(Qt::StrongFocus);
        m_itemFrame->setObjectName("level_" + QString::number(depth));
        m_itemFrame->setFrameShape(QFrame::StyledPanel);
        m_itemFrame->setLineWidth(CONTENT_BORDER);
        m_itemFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        m_itemFrame->installEventFilter(this);
        m_itemFrame->setStyleSheet(m_styleSheetNormal);
        auto itemHLayout = new QHBoxLayout();
        itemHLayout->setContentsMargins(5, CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN);
        itemHLayout->setSpacing(5);

        if (m_item.def()->hasImagePath()) {
            QPixmap image(QString::fromStdString(m_item.def()->imagePath()));
            if (!image.isNull()) {
                if (image.width() != 24 || image.height() != 24) {
                    image = image.scaled(24, 24);
                }
                QLabel *imageLabel = new QLabel();
                imageLabel->setFixedHeight(24);
                imageLabel->setFixedWidth(24);
                imageLabel->setPixmap(image);
                itemHLayout->addWidget(imageLabel);
            }
        }

        QString name = QString::fromStdString(m_item.def()->displayName()) + ": " + QString::fromStdString(m_item.entry("name").toString());
        m_label = new QLabel(name);
        m_label->setStyleSheet("Text-align:left");
        m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        m_label->setFixedHeight(CONTENT_HEIGHT);

        m_itemFrame->setFixedHeight(CONTENT_HEIGHT + 2 * (CONTENT_MARGIN + CONTENT_BORDER));
        m_itemFrame->setLayout(itemHLayout);
        itemHLayout->addWidget(m_label);

        if (canHaveChildren) {
            m_exspandbuttom = new QPushButton("+");
            m_exspandbuttom->setFocusPolicy(Qt::NoFocus);
            m_exspandbuttom->setObjectName("level_" + QString::number(depth));
            m_exspandbuttom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            m_exspandbuttom->setFixedHeight(CONTENT_HEIGHT);
            m_exspandbuttom->setFixedWidth(CONTENT_HEIGHT);
            itemHLayout->addWidget(m_exspandbuttom);
            connect(m_exspandbuttom, SIGNAL(clicked()), this, SLOT(onExspandChanged()));
        }

        layout->addWidget(m_itemFrame);
    }

    if (canHaveChildren) {
        m_childItemWidget = new QWidget();
        m_childItemLayout = new QVBoxLayout();
        m_childItemLayout->setContentsMargins((m_depth == 0) ? 0 : BEFORE, 0, 0, 0);
        m_childItemLayout->setSpacing(SPACING_H);

        Model::Item cItem = m_item.firstChild();
        while (!cItem.isNull()) {
            QWidget * w = new ListViewItem(m_listView, cItem, m_depth+1, this);
            connect(w, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)));
            m_childItemLayout->addWidget(w);
            cItem = m_item.nextChild(cItem);
        }

        m_childItemWidget->setLayout(m_childItemLayout);
        layout->addWidget(m_childItemWidget);

        if (m_exspandbuttom != nullptr) {
            m_exspandbuttom->setEnabled(m_childItemLayout->count() > 0);
        }
    }
    setLayout(layout);
    setExspanded(false);
}

// =============================================================================
// (public)
const Model::Item& ListViewItem::item() const
{
    return m_item;
}

// =============================================================================
// (public)
int ListViewItem::childCount() const
{
    return m_childItemLayout->count();
}

// =============================================================================
// (public)
int ListViewItem::childViewItemIndex(const ListViewItem *childViewItem)
{
    for (int i = 0; i < m_childItemLayout->count(); i++)
    {
        if (childViewItem == m_childItemLayout->itemAt(i)->widget()) {
            return i;
        }
    }
    return -1;
}

// =============================================================================
// (public)
ListViewItem *ListViewItem::child(const Model::Item &item)
{
    for (int i = 0; i < m_childItemLayout->count(); i++)
    {
        ListViewItem * cItem = static_cast<ListViewItem*>(m_childItemLayout->itemAt(i)->widget());
        if (cItem->item().node() == item.node()) {
            return cItem;
        }
    }
    return nullptr;
}

// =============================================================================
// (public)
ListViewItem *ListViewItem::child(int index)
{
    if (index < 0 || index >= m_childItemLayout->count()) { return nullptr; }

    return static_cast<ListViewItem*>(m_childItemLayout->itemAt(index)->widget());
}

// =============================================================================
// (public)
ListViewItem *ListViewItem::parent()
{
    return m_parent;
}

// =============================================================================
// (public)
ListViewItem *ListViewItem::nextSibling()
{
    if (m_parent == nullptr) { return nullptr; }
    int index = m_parent->childViewItemIndex(this);
    assert(index != -1);
    return m_parent->child(index+1);
}

// =============================================================================
// (public)
ListViewItem *ListViewItem::previousSibling()
{
    if (m_parent == nullptr) { return nullptr; }
    int index = m_parent->childViewItemIndex(this);
    assert(index != -1);
    return m_parent->child(index-1);
}

// =============================================================================
// (public)
void ListViewItem::giveFocus()
{
    if (m_itemFrame) { m_itemFrame->setFocus(); }
}

// =============================================================================
// (public)
bool ListViewItem::isExspanded() const
{
    if (m_exspandbuttom == nullptr) { return false; }
    return m_exspandbuttom->text() == "-";
}

// =============================================================================
// (public)
void ListViewItem::setExspanded(bool value)
{
    if (m_childItemWidget != nullptr && m_exspandbuttom != nullptr) {
        m_childItemWidget->setHidden(!value);
        m_exspandbuttom->setText(value ? "-" : "+");
    }
    updateFixedheight();
}

// =============================================================================
// (public)
QSize ListViewItem::sizeHint() const
{
    return QSize(200, m_height);
}

// =============================================================================
// (public)
void ListViewItem::onItemInserted(int index)
{
    Model::Item cItem = m_item.childAt(index);
    QWidget * w = new ListViewItem(m_listView, cItem, m_depth+1, this);
    connect(w, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)));

    m_childItemLayout->insertWidget(index, w);

    if (m_childItemLayout->count() == 1) {
        m_exspandbuttom->setEnabled(true);
        setExspanded(isExspanded());
    } else {
        onHeightChanged(w->sizeHint().height() + SPACING_H);
    }
}

// =============================================================================
// (public)
void ListViewItem::onItemRemoved(int index)
{
    assert(index >= 0 && index < m_childItemLayout->count());

    QLayoutItem * layoutItem = m_childItemLayout->takeAt(index);
    QWidget * w = layoutItem->widget();

    onHeightChanged(-w->sizeHint().height() - SPACING_H);
    if (m_childItemLayout->count() == 0) {
        m_exspandbuttom->setEnabled(false);
        m_childItemWidget->setHidden(true);
    }

    delete layoutItem;
    delete w;
}

// =============================================================================
// (public)
bool ListViewItem::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_itemFrame) {
        if (event->type() == QEvent::FocusIn) {
            m_item.model()->setCurrentItem(m_item);
            return true;
        }
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent * kEvent = dynamic_cast<QKeyEvent*>(event);
            if (kEvent->key() == Qt::Key_Down) {
                if (isExspanded() && childCount() > 0) {
                    child(0)->giveFocus();
                    return true;
                }
                ListViewItem * item1 = this;
                while(item1 != nullptr) {
                    ListViewItem * item2 = item1->nextSibling();
                    if (item2) {
                        item2->giveFocus();
                        return true;
                    }
                    item1 = item1->parent();
                }
                return true;
            } else if (kEvent->key() == Qt::Key_Up) {
                ListViewItem * sItem = previousSibling();
                if (sItem) {
                    while (sItem->isExspanded() && sItem->childCount() > 0) {
                        sItem = sItem->child(sItem->childCount()-1);
                    }
                    sItem->giveFocus();
                } else {
                    ListViewItem * pItem = parent();
                    if (pItem != nullptr) {
                        pItem->giveFocus();
                    }
                }
                return true;
            } else if (kEvent->key() == Qt::Key_Right) {
                if (m_childItemLayout && m_childItemLayout->count() > 0) {
                    setExspanded(true);
                }
            } else if (kEvent->key() == Qt::Key_Left) {
                if (m_childItemLayout && m_childItemLayout->count() > 0) {
                    setExspanded(false);
                }
            }
        }
    }
    return false;
}

// =============================================================================
// (public)
void ListViewItem::clearCurrent()
{
    if (m_itemFrame) {
        m_itemFrame->setStyleSheet(m_styleSheetNormal);
    }
}

// =============================================================================
// (public)
void ListViewItem::setCurrent()
{
    if (m_itemFrame) {
        m_itemFrame->setStyleSheet(m_styleSheetCurrent);
    }
}

// =============================================================================
// (public)
void ListViewItem::updateLabel()
{
    if (m_label) {
        QString name = QString::fromStdString(m_item.def()->displayName()) + ": " + QString::fromStdString(m_item.entry("name").toString());
        m_label->setText(name);
    }
}

// =============================================================================
// (protected)
QString ListViewItem::createStyleSheetCurrent(QColor color)
{
    QString colorStr = QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());
    QString stylesheet = QString(".QFrame { "
                                 "  background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop: 0 rgba(%1, 180), stop: 0.4 rgba(%1, 180), stop: 1 white); "
                                 "}"
                                 ".QPushButton:hover { "
                                 "  background: rgba(%1, 100);"
                                 "}").arg(colorStr);
    return stylesheet;
}

// =============================================================================
// (protected)
QString ListViewItem::createStyleSheetNormal(QColor color)
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
void ListViewItem::updateFixedheight()
{
    int height = 0;
    if (m_itemFrame != nullptr) {
        height = m_itemFrame->height();
    }
    if (m_childItemWidget != nullptr && !m_childItemWidget->isHidden()) {
        for (int i = 0; i < m_childItemLayout->count(); i++)
        {
            height += m_childItemLayout->itemAt(i)->widget()->sizeHint().height() +  SPACING_H;
        }
    }
    if (height == m_height) { return; }
    int change = height - m_height;
    m_height = height;
    setFixedHeight(m_height);
    emit heightChanged(change);
}

// =============================================================================
// (public)
void ListViewItem::onHeightChanged(int change)
{
    if (m_childItemWidget->isHidden()) { return; }
    m_height += change;
    setFixedHeight(m_height);
    emit heightChanged(change);
}

// =============================================================================
// (public)
void ListViewItem::onExspandChanged()
{
    setExspanded(!isExspanded());
}

} // namespace View
} // namespace Oak
