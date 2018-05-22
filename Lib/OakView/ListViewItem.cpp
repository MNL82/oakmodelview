#include "ListViewItem.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

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

    int deltaDepth = listView->depth() - m_depth;

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setContentsMargins(0, 1 , 0, 1);
    layout->setSpacing(0);

    if (depth > 0) {
        QString name = QString::fromStdString(item.def()->displayName()) + ": " + QString::fromStdString(item.value("name").toString());
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

        Model::Item cItem = item.firstChild();
        while (!cItem.isNull()) {
            m_childCount++;
            QWidget * w = new ListViewItem(listView, cItem, depth+1);
            m_childItemLayout->addWidget(w);
            m_height += w->sizeHint().height();
            cItem = item.nextChild(cItem);
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
QSize ListViewItem::sizeHint() const
{
    return QSize(200, m_height);
}

} // namespace View
} // namespace Oak
