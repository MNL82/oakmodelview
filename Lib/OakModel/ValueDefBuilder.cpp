/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueDefBuilder.h"

#include "XMLChildRef.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
bool ValueDefBuilder::setName(ValueDefUPtr& valueDef, const std::string &valueName)
{
    if (!valueDef) { return false; }

    valueDef->m_name = valueName;

    return true;
}

// =============================================================================
// (public)
bool ValueDefBuilder::setDefaultConversion(const ValueDefUPtr& valueDef, ConversionSPtr conversion)
{
    if (!valueDef) { return false; }
    if (!conversion) { return false; }

    valueDef->m_defaultConversion = conversion;

    return true;
}

// =============================================================================
// (public)
bool ValueDefBuilder::setQueryOptions(const ValueDefUPtr& valueDef, QueryRefSPtr queryRef)
{
    if (!valueDef) { return false; }

    if (!valueDef->m_options) {
        valueDef->m_options = new ValueOptions();
    }
    valueDef->m_options->m_query = queryRef;

    return true;
}

// =============================================================================
// (public)
bool ValueDefBuilder::setQueryOptionsExcluded(const ValueDefUPtr& valueDef, QueryRefSPtr queryRef)
{
    if (!valueDef) { return false; }

    if (!valueDef->m_options) {
        valueDef->m_options = new ValueOptions();
    }
    valueDef->m_options->m_queryExcluded = queryRef;

    return true;
}

// =============================================================================
// (public)
ValueSettings &ValueDefBuilder::settings(const ValueDefUPtr &valueDef)
{
    return valueDef->m_settings;
}

// =============================================================================
// (public)
bool ValueDefBuilder::setValueRef(const ValueDefUPtr& valueDef, XML::ValueRefUPtr valueRef)
{
    if (!valueDef) { return false; }
    if (!valueRef) { return false; }

    valueDef->m_valueRef = std::move(valueRef);

    return true;
}

// =============================================================================
// (public)
std::string ValueDefBuilder::generateValueId(std::string tagName, std::string attributeName)
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
