/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueDefinition.h"

#include "XMLChildRef.h"

#include "assert.h"

#include <algorithm>

namespace Oak {
namespace Model {

ValueDefinition ValueDefinition::s_emptyDefinition = ValueDefinition(InvalidVariant());

// =============================================================================
// (public)
ValueDefinition::ValueDefinition(VariantCRef valueTemplate)
    : m_valueTemplate(valueTemplate)
{

}

// =============================================================================
// (public)
ValueDefinition::ValueDefinition(const ValueDefinition &copy)
    : m_valueTemplate(copy.m_valueTemplate)
{
    m_defaultValue = copy.m_defaultValue;
    m_name = copy.m_name;
    m_defaultConversion = copy.m_defaultConversion;

#ifdef XML_BACKEND
    m_valueRef = copy.m_valueRef->copy();
#endif // XML_BACKEND
}

// =============================================================================
// (public)
ValueDefinition::ValueDefinition(ValueDefinition&& move)
    : m_valueTemplate(move.m_valueTemplate)
{
    m_defaultValue = std::move(move.m_defaultValue);
    m_name = std::move(move.m_name);
    m_defaultConversion = move.m_defaultConversion;

#ifdef XML_BACKEND
    m_valueRef = std::move(move.m_valueRef);
#endif // XML_BACKEND
}

// =============================================================================
// (public)
ValueDefinitionUPtr ValueDefinition::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
const std::type_info& ValueDefinition::valueTypeId() const
{
    return m_valueTemplate.typeId();
}

// =============================================================================
// (public)
VariantCRef ValueDefinition::valueTemplate() const
{
    return m_valueTemplate;
}

// =============================================================================
// (public)
const std::string &ValueDefinition::name() const
{
    return m_name;
}

// =============================================================================
// (public)
const std::string &ValueDefinition::displayName() const
{
    if (m_displayName.empty()) {
        return m_name;
    } else {
        return m_displayName;
    }
}

// =============================================================================
// (public)
bool ValueDefinition::isNull() const
{
    return m_valueTemplate.witch() == 0;
}

// =============================================================================
// (public)
ConversionSPtr ValueDefinition::defaultConversion() const
{
    return m_defaultConversion;
}

const ValueSettings& ValueDefinition::settings() const
{
    return m_settings;
}

// =============================================================================
// (public)
bool ValueDefinition::hasOptions() const
{
    return !m_options.empty();
}

// =============================================================================
// (public)
bool ValueDefinition::getOptions(std::vector<VariantCRef>& options) const
{
    options.clear();
    if (m_options.empty()) { return false; }

    for (const auto& option: m_options) {
        options.push_back(option);
    }
    return true;
}

// =============================================================================
// (public)
int ValueDefinition::compareValue(Node _node, VariantCRef value, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    if (_node.isNull()) { return -2; }

    if (!conversion) { conversion = m_defaultConversion; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML: {
        if (m_valueTemplate.typeId() == typeid(std::string)) {
            if (value.typeId() == typeid(std::string)) {
                int result = m_valueRef->compareValue(_node.xmlNode(), value.valueCRef<std::string>());
                if (useDefault && hasDefaultValue() && result == -2) {
                    // Value is not defined in the XML
                    return m_defaultValue.valueCRef<std::string>().compare(value.valueCRef<std::string>());
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
            Variant tempValue(m_valueTemplate);
            if (getValue(_node.xmlNode(), tempValue, useDefault, allowConversion, conversion)) {
                // Compare the to values
                return tempValue.isEqual(value, allowConversion, conversion.get()) ? 0 : 1;
            }
        }
        break;
    }
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }

    return -2;
}

// =============================================================================
// (public)
bool ValueDefinition::hasValue(Node _node) const
{
    if (_node.isNull()) { return false; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML: {
        return m_valueRef->hasValue(_node.xmlNode());
    }
#endif // XML_BACKENDvalue
    }

    // _node.type() returns an unhandled type that needs to be implemented
    assert(false);
    return false;
}

// =============================================================================
// (public)
bool ValueDefinition::canGetValue(Node _node, VariantRef value, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    if (_node.isNull()) { return false; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML: {
        Variant tempValue(value);
        return getValue(_node.xmlNode(), tempValue, useDefault, allowConversion, (conversion) ? conversion : m_defaultConversion);
    }
#endif // XML_BACKENDvalue
    }

    // _node.type() returns an unhandled type that needs to be implemented
    assert(false);
    return false;
}

// =============================================================================
// (public)
bool ValueDefinition::getValue(Node _node, VariantRef value, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    if (_node.isNull()) { return false; }
    if (value.isNull()) { return false; }

    if (!conversion) { conversion = m_defaultConversion; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML: {
        if (m_valueRef->hasValue(_node.xmlNode())) {
            // Check if the returned value type is string or string list
            // Strings can always be returned
            if (value.typeId() == typeid(std::string)) {
                m_valueRef->getValue(_node.xmlNode(), value.valueRef<std::string>());
            } else {
                std::string string;
                m_valueRef->getValue(_node.xmlNode(), string);
                value.set(string, true, conversion.get());
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
        assert(false);
    }
    return false;
}

// =============================================================================
// (public)
Variant ValueDefinition::value(Node _node, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    Variant val(m_valueTemplate);
    getValue(_node, val, useDefault, allowConversion, conversion);
    return std::move(val);
}

// =============================================================================
// (public)
std::string ValueDefinition::toString(Node _node, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    std::string str;
    getValue(_node, str, useDefault, allowConversion, conversion);
    return std::move(str);
}

// =============================================================================
// (public)
bool ValueDefinition::canSetValue(Node _node, VariantCRef value, bool allowConversion, ConversionSPtr conversion) const
{
    if (_node.isNull()) { return false; }
    if (value.isNull()) { return false; }

    if (!conversion) { conversion = m_defaultConversion; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML: {
        if (value.typeId() == m_valueTemplate.typeId()) {
            if (m_valueTemplate.typeId() == typeid(std::string)) {
                return true;
            } else {
                std::string tempStr;
                if (value.get(tempStr, true, conversion.get())) {
                    return true;
                }
            }
        } else if (allowConversion) {
            Variant tempVariant(m_valueTemplate);
            if (tempVariant.set(value, allowConversion, conversion.get())) {
                std::string tempStr;
                if (value.canGet(tempStr, true, conversion.get())) {
                    return true;
                }
            }
        }
    }
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }

    return false;
}

// =============================================================================
// (public)
bool ValueDefinition::setValue(Node _node, VariantCRef value, bool allowConversion, ConversionSPtr conversion) const
{
    if (value.isNull()) { return false; }
    if (_node.isNull()) { return false; }

    if (!conversion) { conversion = m_defaultConversion; }

    switch (_node.type()) {
#ifdef XML_BACKEND
    case Node::Type::XML: {
        if (value.typeId() == m_valueTemplate.typeId()) {
            if (m_valueTemplate.typeId() == typeid(std::string)) {
                return m_valueRef->setValue(_node.xmlNode(), value.valueCRef<std::string>());
            } else {
                std::string tempStr;
                if (value.get(tempStr, true, conversion.get())) {
                    return m_valueRef->setValue(_node.xmlNode(), tempStr);
                }
            }
        } else if (allowConversion) {
            Variant tempVariant(m_valueTemplate);
            if (tempVariant.set(value, allowConversion, conversion.get())) {
                std::string tempStr;
                if (tempVariant.get(tempStr, true, conversion.get())) {
                    return m_valueRef->setValue(_node.xmlNode(), tempStr);
                }
            }
        }
        return false;
    }
#endif // XML_BACKEND
    default:
        // _node.type() returns an unhandled type that needs to be implemented
        assert(false);
    }
    return false;
}

// =============================================================================
// (public)
bool ValueDefinition::hasDefaultValue() const
{
    return !m_defaultValue.isNull();
}

// =============================================================================
// (public)
VariantCRef ValueDefinition::defaultValue() const
{
    return m_defaultValue;
}

// =============================================================================
// (public)
bool ValueDefinition::getDefaultValue(VariantRef value, bool allowConversion, ConversionSPtr conversion) const
{
    return value.set(m_defaultValue, allowConversion, (conversion) ? conversion.get() : m_defaultConversion.get());
}

// =============================================================================
// (public)
ValueDefinition &ValueDefinition::emptyDefinition()
{
    return s_emptyDefinition;
}

} // namespace Model
} // namespace Oak
