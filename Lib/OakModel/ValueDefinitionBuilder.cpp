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
bool ValueDefinitionBuilder::setName(ValueDefinitionUPtr& valueDef, const std::string &valueName)
{
    if (!valueDef) { return false; }

    valueDef->m_name = valueName;

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
std::string ValueDefinitionBuilder::generateValueId(std::string tagName, std::string attributeName)
{
    // The value id is created from the tag name and the attribute name
    if (tagName.empty()) {
        if (attributeName.empty()) {
            return std::string("Body");
        } else {
            return attributeName;
        }
    } else {
        if (attributeName.empty()) {
            return tagName;
        } else {
            return tagName + "[" + attributeName + "]";
        }
    }
}

} // namespace Model
} // namespace Oak
