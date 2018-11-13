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

#include <assert.h>

#include "Union.h"
#include "ConvertFunctions.h"


namespace Oak::Model {

class UnionValue;

// =============================================================================
// Class definition
// =============================================================================
class UnionRef
{
public:
    UnionRef();
    UnionRef(const char *c);
    UnionRef(const bool &b);
    UnionRef(const int &i);
    UnionRef(const double &d);
    UnionRef(const std::string &s);
    UnionRef(const DateTime &dt);
    UnionRef(const UnionValue &v);

    UnionRef(const UnionRef &copy);

    ~UnionRef();

    UnionRef& operator=(const UnionRef& copy);

    template<typename T>
    UnionRef& operator=(T value) { return *this = UnionRef(value); }

    bool operator==(const UnionRef& value) const;
    bool operator!=(const UnionRef& value) const;

    bool operator>(const UnionRef& value) const;
    bool operator>=(const UnionRef& value) const;
    bool operator<(const UnionRef& value) const;
    bool operator<=(const UnionRef& value) const;

    template<typename T>
    bool operator==(T value) const { return *this == UnionRef(value); }
    template<typename T>
    bool operator!=(T value) const { return *this != UnionRef(value); }

    template<typename T>
    bool operator>(T value) const { return *this > UnionRef(value); }
    template<typename T>
    bool operator>=(T value) const { return *this >= UnionRef(value); }
    template<typename T>
    bool operator<(T value) const { return *this < UnionRef(value); }
    template<typename T>
    bool operator<=(T value) const { return *this <= UnionRef(value); }

    int compare(UnionRef value, bool allowConversion = true, Conversion* properties = nullptr) const;

    bool isEqual(const UnionRef& value, bool allowConversion = true, Conversion* properties = nullptr) const;

    bool isNull() const;

    UnionType type() const;

    bool getBool() const;
    int getInt() const;
    double getDouble() const;
    const std::string& getCString() const;
    std::string& getString();
    const DateTime &getDateTime() const;

    template<typename T>
    bool canGet(T& target, bool allowConversion = true, Conversion* properties = nullptr) const;
    template<typename T>
    bool get(T& target, bool allowConversion = true, Conversion* properties = nullptr) const;

    bool canGet(UnionRef& target, bool allowConversion = true, Conversion* properties = nullptr) const;
    bool get(UnionRef& target, bool allowConversion = true, Conversion* properties = nullptr) const;

    bool canGet(UnionValue& target, bool allowConversion = true, Conversion* properties = nullptr) const;
    bool get(UnionValue& target, bool allowConversion = true, Conversion* properties = nullptr) const;

    template<typename T>
    T value(bool allowConversion = true, Conversion* properties = nullptr) const;

protected:
    UPtr r;
    UnionType t;

    friend class UnionValue;
};

// =============================================================================
// (public)
template<typename T>
bool UnionRef::canGet(T &target, bool allowConversion, Conversion *properties) const
{
    if (t == UnionType::Undefined) { return false; }
    if (!allowConversion && t != Union::GetValueType(target)) { return false; }

    switch (t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Char:
            return canConvert(target, r.c, properties);
        case UnionType::Bool:
            return canConvert(target, *r.b, properties);
        case UnionType::Integer:
            return canConvert(target, *r.i, properties);
        case UnionType::Double:
            return canConvert(target, *r.d, properties);
        case UnionType::String:
            return canConvert(target, *r.s, properties);
        case UnionType::DateTime:
            return canConvert(target, *r.dt, properties);
        default:
            assert(false);
            return false;
    }
    return false;
}

// =============================================================================
// (public)
template<typename T>
bool UnionRef::get(T &target, bool allowConversion, Conversion *properties) const
{
    if (t == UnionType::Undefined) { return false; }
    if (!allowConversion && t != Union::GetValueType(target)) { return false; }

    switch (t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Char:
            return convert(target, r.c, properties);
        case UnionType::Bool:
            return convert(target, *r.b, properties);
        case UnionType::Integer:
            return convert(target, *r.i, properties);
        case UnionType::Double:
            return convert(target, *r.d, properties);
        case UnionType::String:
            return convert(target, *r.s, properties);
        case UnionType::DateTime:
            return convert(target, *r.dt, properties);
        default:
            assert(false);
            return false;
    }
    return false;
}

// =============================================================================
// (public)
template<typename T>
T UnionRef::value(bool allowConversion, Conversion *properties) const
{
    T v;
    get(v, allowConversion, properties);
    return std::move(v);
}

} // namespace Oak::Model

