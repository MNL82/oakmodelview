/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include "ValueDefinition.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class ValueDefinitionBuilder
{
    ValueDefinitionBuilder() = delete;
public:
    template<typename T1, typename T2>
    static ValueDefinitionUPtr Make(T1 valueTemplate, T2 valueId);
    template<typename T1, typename T2>
    static ValueDefinitionUPtr Make(T1 valueTemplate, T2 valueId, T1 defaultValue);
#ifdef XML_BACKEND
    template<typename T>
    static ValueDefinitionUPtr MakeXML(T valueTemplate, std::string tagName = std::string(), std::string attributeName = std::string());
    template<typename T>
    static ValueDefinitionUPtr MakeXML(T valueTemplate, std::string tagName, std::string attributeName, T defaultValue);
#endif // XML_BACKEND

    template<typename T>
    static bool setValueId(ValueDefinitionUPtr& valueDef, T valueId);

    template<typename T>
    static bool setDefaultValue(const ValueDefinitionUPtr& valueDef, T defaultValue);

    static bool setDefaultConversion(const ValueDefinitionUPtr& valueDef, ConversionSPtr conversion);

    template<typename T>
    static bool addOption(ValueDefinition &valueDef, T option);

    template<typename T>
    static bool setOptions(const ValueDefinitionUPtr& valueDef, const std::vector<T> &options);

    static ValueSettings& settings(const ValueDefinitionUPtr& valueDef);

#ifdef XML_BACKEND
    static bool setValueRef(const ValueDefinitionUPtr& valueDef, XML::ValueRefUPtr valueRef);

    static std::string generateValueId(std::string tagName, std::string attributeName);
#endif // XML_BACKEND
};

// =============================================================================
// (public)
template<typename T1, typename T2>
ValueDefinitionUPtr ValueDefinitionBuilder::Make(T1 valueTemplate, T2 valueId)
{
    ValueDefinitionUPtr valueDef = ValueDefinition::MakeUPtr(valueTemplate);
    valueDef->m_valueId = valueId;
    valueDef->m_defaultConversion = Conversion::globalDefault();

#ifdef XML_BACKEND
    std::string tagName;
    if (convert(tagName, valueId)) {
        if (XML::Element::validateTagName(tagName)) {
            valueDef->m_valueRef = XML::ValueRef::MakeUPtr("", XML::ChildRef::MakeUPtr(tagName));
        }
    }
#endif // XML_BACKEND

    return std::move(valueDef);
}

// =============================================================================
// (public)
template<typename T1, typename T2>
ValueDefinitionUPtr ValueDefinitionBuilder::Make(T1 valueTemplate, T2 valueId, T1 defaultValue)
{
    auto vDef = Make(valueTemplate, valueId);
    vDef->m_defaultValue = defaultValue;
    return std::move(vDef);
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
template<typename T>
ValueDefinitionUPtr ValueDefinitionBuilder::MakeXML(T valueTemplate, std::string tagName, std::string attributeName)
{
    ValueDefinitionUPtr valueDef = ValueDefinition::MakeUPtr(valueTemplate);
    valueDef->m_defaultConversion = Conversion::globalDefault();
    valueDef->m_valueId = generateValueId(tagName, attributeName);

    if (tagName.empty()) {
        valueDef->m_valueRef = XML::ValueRef::MakeUPtr(attributeName);
    } else {
        valueDef->m_valueRef = XML::ValueRef::MakeUPtr(attributeName, XML::ChildRef::MakeUPtr(tagName));
    }

    return std::move(valueDef);
}

// =============================================================================
// (public)
template<typename T>
ValueDefinitionUPtr ValueDefinitionBuilder::MakeXML(T valueTemplate, std::string tagName, std::string attributeName, T defaultValue)
{
    auto vDef = MakeXML(valueTemplate, tagName, attributeName);
    vDef->m_defaultValue = defaultValue;
    return std::move(vDef);
}
#endif // XML_BACKEND

// =============================================================================
// (public)
template<typename T>
static bool ValueDefinitionBuilder::setValueId(ValueDefinitionUPtr& valueDef, T valueId)
{
    if (!valueDef) { return false; }

    valueDef->m_valueId = valueId;

    return true;
}

// =============================================================================
// (public)
template<typename T>
static bool ValueDefinitionBuilder::setDefaultValue(const ValueDefinitionUPtr& valueDef, T defaultValue)
{
    if (!valueDef) { return false; }

    valueDef->m_defaultValue = defaultValue;

    return true;
}

// =============================================================================
// (public)
template<typename T>
static bool ValueDefinitionBuilder::addOption(ValueDefinition &valueDef, T option)
{
    if (valueDef.m_valueTemplate.isBaseTypeEqual(option)) {
        valueDef.m_options.push_back(option);
        return true;
    }
    return false;
}

// =============================================================================
// (public)
template<typename T>
static bool ValueDefinitionBuilder::setOptions(const ValueDefinitionUPtr& valueDef, const std::vector<T> &options)
{
    if (!options.empty() && valueDef->m_valueTemplate.isBaseTypeEqual(options.front())) {
        valueDef->m_options.clear();
        for (const auto& option: options)
        {
            valueDef->m_options.push_back(option);
        }
        return true;
    }
    return false;
}

typedef ValueDefinitionBuilder VDB;

} // namespace Model
} // namespace Oak
