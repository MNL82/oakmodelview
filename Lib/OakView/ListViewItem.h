#pragma once

#include <QWidget>

#include "Item.h"


class QVBoxLayout;

namespace Oak {
namespace View {

class ListView;

// =============================================================================
// Class definition
// =============================================================================

class ListViewItem : public QWidget
{
    Q_OBJECT
public:
    ListViewItem(ListView * listView, const Model::Item &item, int depth);

    const Model::Item& item() const;

    virtual QSize sizeHint() const override;

signals:

public slots:

protected:
    int m_height;
    int m_depth;
    int m_childCount = 0;
    bool m_exspanded = true;


    ListView * m_listView;
    Model::Item m_item;

    QVBoxLayout * m_childItemLayout;
    QWidget * m_childItemWidget;

};

} // namespace View
} // namespace Oak
