/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "LeafSettingsBuilderData.h"

#include "LeafDefBuilder.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak::Model {

// =============================================================================
// (public)
LeafSettingsBuilderData::LeafSettingsBuilderData()
{
    //TRACE("LeafSettingsBuilderData");
}

// =============================================================================
// (public)
void LeafSettingsBuilderData::validate(std::vector<std::string>& errorMessages) const
{
    UNUSED(errorMessages)
    //errorMessages.push_back("LeafSettings validation not implemented");
}

// =============================================================================
// (public)
void LeafSettingsBuilderData::set(LeafDefBuilderSPtr builder) const
{
    if (optionsOnly != BoolState_Undefined) {
        builder->setSetting(OPTION_ONLY, optionsOnly == BoolState_True);
    }

    if (!unit.empty()) {
        builder->setSetting(UNIT, unit);
    }

    if (unique != BoolState_Undefined) {
        builder->setSetting(UNIQUE, unique);
    }

    if (required != BoolState_Undefined) {
        builder->setSetting(REQUIRED, required);
    }
}


} // namespace Oak::Model
