#pragma once

#include <QTableWidget>

#include "OakModel.h"

namespace Oak {
namespace View {

// =============================================================================
// Class definition
// =============================================================================
class TableView : public QTableWidget
{
public:
    TableView(QWidget *parent = nullptr);
    virtual ~TableView() override;

    void setOakModel(Model::OakModel* model);
    void setRootItem(const Model::Item& item);

    void onItemInserted(const Model::Item& parentItem, int index);
    void onItemMoved(const Model::Item& sourceParentItem, int sourceIndex, const Model::Item& targetParentItem, int targetIndex);
    void onItemCloned(const Model::Item& sourceParentItem, int sourceIndex, const Model::Item& targetParentItem, int targetIndex);
    void onItemRemoved(const Model::Item& parentItem, int index);
    void onItemValueChanged(const Model::Item &item, int valueIndex);

protected:
    Model::OakModel * m_model = nullptr;
    Model::Item m_rootItem;
};

} // namespace View
} // namespace Oak
