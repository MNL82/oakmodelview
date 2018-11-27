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

#include "../ServiceFunctions/Trace.h"

namespace Oak::Model {

// =============================================================================
// (public)
ValueOptionsBuilderData::ValueOptionsBuilderData()
{
    TRACE("ValueOptionsBuilderData");
}

// =============================================================================
// (public)
void ValueOptionsBuilderData::validate(std::vector<std::string>& errorMessages) const
{
    errorMessages.push_back("ValueOptions validation not implemented");
}

// =============================================================================
// (public)
bool ValueOptionsBuilderData::createDerived(ValueDef* valueDef) const
{
    UNUSED(valueDef)
    return false;
}

} // namespace Oak::Model
