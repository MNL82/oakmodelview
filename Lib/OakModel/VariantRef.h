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

class VariantCRef;

// =============================================================================
// Class definition
// =============================================================================
class VariantRef
{
public:
    template<typename T>
    VariantRef(T& value);
    VariantRef(Variant& variant);

    VariantRef(VariantRef& copy);

    template<typename T>
    bool operator==(const T& value) const;
    bool operator==(const VariantCRef& value) const;

    template<typename T>
    bool operator!=(const T& value) const;
    bool operator!=(const VariantCRef& value) const;

    template<typename T>
    bool isEqual(const T& value, bool allowConversion = true, Conversion* properties = nullptr) const;
    bool isEqual(const VariantCRef& value, bool allowConversion = true, Conversion* properties = nullptr) const;

    bool isNull() const;

    const std::type_info& typeId() const;
    int witch() const;

    template<typename T>
    bool canGet(T& value, bool allowConversion = true, Conversion* properties = nullptr) const;
    bool canGet(VariantRef& value, bool allowConversion = true, Conversion* properties = nullptr) const;

    template<typename T>
    bool get(T& value, bool allowConversion = true, Conversion* properties = nullptr) const;
    bool get(VariantRef& value, bool allowConversion = true, Conversion* properties = nullptr) const;

    template<typename T>
    bool canSet(const T& value, bool allowConversion = true, Conversion* properties = nullptr);
    bool canSet(const VariantCRef& value, bool allowConversion = true, Conversion* properties = nullptr);

    template<typename T>
    bool set(const T& value, bool allowConversion = true, Conversion* properties = nullptr);
    bool set(const VariantCRef& value, bool allowConversion = true, Conversion* properties = nullptr);

    template<class T>
    const T& valueCRef() const;

    template<class T>
    T& valueRef() const;

    Variant value() const;

private:
    BaseTypeRef m_value;

    friend class Variant;
    friend class VariantCRef;
};

// =============================================================================
// (public)
template<typename T>
VariantRef::VariantRef(T& value)  : m_value(value) { }

// =============================================================================
// (public)
template<typename T>
bool VariantRef::operator==(const T& value) const
{
    return *this == VariantCRef(value);
}

// =============================================================================
// (public)
template<typename T>
bool VariantRef::operator!=(const T& value) const
{
    return *this != VariantCRef(value);
}

// =============================================================================
// (public)
template<typename T>
bool VariantRef::isEqual(const T& value, bool allowConversion, Conversion* properties) const
{
    return isEqual(VariantCRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<typename T>
bool VariantRef::canGet(T& value, bool allowConversion, Conversion* properties) const
{
    return canGet(VariantRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<typename T>
bool VariantRef::get(T& value, bool allowConversion, Conversion* properties) const
{
    return get(VariantRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<typename T>
bool VariantRef::canSet(const T& value, bool allowConversion, Conversion* properties)
{
    return canSet(VariantCRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<typename T>
bool VariantRef::set(const T& value, bool allowConversion, Conversion* properties)
{
    return set(VariantCRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<class T>
const T& VariantRef::valueCRef() const
{
    return boost::get<const T&>(m_value);
}

// =============================================================================
// (public)
template<class T>
T& VariantRef::valueRef() const
{
    return boost::get<T&>(m_value);
}

} // namespace Model
} // namespace Oak
