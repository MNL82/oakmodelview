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

    int deltaDepth = m_listView->maxDepth() - m_depth;
    bool canHaveChildren = deltaDepth > 0 && m_item.def()->containerCount() > 0;

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins((m_depth == 0) ? 0 : 5, 0 , 0, 0);
    layout->setSpacing(0);

    if (m_depth > 0) {
        int widgetMargin = 2;

        m_itemWidget = new QWidget();
        m_itemWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        auto itemHLayout = new QHBoxLayout();
        itemHLayout->setContentsMargins(0, widgetMargin/2, 0, widgetMargin/2);
        itemHLayout->setSpacing(0);

        QString name = QString::fromStdString(m_item.def()->displayName()) + ": " + QString::fromStdString(m_item.value("name").toString());
        auto button = new QPushButton(name);
        button->setStyleSheet("Text-align:left");
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        int widgetHeight = button->sizeHint().height() + 4 * deltaDepth;

        m_itemWidget->setFixedHeight(widgetHeight + widgetMargin);
        m_itemWidget->setLayout(itemHLayout);
        itemHLayout->addWidget(button);

        if (canHaveChildren) {
            m_exspandbuttom = new QPushButton("+");
            m_exspandbuttom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            m_exspandbuttom->setFixedHeight(widgetHeight);
            m_exspandbuttom->setFixedWidth(widgetHeight);
            itemHLayout->addWidget(m_exspandbuttom);
            connect(m_exspandbuttom, SIGNAL(clicked()), this, SLOT(onExspandChanged()));
        }

        layout->addWidget(m_itemWidget);
    }

    if (canHaveChildren) {
        m_childItemWidget = new QWidget();
        m_childItemLayout = new QVBoxLayout();
        m_childItemLayout->setMargin(0);
        m_childItemLayout->setSpacing(0);

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
        if (cItem->item() == item) {
            return cItem;
        }
    }
    return nullptr;
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
void ListViewItem::updateFixedheight()
{
    int height = 0;
    if (m_itemWidget != nullptr) {
        height = m_itemWidget->sizeHint().height();
    }
    if (m_childItemWidget != nullptr && !m_childItemWidget->isHidden()) {
        for (int i = 0; i < m_childItemLayout->count(); i++)
        {
            height += m_childItemLayout->itemAt(i)->widget()->sizeHint().height();
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
