/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "UnionValue.h"

#include "UnionRef.h"
#include "ConvertFunctions.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
UnionValue::UnionValue()
    : t(UnionType::Undefined)
{
    v.i = 0;
}

// =============================================================================
// (public)
UnionValue::UnionValue(const char *c)
    : t(UnionType::String)
{
    v.s = new std::string(c);
}

// =============================================================================
// (public)
UnionValue::UnionValue(const bool &b)
    : t(UnionType::Bool)
{
    v.b = b;
}

// =============================================================================
// (public)
UnionValue::UnionValue(const int &i)
    : t(UnionType::Integer)
{
    v.i = i;
}

// =============================================================================
// (public)
UnionValue::UnionValue(const double &d)
    : t(UnionType::Double)
{
    v.d = d;
}

// =============================================================================
// (public)
UnionValue::UnionValue(const std::string &s)
    : t(UnionType::String)
{
    v.s = new std::string(s);
}

// =============================================================================
// (public)
UnionValue::UnionValue(const UnionValue &copy)
    : t(UnionType::Undefined)
{
    *this = copy;
}

// =============================================================================
// (public)
UnionValue::UnionValue(UnionValue &&move)
    : t(UnionType::Undefined)
{
    *this = std::move(move);
}

// =============================================================================
// (public)
UnionValue::~UnionValue()
{
    if (t == UnionType::String) {
        delete v.s;
        v.s = nullptr;
    }
}

// =============================================================================
// (public)
UnionValue &UnionValue::operator=(const UnionValue &copy)
{
    if (t == copy.t) {
        if (t == UnionType::String) {
            *v.s = *copy.v.s;
        } else {
            v = copy.v;
        }
    } else {
        if (t == UnionType::String) {
            delete v.s;
        }
        if (copy.t == UnionType::String) {
            v.s = new std::string(*copy.v.s);
        } else {
            memcpy(&v, &copy.v, sizeof(v));
        }
        t = copy.t;
    }

    return *this;
}

// =============================================================================
// (public)
UnionValue &UnionValue::operator=(UnionValue &&move)
{
    if (t == UnionType::String) {
        delete v.s;
    }
    t = move.t;
    memcpy(&v, &move.v, sizeof(v));

    move.t = UnionType::Undefined;
    move.v.s = nullptr;

    return *this;
}

// =============================================================================
// (public)
UnionType UnionValue::type() const
{
    return t;
}

// =============================================================================
// (public)
const UnionRef UnionValue::getRef() const
{
    return UnionRef(*this);
}

// =============================================================================
// (public)
UnionRef UnionValue::getRef()
{
    return UnionRef(*this);
}

// =============================================================================
// (public)
bool UnionValue::getBool() const
{
    assert(t == UnionType::Bool);
    return v.b;
}

// =============================================================================
// (public)
int UnionValue::getInt() const
{
    assert(t == UnionType::Integer);
    return v.i;
}

// =============================================================================
// (public)
double UnionValue::getDouble() const
{
    assert(t == UnionType::Double);
    return v.d;
}

// =============================================================================
// (public)
const std::string& UnionValue::getString() const
{
    assert(t == UnionType::String);
    return *v.s;
}

} // namespace Model
} // namespace Oak
