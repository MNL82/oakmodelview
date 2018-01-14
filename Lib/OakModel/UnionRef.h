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

#include "Union.h"
#include "ConvertFunctions.h"

namespace Oak {
namespace Model {

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
    UnionRef(const UnionValue& v);

    UnionRef(const UnionRef& copy);

    ~UnionRef();

    UnionRef& operator=(const UnionRef& copy);

    UnionType type() const;

    bool getBool() const;
    int getInt() const;
    double getDouble() const;
    const std::string& getString() const;

    template<typename T>
    bool canConvertTo(T& value, Conversion* properties = nullptr) const;
    template<typename T>
    bool convertTo(T& value, Conversion* properties = nullptr) const;

    bool canConvertTo(UnionRef& uRef, Conversion* properties = nullptr) const;
    bool convertTo(UnionRef& uRef, Conversion* properties = nullptr) const;

    bool canConvertTo(UnionValue& target, Conversion* properties = nullptr) const;
    bool convertTo(UnionValue& uRef, Conversion* properties = nullptr) const;

protected:
    UPtr r;
    UnionType t;

    friend class UnionValue;
};


// =============================================================================
// (public)
template<typename T>
bool UnionRef::canConvertTo(T &value, Conversion *properties) const
{
    switch (t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Char:
            return canConvert(value, r.c, properties);
        case UnionType::Bool:
            return canConvert(value, *r.b, properties);
        case UnionType::Integer:
            return canConvert(value, *r.i, properties);
        case UnionType::Double:
            return canConvert(value, *r.d, properties);
        case UnionType::String:
            return canConvert(value, *r.s, properties);
        default:
            assert(false);
            return false;
    }
}

// =============================================================================
// (public)
template<typename T>
bool UnionRef::convertTo(T &value, Conversion *properties) const
{
    switch (t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Char:
            return convert(value, r.c, properties);
        case UnionType::Bool:
            return convert(value, *r.b, properties);
        case UnionType::Integer:
            return convert(value, *r.i, properties);
        case UnionType::Double:
            return convert(value, *r.d, properties);
        case UnionType::String:
            return convert(value, *r.s, properties);
        default:
            assert(false);
            return false;
    }
}



} // namespace Model
} // namespace Oak
