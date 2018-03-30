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
// (protected)
ValueDefBuilder::ValueDefBuilder(UnionType type, const std::string &name)
{
    m_valueDefUPtr = ValueDef::MakeUPtr(type);
    m_valueDef = m_valueDefUPtr.get();
    m_valueDef->m_name = name;
    m_valueDef->m_defaultConversion = Conversion::globalDefault();

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(name)) {
        m_valueDef->m_valueRef = XML::ValueRef::MakeUPtr("", XML::ChildRef::MakeUPtr(name));
    }
#endif // XML_BACKEND
}

// =============================================================================
// (protected)
ValueDefBuilder::ValueDefBuilder(ValueDef *valueDef)
{
    m_valueDef = valueDef;
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::create(UnionType type, const std::string &name)
{
    ValueDefBuilderSPtr sPtr(new ValueDefBuilder(type, name));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::use(ValueDef *valueDef)
{
    ValueDefBuilderSPtr sPtr = ValueDefBuilderSPtr(new ValueDefBuilder(valueDef));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setName(const std::string &value)
{
    assert(m_valueDef);
    m_valueDef->m_name = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setDisplayName(const std::string &value)
{
    assert(m_valueDef);
    m_valueDef->m_displayName = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setTooltip(const std::string &value)
{
    assert(m_valueDef);
    m_valueDef->m_tooltip = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setDefaultValue(const UnionRef &value)
{
    assert(m_valueDef);
    m_valueDef->m_defaultValue = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setDefaultConversion(ConversionSPtr value)
{
    assert(m_valueDef);
    assert(value);
    m_valueDef->m_defaultConversion = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::addOptionStatic(const UnionRef &value)
{
    assert(m_valueDef);
    if (m_valueDef->valueType() == UnionValue::GetType(value)) {
        m_valueDef->m_options->m_options.push_back(value);
    } else {
        assert(false);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setOptionsQuery(QueryRefSPtr value)
{
    assert(m_valueDef);
    m_valueDef->m_options->m_query = value;

    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setOptionsExcludedQuery(QueryRefSPtr value)
{
    assert(m_valueDef);
    m_valueDef->m_options->m_queryExcluded = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setSetting(std::string name, bool value)
{
    assert(m_valueDef);
    if (name == "OptionsOnly") {
        m_valueDef->m_settings.setOptionsOnly(value);
    } else if (name == "Required") {
        m_valueDef->m_settings.setRequired(value);
    } else if (name == "Unique") {
        m_valueDef->m_settings.setUnique(value);
    } else {
        assert(false);
    }
    return m_thisWPtr.lock();
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
ValueDefBuilderSPtr ValueDefBuilder::setXMLValueRef(XML::ValueRefUPtr value)
{
    assert(m_valueDef);
    assert(value);
    m_valueDef->m_valueRef = std::move(value);
    return m_thisWPtr.lock();
}
#endif

// =============================================================================
// (public)
const ValueDef &ValueDefBuilder::valueDef() const
{
    assert(m_valueDef);
    return *m_valueDef;
}

// =============================================================================
// (public)
ValueDefUPtr ValueDefBuilder::get()
{
    assert(m_valueDefUPtr);
    return std::move(m_valueDefUPtr);
}

//// =============================================================================
//// (public)
//bool ValueDefBuilder::setName(ValueDefUPtr& valueDef, const std::string &valueName)
//{
//    if (!valueDef) { return false; }

//    valueDef->m_name = valueName;

//    return true;
//}

//// =============================================================================
//// (public)
//bool ValueDefBuilder::setTooltip(ValueDefUPtr &valueDef, const std::string &tooltip)
//{
//    if (!valueDef) { return false; }

//    valueDef->m_tooltip = tooltip;

//    return true;
//}

//// =============================================================================
//// (public)
//bool ValueDefBuilder::setDefaultConversion(const ValueDefUPtr& valueDef, ConversionSPtr conversion)
//{
//    if (!valueDef) { return false; }
//    if (!conversion) { return false; }

//    valueDef->m_defaultConversion = conversion;

//    return true;
//}

//// =============================================================================
//// (public)
//bool ValueDefBuilder::setOptionsQuery(const ValueDefUPtr& valueDef, QueryRefSPtr queryRef)
//{
//    if (!valueDef) { return false; }

//    if (!valueDef->m_options) {
//        valueDef->m_options = new ValueOptions();
//    }
//    valueDef->m_options->m_query = queryRef;

//    return true;
//}

//// =============================================================================
//// (public)
//bool ValueDefBuilder::setOptionsExcludedQuery(const ValueDefUPtr& valueDef, QueryRefSPtr queryRef)
//{
//    if (!valueDef) { return false; }

//    if (!valueDef->m_options) {
//        valueDef->m_options = new ValueOptions();
//    }
//    valueDef->m_options->m_queryExcluded = queryRef;

//    return true;
//}

//// =============================================================================
//// (public)
//ValueSettings &ValueDefBuilder::settings(const ValueDefUPtr &valueDef)
//{
//    return valueDef->m_settings;
//}

//// =============================================================================
//// (public)
//bool ValueDefBuilder::setValueRef(const ValueDefUPtr& valueDef, XML::ValueRefUPtr valueRef)
//{
//    if (!valueDef) { return false; }
//    if (!valueRef) { return false; }

//    valueDef->m_valueRef = std::move(valueRef);

//    return true;
//}

//// =============================================================================
//// (public)
//std::string ValueDefBuilder::generateValueId(std::string tagName, std::string attributeName)
//{
//    // The value id is created from the tag name and the attribute name
//    if (tagName.empty()) {
//        if (attributeName.empty()) {
//            return std::string("Body");
//        } else {
//            return attributeName;
//        }
//    } else {
//        if (attributeName.empty()) {
//            return tagName;
//        } else {
//            return tagName + "[" + attributeName + "]";
//        }
//    }
//}

} // namespace Model
} // namespace Oak
