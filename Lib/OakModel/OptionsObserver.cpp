/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "OptionsObserver.h"

#include "OakModel.h"
#include "NodeDef.h"
#include "EntryQuery.h"
#include "ItemIndex.h"
#include "QueryBuilder.h"

#include "../ServiceFunctions/Trace.h"


namespace Oak::Model {

// =============================================================================
// (public)
OptionsObserver::OptionsObserver(OakModel *model, const NodeDef *optionsNodeDef, const ValueDef *optionsValueDef)
    : ObserverInterface(model),
      m_optionsNodeDef { optionsNodeDef},
      m_optionsValueDef { optionsValueDef}
{
    ASSERT(m_optionsNodeDef);
    ASSERT(m_optionsValueDef);
    ASSERT(m_optionsValueDef->options().hasQuery());

    const EntryQuery *query = m_optionsValueDef->options().query();
    m_sourceNodeDef = query->itemQuery().nodeDef(m_optionsNodeDef);
    m_sourceValueName = query->valueName();

    // Create an inverse query that points from the option values to the entry where there can be chosen
    m_inverseQuery = QueryBuilder::createInverse(query->itemQuery(), m_optionsNodeDef)->EntryUPtr(m_optionsValueDef->name());


}

// =============================================================================
// (public)
void OptionsObserver::connect()
{
    m_model->notifier_itemInserteAfter.add(this, &OptionsObserver::onItemInserteAfter);
    m_model->notifier_itemMoveBefore.add(this, &OptionsObserver::onItemMoveBefore);
    m_model->notifier_itemMoveAfter.add(this, &OptionsObserver::onItemMoveAfter);
    m_model->notifier_itemCloneAfter.add(this, &OptionsObserver::onItemCloneAfter);

    m_model->notifier_itemRemoveBefore.add(this, &OptionsObserver::onItemRemoveBefore);

    m_model->notifier_entryChangeBefore.add(this, &OptionsObserver::onEntryChangeBefore);
    m_model->notifier_entryChangeAfter.add(this, &OptionsObserver::onEntryChangeAfter);
}

// =============================================================================
// (public)
void OptionsObserver::disconnect()
{
    m_model->notifier_itemInserteAfter.remove(this);
    m_model->notifier_itemMoveBefore.remove(this);
    m_model->notifier_itemMoveAfter.remove(this);
    m_model->notifier_itemCloneAfter.remove(this);

    m_model->notifier_itemRemoveBefore.remove(this);

    m_model->notifier_entryChangeBefore.remove(this);
    m_model->notifier_entryChangeAfter.remove(this);
}

// =============================================================================
// (protected)
void OptionsObserver::onItemInserteAfter(const ItemIndex &itemIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onItemMoveAfter(const ItemIndex &sourceItemIndex, const ItemIndex &targetItemIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onItemMoveBefore(const ItemIndex &sourceItemIndex, const ItemIndex &targetItemIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onItemCloneAfter(const ItemIndex &sourceItemIndex, const ItemIndex &targetItemIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onItemRemoveBefore(const ItemIndex &itemIndex)
{

}

// =============================================================================
// (protected)
void OptionsObserver::onEntryChangeBefore(const ItemIndex &itemIndex, const std::string &valueName)
{
    // If not valid return as fast as possible
    if (m_sourceValueName != valueName) { return; }
    if (m_sourceNodeDef->name() != itemIndex.lastItemIndex().name()) { return; }

    Item sourceItem = itemIndex.item(m_model->rootItem());

    m_valueBeforeChange = sourceItem.entry(valueName).value();
}

// =============================================================================
// (protected)
void OptionsObserver::onEntryChangeAfter(const ItemIndex &itemIndex, const std::string &valueName)
{
    // If not valid return as fast as possible
    if (m_valueBeforeChange.isNull()) { return; }
    if (m_sourceValueName != valueName) { return; }
    if (m_sourceNodeDef->name() != itemIndex.lastItemIndex().name()) { return; }

    Item sourceItem = itemIndex.item(m_model->rootItem());
    UnionValue newValue = sourceItem.entry(valueName).value();

    if (m_valueBeforeChange == newValue) { return; }

    // Loop though all the entries where the option have been used
    auto it = m_inverseQuery->iterator(sourceItem);
    while (it->next()) {
        UnionValue value = it->entry().value();
        if (value == m_valueBeforeChange) {
            // Update the value if it is the one changed
            it->entry().setValue(newValue);
        } else if (it->item() == m_model->currentItem()) {
            // Update the current item because the option list have changed
            m_model->setCurrentItem(m_model->currentItem(), true);
        }
    }

    m_valueBeforeChange = UnionValue();
}

} // namespace Oak::Model

