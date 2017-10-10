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
    template<class T>
    static ValueDefinitionUPtr Make(T valueTemplate, VariantCRef valueId, VariantCRef defaultValue = VariantCRef(), ConversionSPtr conversion = ConversionSPtr());
#ifdef XML_BACKEND
    template<class T>
    static ValueDefinitionUPtr MakeXML(T valueTemplate, std::string tagName = std::string(), std::string attributeName = std::string(), VariantCRef defaultValue = VariantCRef(), ConversionSPtr conversion = ConversionSPtr());
#endif // XML_BACKEND

    static bool setValueId(ValueDefinitionUPtr& valueDef, VariantCRef valueId);

    static bool setDefaultValue(const ValueDefinitionUPtr& valueDef, VariantCRef defaultValue);

    static bool setDefaultConversion(const ValueDefinitionUPtr& valueDef, ConversionSPtr conversion);

    static bool addOption(ValueDefinition &valueDef, VariantCRef option);

    template<typename T>
    static bool setOptions(const ValueDefinitionUPtr& valueDef, const std::vector<T> &options);

    static ValueSettings& settings(const ValueDefinitionUPtr& valueDef);

#ifdef XML_BACKEND
    static bool setValueRef(const ValueDefinitionUPtr& valueDef, XML::ValueRefUPtr valueRef);

    static Variant generateValueId(std::string tagName, std::string attributeName);
#endif // XML_BACKEND
};

// =============================================================================
// (public)
template<class T>
ValueDefinitionUPtr ValueDefinitionBuilder::Make(T valueTemplate, VariantCRef valueId, VariantCRef defaultValue, ConversionSPtr conversion)
{
    if (!defaultValue.isNull()) {
        assert(defaultValue.isBaseTypeEqual(VariantCRef(valueTemplate)));
    }
    ValueDefinitionUPtr valueDef = ValueDefinition::MakeUPtr(valueTemplate);
    valueDef->m_defaultValue = defaultValue;
    valueDef->m_valueId = valueId;
    valueDef->m_defaultConversion = (conversion) ? conversion : Conversion::globalDefault();

#ifdef XML_BACKEND
    if (valueId.canGet(std::string(), true, valueDef->m_defaultConversion.get())) {
        std::string tagName;
        valueId.get(tagName, true, valueDef->m_defaultConversion.get());
        if (XML::Element::validateTagName(tagName)) {
            valueDef->m_valueRef = XML::ValueRef::MakeUPtr("", XML::ChildRef::MakeUPtr(tagName));
        }
    }
#endif // XML_BACKEND

    return std::move(valueDef);
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
template<class T>
ValueDefinitionUPtr ValueDefinitionBuilder::MakeXML(T valueTemplate, std::string tagName, std::string attributeName, VariantCRef defaultValue, ConversionSPtr conversion)
{
    if (!defaultValue.isNull()) {
        assert(defaultValue.isBaseTypeEqual(VariantCRef(valueTemplate)));
    }

    ValueDefinitionUPtr valueDef = ValueDefinition::MakeUPtr(valueTemplate);
    valueDef->m_defaultValue = defaultValue;
    valueDef->m_defaultConversion = (conversion) ? conversion : Conversion::globalDefault();
    valueDef->m_valueId = generateValueId(tagName, attributeName);

    if (tagName.empty()) {
        valueDef->m_valueRef = XML::ValueRef::MakeUPtr(attributeName);
    } else {
        valueDef->m_valueRef = XML::ValueRef::MakeUPtr(attributeName, XML::ChildRef::MakeUPtr(tagName));
    }

    return std::move(valueDef);
}
#endif // XML_BACKEND

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
