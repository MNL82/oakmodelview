#include "ListViewItem.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <QDebug>

#include "ListView.h"

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

    m_height = 0;

    int deltaDepth = m_listView->depth() - m_depth;

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins(m_depth * 5, 1 , 0, 1);
    layout->setSpacing(0);

    if (m_depth > 0) {
        QString name = QString::fromStdString(m_item.def()->displayName()) + ": " + QString::fromStdString(m_item.value("name").toString());
        auto itemWidget = new QPushButton(name);
        itemWidget->setStyleSheet("Text-align:left");
        itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        layout->addWidget(itemWidget);

        m_height += itemWidget->sizeHint().height() +
                    3 * deltaDepth +
                    layout->spacing() +
                    layout->contentsMargins().top() +
                    layout->contentsMargins().bottom();
    }

    if (deltaDepth > 0) {
        m_childItemWidget = new QWidget();
        m_childItemLayout = new QVBoxLayout();
        m_childItemLayout->setMargin(0);
        m_childItemLayout->setSpacing(0);

        Model::Item cItem = m_item.firstChild();
        while (!cItem.isNull()) {
            m_childCount++;
            QWidget * w = new ListViewItem(m_listView, cItem, m_depth+1);
            connect(w, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)));
            m_childItemLayout->addWidget(w);
            m_height += w->sizeHint().height();
            cItem = m_item.nextChild(cItem);
        }

        m_childItemWidget->setLayout(m_childItemLayout);
        layout->addWidget(m_childItemWidget);

        if (!m_exspanded || m_childCount == 0) {
            m_childItemWidget->setHidden(true);
        }
    }


    setLayout(layout);
    setFixedHeight(m_height);
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
        if (cItem->item() == item) {
            return cItem;
        }
    }
    return nullptr;
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
    m_childCount++;
    QWidget * w = new ListViewItem(m_listView, cItem, m_depth+1);
    connect(w, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)));
    m_childItemLayout->insertWidget(index, w);
    onHeightChanged(w->sizeHint().height());
}

// =============================================================================
// (public)
void ListViewItem::onItemRemoved(int index)
{
    assert(index >= 0 && index < m_childItemLayout->count());
    QLayoutItem * layoutItem = m_childItemLayout->takeAt(index);
    QWidget * w = layoutItem->widget();
    onHeightChanged(-w->sizeHint().height());
    delete layoutItem;
    delete w;
}

// =============================================================================
// (public)
void ListViewItem::onHeightChanged(int change)
{
    m_height += change;
    setFixedHeight(m_height);
    emit heightChanged(change);
}

} // namespace View
} // namespace Oak
