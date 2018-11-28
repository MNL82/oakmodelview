/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueOptionsBuilderData.h"

#include "ValueDefBuilder.h"
#include "QueryBuilder.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak::Model {

// =============================================================================
// (public)
ValueOptionsBuilderData::ValueOptionsBuilderData()
{
    //TRACE("ValueOptionsBuilderData");
}

// =============================================================================
// (public)
void ValueOptionsBuilderData::validate(std::vector<std::string>& errorMessages) const
{
    UNUSED(errorMessages)
    //errorMessages.push_back("ValueOptions validation not implemented");
}

// =============================================================================
// (public)
void ValueOptionsBuilderData::set(ValueDefBuilderSPtr builder) const
{
    if(!values.empty()) {
        builder->setOptionsStatic(values);
    }
    if(!excludedValues.empty()) {
        builder->setOptionsExcludedStatic(excludedValues);
    }
    if (!valueQuery.empty()) {
        builder->setOptionsQuery(QueryBuilder::createEntryQuery(valueQuery));
    }
    if (!excludedValueQuery.empty()) {
        builder->setOptionsExcludedQuery(QueryBuilder::createEntryQuery(excludedValueQuery));
    }
}

} // namespace Oak::Model
