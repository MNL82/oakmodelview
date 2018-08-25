#include "TableView.h"

namespace Oak {
namespace View {

// =============================================================================
// (public)
TableView::TableView(QWidget *parent)
    : QTableWidget(parent)
{

}

// =============================================================================
// (public)
TableView::~TableView()
{

}

// =============================================================================
// (public)
void TableView::setBaseRef(Model::ItemQueryUPtr baseRef)
{
    if (m_baseRef != baseRef) {
        m_baseRef = std::move(baseRef);
        updateTable();
    }
}

// =============================================================================
// (public)
void TableView::addValueRef(Model::ValueQuerySPtr valueRef)
{
    m_valueRefList.push_back(valueRef);
    updateTable();
}

// =============================================================================
// (public)
void TableView::updateTable()
{
    clearContents();

    if (m_model == nullptr) { return; }

    if (m_model->rootItem().isNull()) { return; }

    m_rootItem = m_model->rootItem();
    //if (m_baseRef.)

}

// =============================================================================
// (public)
void TableView::setOakModel(Model::OakModel *model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
//        m_model->notifier_currentItemChanged.remove(this);
        m_model->notifier_rootNodeDefChanged.remove(this);
        m_model->notifier_rootNodeChanged.remove(this);
//        m_model->notifier_destroyed.remove(this);

        m_model->notifier_itemInserted.remove(this);
        m_model->notifier_itemMoved.remove(this);
        m_model->notifier_itemCloned.remove(this);
        m_model->notifier_itemRemoved.remove(this);

        m_model->notifier_itemValueChanged.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
//        m_model->notifier_currentItemChanged.add(this, &TableView::currentItemChanged);
        m_model->notifier_rootNodeDefChanged.add(this, &TableView::updateTable);
        m_model->notifier_rootNodeChanged.add(this, &TableView::updateTable);
//        m_model->notifier_destroyed.add(this, &ListView::modelDestroyed);

        m_model->notifier_itemInserted.add(this, &TableView::onItemInserted);
        m_model->notifier_itemMoved.add(this, &TableView::onItemMoved);
        m_model->notifier_itemCloned.add(this, &TableView::onItemCloned);
        m_model->notifier_itemRemoved.add(this, &TableView::onItemRemoved);

        m_model->notifier_itemValueChanged.add(this, &TableView::onItemValueChanged);
    }
}

// =============================================================================
// (public)
void TableView::onItemInserted(const Model::Item &parentItem, int index)
{

}

// =============================================================================
// (public)
void TableView::onItemMoved(const Model::Item &sourceParentItem, int sourceIndex, const Model::Item &targetParentItem, int targetIndex)
{

}

// =============================================================================
// (public)
void TableView::onItemCloned(const Model::Item &sourceParentItem, int sourceIndex, const Model::Item &targetParentItem, int targetIndex)
{

}

// =============================================================================
// (public)
void TableView::onItemRemoved(const Model::Item &parentItem, int index)
{

}

// =============================================================================
// (public)
void TableView::onItemValueChanged(const Model::Item &item, int valueIndex)
{

}

} // namespace View
} // namespace Oak
