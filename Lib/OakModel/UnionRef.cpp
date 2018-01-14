/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "UnionRef.h"
#include "UnionValue.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
UnionRef::UnionRef()
    : t(UnionType::Undefined)
{
    r.i = nullptr;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const char *c)
    : t(UnionType::Char)
{
    r.c = c;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const bool &b)
    : t(UnionType::Bool)
{
    r.b = const_cast<bool*>(&b);
}

// =============================================================================
// (public)
UnionRef::UnionRef(const int &i)
    : t(UnionType::Integer)
{
    r.i = &i;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const double &d)
    : t(UnionType::Double)
{
    r.d = &d;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const std::string &s)
    : t(UnionType::String)
{
    r.s = &s;
}

// =============================================================================
// (public)
UnionRef::UnionRef(const UnionValue &v)
    : t(v.type())
{
    if (t == UnionType::Bool) {
        r.b = const_cast<bool*>(&v.v.b);
    } else if (t == UnionType::Integer) {
        r.i = const_cast<int*>(&v.v.i);
    } else if (t == UnionType::Double) {
        r.d = const_cast<double*>(&v.v.d);
    } else if (t == UnionType::String) {
        r.s = v.v.s;
    } else if (t == UnionType::Undefined) {
        r.i = nullptr;
    } else {
        assert(false);
    }
}

// =============================================================================
// (public)
UnionRef::UnionRef(const UnionRef &copy)
    : t(copy.t)
{
    memcpy(&r, &copy.r, sizeof(r));
}

// =============================================================================
// (public)
UnionRef::~UnionRef()
{

}

// =============================================================================
// (public)
UnionType UnionRef::type() const
{
    return t;
}

// =============================================================================
// (public)
bool UnionRef::getBool() const
{
    assert(t == UnionType::Bool);
    return *r.b;
}

// =============================================================================
// (public)
int UnionRef::getInt() const
{
    assert(t == UnionType::Integer);
    return *r.i;
}

// =============================================================================
// (public)
double UnionRef::getDouble() const
{
    assert(t == UnionType::Double);
    return *r.d;
}

// =============================================================================
// (public)
const std::string& UnionRef::getString() const
{
    assert(t == UnionType::String);
    return *r.s;
}

// =============================================================================
// (public)
bool UnionRef::canConvertTo(UnionRef &uRef, Conversion *properties) const
{
    if (t == UnionType::Undefined) { return false; }

    switch (uRef.t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Char:
            return false;
        case UnionType::Bool:
            return canConvertTo(static_cast<bool>(*uRef.r.b), properties);
        case UnionType::Integer:
            return canConvertTo(static_cast<int>(*uRef.r.i), properties);
        case UnionType::Double:
            return canConvertTo(static_cast<double>(*uRef.r.d), properties);
        case UnionType::String:
            return canConvertTo(static_cast<std::string>(*uRef.r.s), properties);
        default:
            assert(false);
            return false;
    }
}

// =============================================================================
// (public)
bool UnionRef::convertTo(UnionRef &uRef, Conversion *properties) const
{
    if (t == UnionType::Undefined) { return false; }

    switch (uRef.t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Char:
            return false;
        case UnionType::Bool:
            return convertTo(static_cast<bool>(*uRef.r.b), properties);
        case UnionType::Integer:
            return convertTo(static_cast<int>(*uRef.r.i), properties);
        case UnionType::Double:
            return convertTo(static_cast<double>(*uRef.r.d), properties);
        case UnionType::String:
            return convertTo(static_cast<std::string>(*uRef.r.s), properties);
        default:
            assert(false);
            return false;
    }
}

// =============================================================================
// (public)
bool UnionRef::canConvertTo(UnionValue &target, Conversion *properties) const
{
    return canConvertTo(UnionRef(target), properties);
//    if (t == UnionType::Undefined) { return false; }

//    switch (uValue.t) {
//        case UnionType::Undefined:
//            return false;
//        case UnionType::Bool:
//            return canConvert(static_cast<bool>(uValue.v.b), properties);
//        case UnionType::Integer:
//            return canConvert(static_cast<int>(uValue.v.i), properties);
//        case UnionType::Double:
//            return canConvert(static_cast<double>(uValue.v.d), properties);
//        case UnionType::String:
//            return canConvert(static_cast<std::string>(*uValue.v.s), properties);
//        default:
//            assert(false);
//            return false;
//    }
}

// =============================================================================
// (public)
bool UnionRef::convertTo(UnionValue &uValue, Conversion *properties) const
{
    if (t == UnionType::Undefined) { return false; }

    switch (uValue.t) {
        case UnionType::Undefined:
            return false;
        case UnionType::Bool:
            return convert(static_cast<bool>(uValue.v.b), properties);
        case UnionType::Integer:
            return convert(static_cast<int>(uValue.v.i), properties);
        case UnionType::Double:
            return convert(static_cast<double>(uValue.v.d), properties);
        case UnionType::String:
            return convert(static_cast<std::string>(*uValue.v.s), properties);
        default:
            assert(false);
            return false;
    }
}

} // namespace Model
} // namespace Oak
