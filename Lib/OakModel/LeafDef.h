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

#include "LeafSettings.h"

#include "NodeData.h"

#include "UnionRef.h"
#include "UnionValue.h"

#ifdef XML_BACKEND
#include "XMLValueRef.h"
#endif // XML_BACKEND


namespace Oak::Model {

class ValueOptions;

class LeafDef;
typedef std::unique_ptr<LeafDef> LeafDefUPtr;

// =============================================================================
// Class definition
// =============================================================================
class LeafDef
{
public:
    LeafDef(const UnionRef& valueTemplate);
    LeafDef(UnionType type);

    LeafDef(const LeafDef &copy);
    LeafDef(LeafDef &&move);

    virtual LeafDefUPtr copy() const;

    virtual ~LeafDef();

    UnionType valueType() const;
    const UnionRef valueTemplate() const;
    const std::string &name() const;
    const std::string &displayName() const;
    const std::string &tooltip() const;

    bool isNull() const;
    ConversionSPtr defaultConversion() const;

    const LeafSettings& settings() const;

    const ValueOptions &options() const;

    virtual int compareValue(const NodeData &_node, const UnionRef& value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    virtual bool hasValue(const NodeData &_node) const;
    virtual bool canGetValue(const NodeData &_node, const UnionRef& value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual bool getValue(const NodeData &_node, UnionRef value, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual UnionValue value(const NodeData &_node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual std::string toString(const NodeData &_node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    template<typename T>
    T value(const NodeData & _node, bool useDefault = true, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    // TODO: Different error states
    virtual bool canSetValue(const NodeData &_node, const UnionRef& value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;
    virtual bool setValue(const NodeData &_node, const UnionRef& value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    virtual bool hasDefaultValue() const;
    virtual const UnionRef defaultValue() const;
    virtual bool getDefaultValue(const UnionRef& value, bool allowConversion = false, ConversionSPtr conversion = ConversionSPtr()) const;

    // Service functions
    template<class... _Types> inline
    static LeafDefUPtr MakeUPtr(_Types&&... _Args);

    static LeafDef &emptyDef();

protected:
    UnionValue m_valueTemplate;
    LeafSettings m_settings;
    std::string m_name;
    std::string m_displayName;
    UnionValue m_defaultValue;
    ConversionSPtr m_defaultConversion;
    ValueOptions *m_options;
    std::string m_tooltip;


#ifdef XML_BACKEND
    XML::ValueRefUPtr m_valueRef;
#endif // XML_BACKEND

    static LeafDef s_emptyDef;

    friend class LeafDefBuilder;
};

// =============================================================================
// (public)
template<typename T>
T LeafDef::value(const NodeData &_node, bool useDefault, bool allowConversion, ConversionSPtr conversion) const
{
    T val;
    getValue(_node, val, useDefault, allowConversion, conversion);
    return std::move(val);
}

// =============================================================================
// (public)
template<class... _Types>
LeafDefUPtr LeafDef::MakeUPtr(_Types&&... _Args)
{
    return (LeafDefUPtr(new LeafDef(_STD forward<_Types>(_Args)...)));
}

typedef LeafDef LeafDef;

} // namespace Oak::Model

