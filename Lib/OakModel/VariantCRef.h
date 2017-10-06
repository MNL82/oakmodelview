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

#include "Visitor.h"
#include "Variant.h"

namespace Oak {
namespace Model {

class VariantRef;

// =============================================================================
// Class definition
// =============================================================================
class VariantCRef
{
public:
    VariantCRef();
#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
    VariantCRef(const char* value);
#endif // TDM_USE_CHAR_PTR_IN_VARIANT
    template<typename T>
    VariantCRef(const T& value);
    VariantCRef(const Variant& variant);
    VariantCRef(const VariantRef& variant);

    VariantCRef(const VariantCRef& copy);

    template<typename T>
    bool operator==(const T& value) const;
    bool operator==(const VariantCRef& value) const;

    template<typename T>
    bool operator!=(const T& value) const;
    bool operator!=(const VariantCRef& value) const;

    bool operator<(const VariantCRef& value) const;

    template<typename T>
    bool isEqual(const T& value, bool allowConversion = true, Conversion* properties = nullptr) const;
    bool isEqual(const VariantCRef& value, bool allowConversion = true, Conversion* properties = nullptr) const;

    bool isBaseTypeEqual(const VariantCRef& value) const;

    bool isNull() const;

    const std::type_info& typeId() const;
    int witch() const;

    template<typename T>
    bool canGet(T& value, bool allowConversion = true, Conversion* properties = nullptr) const;
    bool canGet(VariantRef& value, bool allowConversion = true, Conversion* properties = nullptr) const;

    template<typename T>
    bool get(T& value, bool allowConversion = true, Conversion* properties = nullptr) const;
    bool get(VariantRef& value, bool allowConversion = true, Conversion* properties = nullptr) const;

    template<class T>
    const T& valueCRef() const;

    Variant value() const;

private:
    BaseTypeCRef m_value;

    friend class Variant;
    friend class VariantRef;
};

// =============================================================================
// (public)
template<typename T>
VariantCRef::VariantCRef(const T& value)
    : m_value(value)
{
}

// =============================================================================
// (public)
template<typename T>
bool VariantCRef::operator==(const T& value) const
{
    return boost::apply_visitor(EqualVisitor(), m_value, BaseTypeCRef(value));
}

// =============================================================================
// (public)
template<typename T>
bool VariantCRef::operator!=(const T& value) const
{
    return !boost::apply_visitor(EqualVisitor(), m_value, BaseTypeCRef(value));
}

// =============================================================================
// (public)
template<typename T>
bool VariantCRef::isEqual(const T& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(EqualVisitor(allowConversion, properties), m_value,  BaseTypeCRef(value));
}

// =============================================================================
// (public)
template<typename T>
bool VariantCRef::canGet(T& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(CanConvertVisitor(allowConversion, properties), BaseType(value), m_value);
}

// =============================================================================
// (public)
template<typename T>
bool VariantCRef::get(T& value, bool allowConversion, Conversion* properties) const
{
    return boost::apply_visitor(ConvertVisitor(allowConversion, properties), BaseTypeRef(value), m_value);
}

// =============================================================================
// (public)
template<class T>
const T& VariantCRef::valueCRef() const
{
//    static T v;
//    return v;
    if (m_value.which() == 9) {
        const T* vPtr = boost::get<const T*>(m_value);
        return *vPtr;
    } else {
        const T& vRef = boost::get<const T&>(m_value);
        return vRef;
    }
}

} // namespace Model
} // namespace Oak
