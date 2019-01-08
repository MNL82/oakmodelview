/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "LeafDefBuilderData.h"

#include "LeafDefBuilder.h"

#include "../ServiceFunctions/Trace.h"

namespace Oak::Model {

// =============================================================================
// (public)
LeafDefBuilderData::LeafDefBuilderData()
{
    //TRACE("LeafDefBuilderData");
}

// =============================================================================
// (public)
void LeafDefBuilderData::validate(std::vector<std::string>& errorMessages) const
{
    //errorMessages.push_back("LeafDef validation not implemented");

    if (name.empty()) {
        errorMessages.push_back("LeafDef have to have a name");
    }

    if (dataType == UnionType::Undefined) {
        errorMessages.push_back("LeafDef dataType have to be defined");
    } else if (dataType == UnionType::Char) {
        errorMessages.push_back("LeafDef dataType Char is not supported use String");
    }

    if (settings) {
        settings->validate(errorMessages);
    }

    if (options) {
        options->validate(errorMessages);
    }
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilderData::create() const
{
    UnionType type = (dataType == UnionType::Undefined) ? UnionType::String : dataType;
    LeafDefBuilderSPtr builder = LeafDefBuilder::create(type, name);
    if (!displayName.empty()) {
        builder->setDisplayName(displayName);
    }
    if (!defaultValue.isNull()) {
        builder->setDefaultValue(defaultValue);
    }
    if (!tooltip.empty()) {
        builder->setTooltip(tooltip);
    }


    if (settings) {
        settings->set(builder);
    }
    if (options) {
        options->set(builder);
    }
    return builder;
}


} // namespace Oak::Model
