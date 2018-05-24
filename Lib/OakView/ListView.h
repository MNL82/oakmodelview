#pragma once

#include <QWidget>

#include "OakModel.h"

class QScrollArea;
class QGridLayout;
class QVBoxLayout;

namespace Oak {
namespace View {

class ListViewItem;

// =============================================================================
// Class definition
// =============================================================================
class ListView : public QWidget
{
    Q_OBJECT

public:
    ListView(QWidget *parent = nullptr);
    virtual ~ListView() override;

    void setOakModel(Model::OakModel* model);
    void setRootItem(const Model::Item& item);

    void currentItemChanged();

    void setCurrentItem(const Model::Item& item);

    void onItemInserted(const Model::Item& parentItem, int index);
    void onItemMoved(const Model::Item& sourceParentItem, int sourceIndex, const Model::Item& targetParentItem, int targetIndex);
    void onItemCloned(const Model::Item& sourceParentItem, int sourceIndex, const Model::Item& targetParentItem, int targetIndex);
    void onItemRemoved(const Model::Item& parentItem, int index);

    int maxDepth() const;
    void setMaxDepth(int maxDepth);


    void resizeEvent(QResizeEvent *event) override;

protected:
    ListViewItem * getViewItem(const Model::Item &item);

    void createDragItems() const;
    void clearDragItems() const;

protected:
    Model::OakModel * m_model = nullptr;
    ListViewItem * m_rootItem = nullptr;

    int m_maxDepth = 2;

    QScrollArea * m_scrollArea;
//    QWidget * m_viewWidget;
//    QVBoxLayout * m_viewLayout;
    QGridLayout * m_dragLayout;
};

} // namespace View
} // namespace Oak
