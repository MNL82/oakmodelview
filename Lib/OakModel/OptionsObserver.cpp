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

#include "../ServiceFunctions/Trace.h"

namespace Oak {
namespace Model {

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
}

// =============================================================================
// (public)
void OptionsObserver::connect()
{
    m_model->notifier_entryChangeAfter.add(this, &OptionsObserver::onEntryChangeAfter);
}

// =============================================================================
// (public)
void OptionsObserver::disconnect()
{
    m_model->notifier_entryChangeAfter.remove(this);
}

// =============================================================================
// (public)
void OptionsObserver::onEntryChangeAfter(const ItemIndex &itemIndex, const std::string &valueName)
{
    // If not valid return as fast as possible
    if (m_sourceValueName != valueName) { return; }
    if (m_sourceNodeDef->name() != itemIndex.lastItemIndex().name()) { return; }

    Item sourceItem = itemIndex.item(m_model->rootItem());


    TRACE(L"Options value changed!!!");
}

} // namespace Model
} // namespace Oak
