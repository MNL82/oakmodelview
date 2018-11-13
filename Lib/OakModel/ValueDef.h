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

#include "UnionRef.h"
#include "UnionValue.h"

#ifdef XML_BACKEND
#include "XMLValueRef.h"
#endif // XML_BACKEND


namespace Oak::Model {

class ValueOptions;

class ValueDef;
typedef std::unique_ptr<ValueDef> ValueDefUPtr;
typedef std::unique_ptr<ValueDef> ValueDefUPtr;

// =============================================================================
// Class definition
// =============================================================================
class ValueDef
{
public:
    ValueDef(const UnionRef& valueTemplate);
    ValueDef(UnionType type);

    ValueDef(const ValueDef &copy);
    ValueDef(ValueDef &&move);

    virtual ValueDefUPtr copy() const;

    virtual ~ValueDef();

    UnionType valueType() const;
    const UnionRef valueTemplate() const;
    const std::string &name() const;
    const std::string &displayName() const;
    const std::string &tooltip() const;

    bool isNull() const;
    ConversionSPtr defaultConversion() const;

    const ValueSettings& settings() const;

    const ValueOptions &options() const;

    virtual int compareValue(const Node &_node, const UnionRef& value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    virtual bool hasValue(const Node &_node) const;
    virtual bool canGetValue(const Node &_node, const UnionRef& value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual bool getValue(const Node &_node, UnionRef value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual UnionValue value(const Node &_node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual std::string toString(const Node &_node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    template<typename T>
    T value(const Node & _node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    // TODO: Different error states
    virtual bool canSetValue(const Node &_node, const UnionRef& value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual bool setValue(const Node &_node, const UnionRef& value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    virtual bool hasDefaultValue() const;
    virtual const UnionRef defaultValue() const;
    virtual bool getDefaultValue(const UnionRef& value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    // Service functions
    template<class... _Types> inline
    static ValueDefUPtr MakeUPtr(_Types&&... _Args);

    static ValueDef &emptyDef();

protected:
    UnionValue m_valueTemplate;
    ValueSettings m_settings;
    std::string m_name;
    std::string m_displayName;
    UnionValue m_defaultValue;
    ConversionSPtr m_defaultConversion;
    ValueOptions *m_options;
    std::string m_tooltip;
    //std::vector<Variant> m_options;


#ifdef XML_BACKEND
    XML::ValueRefUPtr m_valueRef;
#endif // XML_BACKEND

    //TODO: Have a optional validator class
    // - Values only list
    // - Values excluded list
    // - Value filter

    static ValueDef s_emptyDef;

    friend class ValueDefBuilder;
};

// =============================================================================
// (public)
template<typename T>
T ValueDef::value(const Node &_node, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    T val;
    getValue(_node, val, useDefault, allowConversion, conversion);
    return std::move(val);
}

// =============================================================================
// (public)
template<class... _Types>
ValueDefUPtr ValueDef::MakeUPtr(_Types&&... _Args)
{
    return (ValueDefUPtr(new ValueDef(_STD forward<_Types>(_Args)...)));
}

typedef ValueDef ValueDef;

} // namespace Oak::Model

