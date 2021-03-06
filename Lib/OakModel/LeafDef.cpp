/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "LeafDef.h"

#include <algorithm>

#include "ValueOptions.h"
#include "XMLChildRef.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

LeafDef LeafDef::s_emptyDef = LeafDef(UnionRef());

// =============================================================================
// (public)
LeafDef::LeafDef(const UnionRef &valueTemplate)
    : m_valueTemplate(valueTemplate), m_options(new ValueOptions())
{

}

// =============================================================================
// (public)
LeafDef::LeafDef(UnionType type)
    : m_valueTemplate(type), m_options(new ValueOptions())
{

}

// =============================================================================
// (public)
LeafDef::LeafDef(const LeafDef &copy)
    : m_valueTemplate(copy.m_valueTemplate)
{
    m_defaultValue = copy.m_defaultValue;
    m_name = copy.m_name;
    m_defaultConversion = copy.m_defaultConversion;
    m_options = new ValueOptions(*copy.m_options);

#ifdef XML_BACKEND
    m_valueRef = copy.m_valueRef->copy();
#endif // XML_BACKEND
}

// =============================================================================
// (public)
LeafDef::LeafDef(LeafDef&& move)
    : m_valueTemplate(move.m_valueTemplate)
{
    m_defaultValue = std::move(move.m_defaultValue);
    m_name = std::move(move.m_name);
    m_defaultConversion = move.m_defaultConversion;
    m_options = move.m_options;
    move.m_options = nullptr;

#ifdef XML_BACKEND
    m_valueRef = std::move(move.m_valueRef);
#endif // XML_BACKEND
}

// =============================================================================
// (public)
LeafDefUPtr LeafDef::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
LeafDef::~LeafDef()
{
    if (m_options) {
        delete m_options;
        m_options = nullptr;
    }
}

// =============================================================================
// (public)
UnionType LeafDef::valueType() const
{
    return m_valueTemplate.type();
}

// =============================================================================
// (public)
const UnionRef LeafDef::valueTemplate() const
{
    return m_valueTemplate;
}

// =============================================================================
// (public)
const std::string &LeafDef::name() const
{
    return m_name;
}

// =============================================================================
// (public)
const std::string &LeafDef::displayName() const
{
    if (m_displayName.empty()) {
        return m_name;
    } else {
        return m_displayName;
    }
}

// =============================================================================
// (public)
const std::string &LeafDef::tooltip() const
{
    return m_tooltip;
}

// =============================================================================
// (public)
bool LeafDef::isNull() const
{
    return m_valueTemplate.isNull();
}

// =============================================================================
// (public)
ConversionSPtr LeafDef::defaultConversion() const
{
    return m_defaultConversion;
}

// =============================================================================
// (public)
const LeafSettings& LeafDef::settings() const
{
    return m_settings;
}

// =============================================================================
// (public)
const ValueOptions &LeafDef::options() const
{
    if (m_options) {
        return *m_options;
    } else {
        return ValueOptions::empty();
    }
}

// =============================================================================
// (public)
int LeafDef::compareValue(const NodeData &_node, const UnionRef &value, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    if (_node.isNull()) { return -2; }

    if (!conversion) { conversion = m_defaultConversion; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML: {
        if (m_valueTemplate.type() == UnionType::String) {
            if (value.type() == UnionType::String) {
                int result = m_valueRef->compareValue(_node.xmlNode(), value.getCString());
                if (useDefault && hasDefaultValue() && result == -2) {
                    // Value is not defined in the XML
                    return m_defaultValue.getCString().compare(value.getCString());
                }
                return result;
            } else if (allowConversion) {
                std::string tempStr;
                if (value.get(tempStr, true, conversion.get())) {
                    return m_valueRef->compareValue(_node.xmlNode(), tempStr);
                }
            }
            return -2;
        } else {
            // Values needs to be compared after they are converted to its value type
            // Read the value stored in the xml document
            UnionValue tempValue(m_valueTemplate);
            UnionRef tempRef(tempValue);
            if (getValue(_node.xmlNode(), tempRef, useDefault, allowConversion, conversion)) {
                // Compare the to values
                return tempValue.isEqual(value, allowConversion, conversion.get()) ? 0 : 1;
            }
        }
        break;
    }
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }

    return -2;
}

// =============================================================================
// (public)
bool LeafDef::hasValue(const NodeData &_node) const
{
    if (_node.isNull()) { return false; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML: {
        return m_valueRef->hasValue(_node.xmlNode());
    }
#endif // XML_BACKEND
    case NodeData::Type::UNDEFINED:
        break;
    }

    // _node.type() returns an unhandled type that needs to be implemented
    ASSERT(false);
    return false;
}

