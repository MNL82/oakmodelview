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

namespace Oak {
namespace Model {

class VariantRef;
class VariantCRef;

// =============================================================================
// Class definition
// =============================================================================
class Variant
{
public:
    Variant();
    Variant(const char* value);
    template<typename T>
    Variant(const T& value);
    Variant(const VariantRef& variant);
    Variant(const VariantCRef& variant);

    Variant(const Variant& copy);
    Variant(Variant&& move);

    template<typename T>
    Variant& operator=(const T& value);
    Variant& operator=(const char* value);
    Variant& operator=(const VariantCRef& value);
    Variant& operator=(const VariantRef& value);
    Variant& operator=(const Variant& value);

    template<typename T>
    bool operator==(const T& value) const;
    bool operator==(const VariantCRef& value) const;

    template<typename T>
    bool operator!=(const T& value) const;
    bool operator!=(const VariantCRef& value) const;

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

private:
    BaseType m_value;

    friend class VariantRef;
    friend class VariantCRef;
};

// =============================================================================
// (public)
template<typename T>
Variant::Variant(const T& value) : m_value(value) { }

// =============================================================================
// (public)
template<typename T>
Variant& Variant::operator=(const T& value)
{
    m_value = value;
    return *this;
}

// =============================================================================
// (public)
template<typename T>
bool Variant::operator==(const T& value) const
{
    return *this == VariantCRef(value);
}

// =============================================================================
// (public)
template<typename T>
bool Variant::operator!=(const T& value) const
{
    return *this != VariantCRef(value);
}

// =============================================================================
// (public)
template<typename T>
bool Variant::isEqual(const T& value, bool allowConversion, Conversion* properties) const
{
    return isEqual(VariantCRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<typename T>
bool Variant::canGet(T& value, bool allowConversion, Conversion* properties) const
{
    return canGet(VariantRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<typename T>
bool Variant::get(T& value, bool allowConversion, Conversion* properties) const
{
    return get(VariantRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<typename T>
bool Variant::canSet(const T& value, bool allowConversion, Conversion* properties)
{
    return canSet(VariantCRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<typename T>
bool Variant::set(const T& value, bool allowConversion, Conversion* properties)
{
    return set(VariantCRef(value), allowConversion, properties);
}

// =============================================================================
// (public)
template<class T>
const T& Variant::valueCRef() const
{
    return boost::get<const T&>(m_value);
}

// =============================================================================
// (public)
template<class T>
T& Variant::valueRef() const
{
    return boost::get<T&>(m_value);
}

} // namespace Model
} // namespace Oak
