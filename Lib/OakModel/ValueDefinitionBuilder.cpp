/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueDefinitionBuilder.h"

#include "XMLChildRef.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
bool ValueDefinitionBuilder::setValueId(ValueDefinitionUPtr &valueDef, VariantCRef valueId)
{
    if (!valueDef) { return false; }
    if (valueId.isNull()) { return false; }

    valueDef->m_valueId = valueId;

    return true;
}

// =============================================================================
// (public)
bool ValueDefinitionBuilder::setDefaultValue(const ValueDefinitionUPtr& valueDef, VariantCRef defaultValue)
{
    if (!valueDef) { return false; }

    valueDef->m_defaultValue = defaultValue;

    return true;
}

// =============================================================================
// (public)
bool ValueDefinitionBuilder::setDefaultConversion(const ValueDefinitionUPtr& valueDef, ConversionSPtr conversion)
{
    if (!valueDef) { return false; }
    if (!conversion) { return false; }

    valueDef->m_defaultConversion = conversion;

    return true;
}

// =============================================================================
// (public)
bool ValueDefinitionBuilder::addOption(ValueDefinition& valueDef, VariantCRef option)
{
    if (option.isBaseTypeEqual(valueDef.m_valueTemplate)) {
        valueDef.m_options.push_back(option);
        return true;
    }
    return false;
}

// =============================================================================
// (public)
bool ValueDefinitionBuilder::setOptions(const ValueDefinitionUPtr& valueDef, const std::vector<VariantCRef>& options)
{
    if (!options.empty() && options.front().isBaseTypeEqual(valueDef->m_valueTemplate)) {
        valueDef->m_options.clear();
        for (const auto& option: options)
        {
            valueDef->m_options.push_back(option);
        }
        return true;
    }
    return false;
}

// =============================================================================
// (public)
ValueSettings &ValueDefinitionBuilder::settings(const ValueDefinitionUPtr &valueDef)
{
    return valueDef->m_settings;
}

// =============================================================================
// (public)
bool ValueDefinitionBuilder::setValueRef(const ValueDefinitionUPtr& valueDef, XML::ValueRefUPtr valueRef)
{
    if (!valueDef) { return false; }
    if (!valueRef) { return false; }

    valueDef->m_valueRef = std::move(valueRef);

    return true;
}

// =============================================================================
// (public)
Variant ValueDefinitionBuilder::generateValueId(std::string tagName, std::string attributeName)
{
    // The value id is created from the tag name and the attribute name
    Variant valueId;

    if (tagName.empty()) {
        if (attributeName.empty()) {
            valueId = std::string("Body");
        } else {
            valueId = attributeName;
        }
    } else {
        if (attributeName.empty()) {
            valueId = tagName;
        } else {
            valueId = tagName + "[" + attributeName + "]";
        }
    }
    return std::move(valueId);
}

} // namespace Model
} // namespace Oak
