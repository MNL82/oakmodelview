/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueSettingsBuilderData.h"

#include "ValueDefBuilder.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak::Model {

// =============================================================================
// (public)
ValueSettingsBuilderData::ValueSettingsBuilderData()
{
    //TRACE("ValueSettingsBuilderData");
}

// =============================================================================
// (public)
void ValueSettingsBuilderData::validate(std::vector<std::string>& errorMessages) const
{
    UNUSED(errorMessages)
    //errorMessages.push_back("ValueSettings validation not implemented");
}

// =============================================================================
// (public)
void ValueSettingsBuilderData::set(ValueDefBuilderSPtr builder) const
{
    if (readOnly != BoolState_Undefined) {
        builder->setSetting(OPTION_ONLY, readOnly == BoolState_True);
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