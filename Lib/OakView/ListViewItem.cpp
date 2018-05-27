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
ListViewItem::ListViewItem(ListView * listView, const Model::Item &item, int depth)
    : QWidget(nullptr)
{
    assert(listView != nullptr);

    m_listView = listView;
    m_item = item;
    m_depth = depth;

    int deltaDepth = m_listView->maxDepth() - m_depth;
    bool canHaveChildren = deltaDepth > 0 && m_item.def()->containerCount() > 0;

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setSpacing(SPACING_H);

    if (m_depth > 0) {
        m_itemFrame = new QFrame();
        m_itemFrame->setObjectName("level_" + QString::number(depth));
        m_itemFrame->setFrameShape(QFrame::StyledPanel);
        m_itemFrame->setLineWidth(CONTENT_BORDER);
        m_itemFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        auto itemHLayout = new QHBoxLayout();
        itemHLayout->setContentsMargins(5, CONTENT_MARGIN, CONTENT_MARGIN, CONTENT_MARGIN);
        itemHLayout->setSpacing(5);

        QString name = QString::fromStdString(m_item.def()->displayName()) + ": " + QString::fromStdString(m_item.value("name").toString());
        m_label = new QLabel(name);
        m_label->setStyleSheet("Text-align:left");
        m_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        m_label->setFixedHeight(CONTENT_HEIGHT);

        m_itemFrame->setFixedHeight(CONTENT_HEIGHT + 2 * (CONTENT_MARGIN + CONTENT_BORDER));
        m_itemFrame->setLayout(itemHLayout);
        itemHLayout->addWidget(m_label);

        if (canHaveChildren) {
            m_exspandbuttom = new QPushButton("+");
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
            QWidget * w = new ListViewItem(m_listView, cItem, m_depth+1);
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
    QWidget * w = new ListViewItem(m_listView, cItem, m_depth+1);
    connect(w, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)));

    m_childItemLayout->insertWidget(index, w);

    if (m_childItemLayout->count() == 1) {
        m_exspandbuttom->setEnabled(true);
        setExspanded(isExspanded());
    } else {
        onHeightChanged(w->sizeHint().height());
    }
}

// =============================================================================
// (public)
void ListViewItem::onItemRemoved(int index)
{
    assert(index >= 0 && index < m_childItemLayout->count());

    QLayoutItem * layoutItem = m_childItemLayout->takeAt(index);
    QWidget * w = layoutItem->widget();

    onHeightChanged(-w->sizeHint().height());
    if (m_childItemLayout->count() == 0) {
        m_exspandbuttom->setEnabled(false);
        m_childItemWidget->setHidden(true);
    }

    delete layoutItem;
    delete w;
}

// =============================================================================
// (public)
void ListViewItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        event->accept();
        m_item.model()->setCurrentItem(m_item);
    }
}

// =============================================================================
// (public)
void ListViewItem::clearCurrent()
{
    if (m_itemFrame) {
        m_itemFrame->setStyleSheet("");
    }
}

// =============================================================================
// (public)
void ListViewItem::setCurrent()
{
    if (m_itemFrame) {
        QString styleSheet = currentStylesheet(QColor(100, 100, 100));
        m_itemFrame->setStyleSheet(styleSheet);
    }
}

// =============================================================================
// (public)
void ListViewItem::updateLabel()
{
    if (m_label) {
        QString name = QString::fromStdString(m_item.def()->displayName()) + ": " + QString::fromStdString(m_item.value("name").toString());
        m_label->setText(name);
    }
}

// =============================================================================
// (protected)
QString ListViewItem::currentStylesheet(QColor color)
{
    QString colorStr = QString("%1,%2,%3").arg(color.red()).arg(color.green()).arg(color.blue());
    QString stylesheet = QString(".QFrame {"
                                 "  background: qlineargradient(x1:0, y1:1, x2:1, y2:0, stop: 0 rgba(%1, 180), stop: 0.4 rgba(%1, 180), stop: 1 white);"
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
