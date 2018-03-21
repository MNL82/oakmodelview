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

#include "ValueDef.h"
#include "XMLRefFactory.h"
#include "XMLValueRef.h"
#include "ValueOptions.h"

namespace Oak {
namespace Model {

class ValueDefBuilder;
typedef std::shared_ptr<ValueDefBuilder> ValueDefBuilderSPtr;
typedef std::weak_ptr<ValueDefBuilder> ValueDefBuilderWPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueDefBuilder
{
protected:
    ValueDefBuilder(UnionType type, const std::string &name);
    ValueDefBuilder(ValueDef *valueDef);

public:
    static ValueDefBuilderSPtr create(UnionType type, const std::string &name);
    static ValueDefBuilderSPtr use(ValueDef *valueDef);

    ValueDefBuilderSPtr setName(const std::string &value);
    ValueDefBuilderSPtr setDisplayName(const std::string &value);
    ValueDefBuilderSPtr setTooltip(const std::string &value);


    ValueDefBuilderSPtr setDefaultValue(const UnionRef &value);

    ValueDefBuilderSPtr setDefaultConversion(ConversionSPtr value);

    ValueDefBuilderSPtr addOptionStatic(const UnionRef & value);
    template<typename T>
    ValueDefBuilderSPtr setOptionsStatic(const std::vector<T> &options);
    template<typename T>
    ValueDefBuilderSPtr setOptionsExcludedStatic(const std::vector<T> &options);

    ValueDefBuilderSPtr setOptionsQuery(QueryRefSPtr value);
    ValueDefBuilderSPtr setOptionsExcludedQuery(QueryRefSPtr value);

    ValueSettings& settings();

#ifdef XML_BACKEND
    ValueDefBuilderSPtr setXMLValueRef(XML::ValueRefUPtr value);
#endif
    const ValueDef &valueDef() const;
    ValueDefUPtr get();

private:
    ValueDefUPtr m_valueDefUPtr;
    ValueDef *m_valueDef;
    ValueDefBuilderWPtr m_thisWPtr;

//public:
//    template<typename T>
//    static ValueDefUPtr Make(T valueTemplate, const std::string &name, const std::string &displayName = std::string());
//    template<typename T>
//    static ValueDefUPtr Make(T valueTemplate, const std::string &name, const std::string &displayName, T defaultValue);
//#ifdef XML_BACKEND
//    template<typename T>
//    static ValueDefUPtr MakeXML(T valueTemplate, const std::string& elementRef = "", const std::string& attributeName = "");
//    template<typename T>
//    static ValueDefUPtr MakeXML(T valueTemplate, const std::string& elementRef, const std::string& attributeName, T defaultValue);
//#endif // XML_BACKEND

//    static bool setName(ValueDefUPtr& valueDef, const std::string &valueName);
//    static bool setTooltip(ValueDefUPtr& valueDef, const std::string &tooltip);

//    template<typename T>
//    static bool setDefaultValue(ValueDefUPtr& valueDef, T defaultValue);

//    static bool setDefaultConversion(const ValueDefUPtr& valueDef, ConversionSPtr conversion);


//    template<typename T>
//    static bool addStaticOption(ValueDef &valueDef, T option);
//    template<typename T>
//    static bool setStaticOptions(const ValueDefUPtr& valueDef, const std::vector<T> &options);
//    template<typename T>
//    static bool setStaticOptionsExcluded(const ValueDefUPtr& valueDef, const std::vector<T> &options);

//    static bool setOptionsQuery(const ValueDefUPtr& valueDef, QueryRefSPtr queryRef);
//    static bool setOptionsExcludedQuery(const ValueDefUPtr& valueDef, QueryRefSPtr queryRef);

//    static ValueSettings& settings(const ValueDefUPtr& valueDef);

//#ifdef XML_BACKEND
//    static bool setValueRef(const ValueDefUPtr& valueDef, XML::ValueRefUPtr valueRef);

//    static std::string generateValueId(std::string tagName, std::string attributeName);
//#endif // XML_BACKEND
};

// =============================================================================
// (public)
template<typename T>
ValueDefBuilderSPtr ValueDefBuilder::setOptionsStatic(const std::vector<T> &options)
{
    assert(m_valueDef);
    if (!options.empty() && m_valueDef->m_valueTemplate.type() == UnionValue::GetType(options.front())) {
        m_valueDef->m_options->m_options.clear();
        for (const auto& option: options)
        {
            m_valueDef->m_options->m_options.push_back(option);
        }
    } else {
        assert(false);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
template<typename T>
ValueDefBuilderSPtr ValueDefBuilder::setOptionsExcludedStatic(const std::vector<T> &options)
{
    assert(m_valueDef);
    if (!options.empty() && m_valueDef->m_valueTemplate.type() == UnionValue::GetType(options.front())) {
        m_valueDef->m_options->m_excluded.clear();
        for (const auto& option: options)
        {
            m_valueDef->m_options->m_excluded.push_back(option);
        }
    } else {
        assert(false);
    }
    return m_thisWPtr.lock();
}

//// =============================================================================
//// (public)
//template<typename T>
//ValueDefUPtr ValueDefBuilder::Make(T valueTemplate, const std::string &name, const std::string &displayName)
//{
//    ValueDefUPtr valueDef = ValueDef::MakeUPtr(valueTemplate);
//    valueDef->m_name = name;
//    valueDef->m_displayName = displayName;
//    valueDef->m_defaultConversion = Conversion::globalDefault();

//#ifdef XML_BACKEND
//    if (XML::Element::validateTagName(name)) {
//        valueDef->m_valueRef = XML::ValueRef::MakeUPtr("", XML::ChildRef::MakeUPtr(name));
//    }
//#endif // XML_BACKEND

//    return std::move(valueDef);
//}

//// =============================================================================
//// (public)
//template<typename T>
//ValueDefUPtr ValueDefBuilder::Make(T valueTemplate, const std::string &name, const std::string &displayName, T defaultValue)
//{
//    auto vDef = Make(valueTemplate, name, displayName);
//    vDef->m_defaultValue = defaultValue;
//    return std::move(vDef);
//}

//#ifdef XML_BACKEND
//// =============================================================================
//// (public)
//template<typename T>
//ValueDefUPtr ValueDefBuilder::MakeXML(T valueTemplate, const std::string &elementRef, const std::string &attributeName)
//{
//    ValueDefUPtr valueDef = ValueDef::MakeUPtr(valueTemplate);
//    valueDef->m_defaultConversion = Conversion::globalDefault();

//    valueDef->m_valueRef = XML::RefFactory::MakeValueRef(elementRef, attributeName);
//    valueDef->m_name = generateValueId(valueDef->m_valueRef->elementRef()->lastTagName(), attributeName);

//    return std::move(valueDef);
//}

//// =============================================================================
//// (public)
//template<typename T>
//ValueDefUPtr ValueDefBuilder::MakeXML(T valueTemplate, const std::string &elementRef, const std::string &attributeName, T defaultValue)
//{
//    auto vDef = MakeXML(valueTemplate, elementRef, attributeName);
//    vDef->m_defaultValue = defaultValue;
//    return std::move(vDef);
//}
//#endif // XML_BACKEND

//// =============================================================================
//// (public)
//template<typename T>
//static bool ValueDefBuilder::setDefaultValue(ValueDefUPtr &valueDef, T defaultValue)
//{
//    if (!valueDef) { return false; }

//    valueDef->m_defaultValue = defaultValue;

//    return true;
//}

//// =============================================================================
//// (public)
//template<typename T>
//static bool ValueDefBuilder::addStaticOption(ValueDef &valueDef, T option)
//{
//    if (valueDef.valueType() == UnionValue::GetType(option)) {
//        if (!valueDef.m_options) {
//            valueDef.m_options = new ValueOptions();
//        }
//        valueDef.m_options->m_options.push_back(option);
//        return true;
//    }
//    return false;
//}

//// =============================================================================
//// (public)
//template<typename T>
//static bool ValueDefBuilder::setStaticOptions(const ValueDefUPtr& valueDef, const std::vector<T> &options)
//{
//    if (!options.empty() && valueDef->m_valueTemplate.type() == UnionValue::GetType(options.front())) {
//        if (!valueDef->m_options) {
//            valueDef->m_options = new ValueOptions();
//        }
//        valueDef->m_options->m_options.clear();
//        for (const auto& option: options)
//        {
//            valueDef->m_options->m_options.push_back(option);
//        }
//        return true;
//    }
//    return false;
//}

//// =============================================================================
//// (public)
//template<typename T>
//static bool ValueDefBuilder::setStaticOptionsExcluded(const ValueDefUPtr& valueDef, const std::vector<T> &options)
//{
//    if (!options.empty() && valueDef->m_valueTemplate.isBaseTypeEqual(options.front())) {
//        if (!valueDef->m_options) {
//            valueDef->m_options = new ValueOptions();
//        }
//        valueDef->m_options->m_excluded.clear();
//        for (const auto& option: options)
//        {
//            valueDef->m_options->m_excluded.push_back(option);
//        }
//        return true;
//    }
//    return false;
//}

typedef ValueDefBuilder VDB;

} // namespace Model
} // namespace Oak
