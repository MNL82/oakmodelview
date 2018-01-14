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
#include "Conversion.h"
#include "ConvertFunctions.h"

#include <assert.h>

namespace Oak {
namespace Model {

class UnionRef;

// =============================================================================
// Class definition
// =============================================================================
class UnionValue
{
public:
    UnionValue();
    UnionValue(const char *c);
    UnionValue(const bool &b);
    UnionValue(const int &i);
    UnionValue(const double &d);
    UnionValue(const std::string &s);

    UnionValue(const UnionValue& copy);
    UnionValue(UnionValue&& move);

    ~UnionValue();

    UnionValue& operator=(const UnionValue& copy);
    UnionValue& operator=(UnionValue&& move);

    UnionType type() const;

    const UnionRef getRef() const;
    UnionRef getRef();

    bool getBool() const;
    int getInt() const;
    double getDouble() const;
    const std::string& getString() const;

    template<typename T>
    bool canConvertTo(T& value, Conversion* properties = nullptr) const;
    template<typename T>
    bool convertTo(T& value, Conversion* properties = nullptr) const;

protected:
    UValue v;
    UnionType t;

    friend class UnionRef;
};

// =============================================================================
// (public)
template<typename T>
bool UnionValue::canConvertTo(T &value, Conversion *properties) const
{
    switch (t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Bool:
            return canConvert(value, v.b, properties);
        case UnionType::Integer:
            return canConvert(value, v.i, properties);
        case UnionType::Double:
            return canConvert(value, v.d, properties);
        case UnionType::String:
            return canConvert(value, v.s, properties);
        default:
            assert(false);
            return false;
    }
}

// =============================================================================
// (public)
template<typename T>
bool UnionValue::convertTo(T &value, Conversion *properties) const
{
    switch (t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Bool:
            return convert(value, v.b, properties);
        case UnionType::Integer:
            return convert(value, v.i, properties);
        case UnionType::Double:
            return convert(value, v.d, properties);
        case UnionType::String:
            return convert(value, v.s, properties);
        default:
            assert(false);
            return false;
    }
}


} // namespace Model
} // namespace Oak
