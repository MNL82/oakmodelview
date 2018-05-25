#include "ListViewItem.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

#include <QDebug>

#include "ListView.h"

#define hSpacing 4

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
    layout->setContentsMargins((m_depth == 0) ? 0 : 5, 0, 0, 0);
    layout->setSpacing(hSpacing);

    if (m_depth > 0) {
        int margin = 3;
        int border = 1;
        int contentHeight = 24;

        m_itemFrame = new QFrame();
        m_itemFrame->setStyleSheet(".QFrame { border: 1px solid green; border-radius: 4px; }");
        // background-image: url(images/welcome.png);
        m_itemFrame->setFrameShape(QFrame::StyledPanel);
        m_itemFrame->setLineWidth(border);
        m_itemFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        auto itemHLayout = new QHBoxLayout();
        itemHLayout->setMargin(margin);
        itemHLayout->setSpacing(5);

        QString name = QString::fromStdString(m_item.def()->displayName()) + ": " + QString::fromStdString(m_item.value("name").toString());
        auto label = new QLabel(name);
        label->setStyleSheet("Text-align:left");
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        label->setFixedHeight(contentHeight);

        m_itemFrame->setFixedHeight(contentHeight + 2*(margin+border));
        m_itemFrame->setLayout(itemHLayout);
        itemHLayout->addWidget(label);

        if (canHaveChildren) {
            m_exspandbuttom = new QPushButton("+");
            m_exspandbuttom->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            m_exspandbuttom->setFixedHeight(contentHeight);
            m_exspandbuttom->setFixedWidth(contentHeight);
            itemHLayout->addWidget(m_exspandbuttom);
            connect(m_exspandbuttom, SIGNAL(clicked()), this, SLOT(onExspandChanged()));
        }

        layout->addWidget(m_itemFrame);
    }

    if (canHaveChildren) {
        m_childItemWidget = new QWidget();
        m_childItemLayout = new QVBoxLayout();
        m_childItemLayout->setMargin(0);
        m_childItemLayout->setSpacing(hSpacing);

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
    if (m_itemFrame != nullptr) {
        height = m_itemFrame->height();
    }
    if (m_childItemWidget != nullptr && !m_childItemWidget->isHidden()) {
        for (int i = 0; i < m_childItemLayout->count(); i++)
        {
            height += m_childItemLayout->itemAt(i)->widget()->sizeHint().height() +  hSpacing;
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
