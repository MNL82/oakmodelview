/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "LeafDefBuilder.h"

#include "XMLChildRef.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (protected)
LeafDefBuilder::LeafDefBuilder(UnionType type, const std::string &name)
{
    m_leafDefUPtr = LeafDef::MakeUPtr(type);
    m_leafDef = m_leafDefUPtr.get();
    m_leafDef->m_name = name;
    m_leafDef->m_defaultConversion = Conversion::globalDefault();

#ifdef XML_BACKEND
    if (XML::Element::validateTagName(name)) {
        m_leafDef->m_valueRef = XML::ValueRef::MakeUPtr("", XML::ChildRef::MakeUPtr(name));
    }
#endif // XML_BACKEND
}

// =============================================================================
// (protected)
LeafDefBuilder::LeafDefBuilder(LeafDef *leafDef)
{
    m_leafDef = leafDef;
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::create(UnionType type, const std::string &name)
{
    LeafDefBuilderSPtr sPtr(new LeafDefBuilder(type, name));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::use(LeafDef *leafDef)
{
    LeafDefBuilderSPtr sPtr = LeafDefBuilderSPtr(new LeafDefBuilder(leafDef));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setName(const std::string &value)
{
    ASSERT(m_leafDef);
    m_leafDef->m_name = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setDisplayName(const std::string &value)
{
    ASSERT(m_leafDef);
    m_leafDef->m_displayName = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setTooltip(const std::string &value)
{
    ASSERT(m_leafDef);
    m_leafDef->m_tooltip = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setDefaultValue(const UnionRef &value)
{
    ASSERT(m_leafDef);
    m_leafDef->m_defaultValue = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setDefaultConversion(ConversionSPtr value)
{
    ASSERT(m_leafDef);
    ASSERT(value);
    m_leafDef->m_defaultConversion = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::addOptionStatic(const UnionRef &value)
{
    ASSERT(m_leafDef);
    if (m_leafDef->valueType() == UnionValue::GetType(value)) {
        m_leafDef->m_options->m_options.push_back(value);
    } else {
        ASSERT(false);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setOptionsStatic(const std::vector<UnionValue>& options)
{
    assert(m_leafDef);
    if (!options.empty() && m_leafDef->m_valueTemplate.type() == UnionValue::GetType(options.front())) {
        m_leafDef->m_options->m_options.resize(options.size());
        auto it = m_leafDef->m_options->m_options.begin();
        for (const auto& option: options)
        {
            if (option.type() == m_leafDef->m_valueTemplate.type()) { // No conversion needed
                *it = option;
            } else { // Converting the value to that on the LeafDef
                *it = UnionValue(m_leafDef->m_valueTemplate.type());
                it->get(option);
            }
            it++;
        }
    } else {
        assert(false);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setOptionsExcludedStatic(const std::vector<UnionValue>& options)
{
    assert(m_leafDef);
    if (!options.empty() && m_leafDef->m_valueTemplate.type() == UnionValue::GetType(options.front())) {
        m_leafDef->m_options->m_excluded.resize(options.size());
        auto it = m_leafDef->m_options->m_excluded.begin();
        for (const auto& option: options)
        {
            if (option.type() == m_leafDef->m_valueTemplate.type()) { // No conversion needed
                *it = option;
            } else { // Converting the value to that on the LeafDef
                *it = UnionValue(m_leafDef->m_valueTemplate.type());
                it->get(option);
            }
            it++;
        }
    } else {
        assert(false);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setOptionsQuery(LeafQuerySPtr value)
{
    ASSERT(m_leafDef);
    m_leafDef->m_options->m_query = value;

    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setOptionsExcludedQuery(LeafQuerySPtr value)
{
    ASSERT(m_leafDef);
    m_leafDef->m_options->m_queryExcluded = value;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setSetting(std::string name, const UnionRef &value)
{
    ASSERT(m_leafDef);
    m_leafDef->m_settings.setValue(name, value);
    return m_thisWPtr.lock();
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
LeafDefBuilderSPtr LeafDefBuilder::setXMLValueRef(XML::ValueRefUPtr value)
{
    ASSERT(m_leafDef);
    ASSERT(value);
    m_leafDef->m_valueRef = std::move(value);
    return m_thisWPtr.lock();
}
#endif

// =============================================================================
// (public)
const LeafDef &LeafDefBuilder::leafDef() const
{
    ASSERT(m_leafDef);
    return *m_leafDef;
}

// =============================================================================
// (public)
LeafDefUPtr LeafDefBuilder::get()
{
    ASSERT(m_leafDefUPtr);
    return std::move(m_leafDefUPtr);
}

//// =============================================================================
//// (public)
//bool LeafDefBuilder::setName(LeafDefUPtr& leafDef, const std::string &valueName)
//{
//    if (!leafDef) { return false; }

//    leafDef->m_name = valueName;

//    return true;
//}

//// =============================================================================
//// (public)
//bool LeafDefBuilder::setTooltip(LeafDefUPtr &leafDef, const std::string &tooltip)
//{
//    if (!leafDef) { return false; }

//    leafDef->m_tooltip = tooltip;

//    return true;
//}

//// =============================================================================
//// (public)
//bool LeafDefBuilder::setDefaultConversion(const LeafDefUPtr& leafDef, ConversionSPtr conversion)
//{
//    if (!leafDef) { return false; }
//    if (!conversion) { return false; }

//    leafDef->m_defaultConversion = conversion;

//    return true;
//}

//// =============================================================================
//// (public)
//bool LeafDefBuilder::setOptionsQuery(const LeafDefUPtr& leafDef, QueryRefSPtr queryRef)
//{
//    if (!leafDef) { return false; }

//    if (!leafDef->m_options) {
//        leafDef->m_options = new ValueOptions();
//    }
//    leafDef->m_options->m_query = queryRef;

//    return true;
//}

//// =============================================================================
//// (public)
//bool LeafDefBuilder::setOptionsExcludedQuery(const LeafDefUPtr& leafDef, QueryRefSPtr queryRef)
//{
//    if (!leafDef) { return false; }

//    if (!leafDef->m_options) {
//        leafDef->m_options = new ValueOptions();
//    }
//    leafDef->m_options->m_queryExcluded = queryRef;

//    return true;
//}

//// =============================================================================
//// (public)
//LeafSettings &LeafDefBuilder::settings(const LeafDefUPtr &leafDef)
//{
//    return leafDef->m_settings;
//}

//// =============================================================================
//// (public)
//bool LeafDefBuilder::setValueRef(const LeafDefUPtr& leafDef, XML::ValueRefUPtr valueRef)
//{
//    if (!leafDef) { return false; }
//    if (!valueRef) { return false; }

//    leafDef->m_valueRef = std::move(valueRef);

//    return true;
//}

//// =============================================================================
//// (public)
//std::string LeafDefBuilder::generateValueId(std::string tagName, std::string attributeName)
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

} // namespace Oak::Model

