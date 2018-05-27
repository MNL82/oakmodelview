#pragma once

#include <QWidget>

#include "Item.h"


class QVBoxLayout;
class QPushButton;
class QFrame;

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

    ListViewItem * child(const Model::Item &item);

    bool isExspanded() const;
    void setExspanded(bool value);

    virtual QSize sizeHint() const override;

    void onItemInserted(int index);
    void onItemRemoved(int index);

    void clearCurrent();
    void setCurrent();

    virtual void mouseReleaseEvent(QMouseEvent *event) override;


protected:
    void updateFixedheight();

protected slots:
    void onHeightChanged(int change);
    void onExspandChanged();

signals:
    void heightChanged(int change);

protected:
    int m_height;
    int m_depth;
    int m_childCount = 0;

    ListView * m_listView;
    Model::Item m_item;

    QFrame * m_itemFrame = nullptr;
    QPushButton * m_exspandbuttom = nullptr;

    QWidget * m_childItemWidget = nullptr;
    QVBoxLayout * m_childItemLayout = nullptr;

};

} // namespace View
} // namespace Oak