// =============================================================================
// (public)
bool LeafDef::canGetValue(const NodeData &_node, const UnionRef &value, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    if (_node.isNull()) { return false; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML: {
        UnionValue tempValue(value);
        UnionRef tempRef(tempValue);
        return getValue(_node.xmlNode(), tempRef, useDefault, allowConversion, (conversion) ? conversion : m_defaultConversion);
    }
#endif // XML_BACKEND
    case NodeData::Type::UNDEFINED:
        break;
    }

    // _node.type() returns an unhandled type that needs to be implemented
    ASSERT(false);
    return false;
}

// =============================================================================
// (public)
bool LeafDef::getValue(const NodeData &_node, UnionRef value, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    if (_node.isNull()) { return false; }
    if (value.isNull()) { return false; }

    if (!conversion) { conversion = m_defaultConversion; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML: {
        if (m_valueRef->hasValue(_node.xmlNode())) {
            // Check if the returned value type is string or string list
            // Strings can always be returned
            if (value.type() == UnionType::String) {
                m_valueRef->getValue(_node.xmlNode(), value.getString());
            } else {
                std::string string;
                m_valueRef->getValue(_node.xmlNode(), string);
                UnionRef(string).get(value);
                //value.set(string, true, conversion.get());
            }
            return true;
        }
        if (useDefault && hasDefaultValue()) {
            m_defaultValue.get(value, allowConversion, conversion.get());
            return true;
        } else {
            return false;
        }
    }
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        ASSERT(false);
    }
    return false;
}

// =============================================================================
// (public)
UnionValue LeafDef::value(const NodeData &_node, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    UnionValue val(m_valueTemplate);
    UnionRef uRef(val);
    getValue(_node, uRef, useDefault, allowConversion, conversion);
    return val;
}

// =============================================================================
// (public)
std::string LeafDef::toString(const NodeData &_node, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    std::string str;
    UnionRef uRef(str);
    getValue(_node, uRef, useDefault, allowConversion, conversion);
    return str;
}

// =============================================================================
// (public)
bool LeafDef::canSetValue(const NodeData &_node, const UnionRef &value, bool allowConversion, ConversionSPtr conversion) const
{
    if (_node.isNull()) { return false; }
    if (value.isNull()) { return false; }

    if (!conversion) { conversion = m_defaultConversion; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML: {
        if (value.type() == m_valueTemplate.type()) {
            if (m_valueTemplate.type() == UnionType::String) {
                return true;
            } else {
                std::string tempStr;
                if (value.get(tempStr, true, conversion.get())) {
                    return true;
                }
            }
        } else if (allowConversion) {
            UnionValue tempVariant(m_valueTemplate);
            if (value.get(tempVariant, allowConversion, conversion.get())) {
                std::string tempStr;
                if (value.canGet(tempStr, true, conversion.get())) {
                    return true;
                }
            }
        }
        return false;
    }
#endif // XML_BACKEND
    case NodeData::Type::UNDEFINED:
        break;
    }
    // _node.type() returns an unhandled type that needs to be implemented
    ASSERT(false);

    return false;
}

// =============================================================================
// (public)
bool LeafDef::setValue(const NodeData &_node, const UnionRef &value, bool allowConversion, ConversionSPtr conversion) const
{
    if (value.isNull()) { return false; }
    if (_node.isNull()) { return false; }

    if (!conversion) { conversion = m_defaultConversion; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case NodeData::Type::XML: {
        if (value.type() == m_valueTemplate.type()) {
            if (m_valueTemplate.type() == UnionType::String) {
                return m_valueRef->setValue(_node.xmlNode(), value.getCString());
            } else {
                std::string tempStr;
                if (value.get(tempStr, true, conversion.get())) {
                    return m_valueRef->setValue(_node.xmlNode(), tempStr);
                }
            }
        } else if (allowConversion) {
            UnionValue tempVariant(m_valueTemplate);
            if (value.get(tempVariant, allowConversion, conversion.get())) {
                std::string tempStr;
                if (tempVariant.get(tempStr, true, conversion.get())) {
                    return m_valueRef->setValue(_node.xmlNode(), tempStr);
                }
            }
        }
        return false;
    }
#endif // XML_BACKEND
    case NodeData::Type::UNDEFINED:
        break;
    }
    // _node.type() returns an unhandled type that needs to be implemented
    ASSERT(false);
    return false;
}

// =============================================================================
// (public)
bool LeafDef::hasDefaultValue() const
{
    return !m_defaultValue.isNull();
}

// =============================================================================
// (public)
const UnionRef LeafDef::defaultValue() const
{
    return m_defaultValue;
}

// =============================================================================
// (public)
bool LeafDef::getDefaultValue(const UnionRef &value, bool allowConversion, ConversionSPtr conversion) const
{
    return m_defaultValue.get(value, allowConversion, (conversion) ? conversion.get() : m_defaultConversion.get());
}

// =============================================================================
// (public)
LeafDef &LeafDef::emptyDef()
{
    return s_emptyDef;
}

} // namespace Oak::Model

