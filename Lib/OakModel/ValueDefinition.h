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

#include "ValueSettings.h"

#include "Node.h"

#include "VariantRef.h"
#include "VariantCRef.h"

#ifdef XML_BACKEND
#include "XMLValueRef.h"
#endif // XML_BACKEND

namespace Oak {
namespace Model {

class ValueOptions;

class ValueDefinition;
typedef std::unique_ptr<ValueDefinition> ValueDefinitionUPtr;
typedef std::unique_ptr<ValueDefinition> ValueDefUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueDefinition
{
public:
    ValueDefinition(VariantCRef valueTemplate);

    ValueDefinition(const ValueDefinition &copy);
    ValueDefinition(ValueDefinition &&move);

    virtual ValueDefinitionUPtr copy() const;

    virtual ~ValueDefinition();

    const type_info& valueTypeId() const;
    VariantCRef valueTemplate() const;
    const std::string &name() const;
    const std::string &displayName() const;

    bool isNull() const;
    ConversionSPtr defaultConversion() const;

    const ValueSettings& settings() const;

    const ValueOptions &options() const;

    virtual int compareValue(Node _node, VariantCRef value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    virtual bool hasValue(Node _node) const;
    virtual bool canGetValue(Node _node, VariantRef value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual bool getValue(Node _node, VariantRef value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual Variant value(Node _node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual std::string toString(Node _node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    template<typename T>
    T value(Node _node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    // TODO: Different error states
    virtual bool canSetValue(Node _node, VariantCRef value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual bool setValue(Node _node, VariantCRef value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    virtual bool hasDefaultValue() const;
    virtual VariantCRef defaultValue() const;
    virtual bool getDefaultValue(VariantRef value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    // Service functions
    template<class... _Types> inline
    static typename ValueDefinitionUPtr MakeUPtr(_Types&&... _Args);

    static ValueDefinition &emptyDefinition();

protected:
    Variant m_valueTemplate;
    ValueSettings m_settings;
    std::string m_name;
    std::string m_displayName;
    Variant m_defaultValue;
    ConversionSPtr m_defaultConversion;
    ValueOptions *m_options;
    //std::vector<Variant> m_options;


#ifdef XML_BACKEND
    XML::ValueRefUPtr m_valueRef;
#endif // XML_BACKEND

    //TODO: Have a optional validator class
    // - Values only list
    // - Values excluded list
    // - Value filter

    static ValueDefinition s_emptyDefinition;

    friend class ValueDefinitionBuilder;
};

// =============================================================================
// (public)
template<typename T>
T ValueDefinition::value(Node _node, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    T val;
    getValue(_node, val, useDefault, allowConversion, conversion);
    return std::move(val);
}

// =============================================================================
// (public)
template<class... _Types> inline
static typename ValueDefinitionUPtr ValueDefinition::MakeUPtr(_Types&&... _Args)
{
    return (ValueDefinitionUPtr(new ValueDefinition(_STD forward<_Types>(_Args)...)));
}

typedef ValueDefinition ValueDef;

} // namespace Model
} // namespace Oak
