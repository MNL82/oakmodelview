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


#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

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
UnionValue::UnionValue(bool b)
    : t(UnionType::Bool)
{
    v.b = b;
}

// =============================================================================
// (public)
UnionValue::UnionValue(int i)
    : t(UnionType::Integer)
{
    v.i = i;
}

// =============================================================================
// (public)
UnionValue::UnionValue(double d)
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
UnionValue::UnionValue(const DateTime &dt)
    : t(UnionType::DateTime)
{
    v.dt = new DateTime(dt);
}

// =============================================================================
// (public)
UnionValue::UnionValue(UnionType type)
    : t(type)
{
    ASSERT(static_cast<int>(type) > 0);
    if (type == UnionType::String) {
        v.s = new std::string();
    } else if (type == UnionType::DateTime) {
        v.dt = new DateTime(DateTime::defaultDateTime());
    } else {
        v.i = 0;
    }
}

// =============================================================================
// (public)
UnionValue::UnionValue(const UnionRef &uRef)
{
    *this = uRef;
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
    } else if (t == UnionType::DateTime) {
        delete v.dt;
        v.dt = nullptr;
    }
}

// =============================================================================
// (public)
UnionValue &UnionValue::operator=(const UnionRef &value)
{
    if (t == UnionType::String) {
        if (value.t == UnionType::String) {
            *v.s = *value.r.s;
            return *this;
        } else {
            delete v.s;
        }
    }

    if (t == UnionType::DateTime) {
        if (value.t == UnionType::DateTime) {
            *v.dt = *value.r.dt;
            return *this;
        } else {
            delete v.dt;
        }
    }

    t = value.t;
    switch (value.t) {
        case UnionType::Undefined:
            v.i = 0;
            break;
        case UnionType::Char:
            v.s = new std::string(value.r.c);
            t = UnionType::String;
            break;
        case UnionType::Bool:
            v.b = *value.r.b;
            break;
        case UnionType::Integer:
            v.i = *value.r.i;
            break;
        case UnionType::Double:
            v.d = *value.r.d;
            break;
        case UnionType::String:
            v.s = new std::string(*value.r.s);
            break;
        case UnionType::DateTime:
            v.dt = new DateTime(*value.r.dt);
            break;
        default:
            ASSERT(false);
    }

    return *this;
}

// =============================================================================
// (public)
UnionValue &UnionValue::operator=(const UnionValue &copy)
{
    UnionRef uRef(copy);
    return *this = uRef;
}

// =============================================================================
// (public)
bool UnionValue::operator==(const UnionRef &value) const
{
    UnionRef uRef(*this);
    return uRef == value;
}

// =============================================================================
// (public)
bool UnionValue::operator!=(const UnionRef &value) const
{
    UnionRef uRef(*this);
    return uRef != value;
}

// =============================================================================
// (public)
bool UnionValue::operator>=(const UnionRef &value) const
{
    UnionRef uRef(*this);
    return uRef >= value;
}

// =============================================================================
// (public)
bool UnionValue::operator>(const UnionRef &value) const
{
    UnionRef uRef(*this);
    return uRef > value;
}

// =============================================================================
// (public)
bool UnionValue::operator<(const UnionRef &value) const
{
    UnionRef uRef(*this);
    return uRef < value;
}

// =============================================================================
// (public)
bool UnionValue::operator<=(const UnionRef &value) const
{
    UnionRef uRef(*this);
    return uRef <= value;
}

// =============================================================================
// (public)
int UnionValue::compare(const UnionRef &value, bool allowConversion, Conversion *properties) const
{
    UnionRef uRef(*this);
    return uRef.compare(value, allowConversion, properties);
}

// =============================================================================
// (public)
bool UnionValue::isEqual(const UnionRef &value, bool allowConversion, Conversion *properties) const
{
    UnionRef uRef(*this);
    return uRef.isEqual(value, allowConversion, properties);
}

// =============================================================================
// (public)
bool UnionValue::isNull() const
{
    return t == UnionType::Undefined;
}

// =============================================================================
// (public)
UnionValue &UnionValue::operator=(UnionValue &&move)
{
    if (t == UnionType::String) {
        delete v.s;
    } else if (t == UnionType::DateTime) {
        delete v.dt;
    }
    t = move.t;
    memcpy(&v, &move.v, sizeof(v));

    move.t = UnionType::Undefined;
    move.v.s = nullptr;

    return *this;
}

// =============================================================================
// (public)
UnionValue::operator bool() const
{
    switch (t) {
        case UnionType::Undefined:
            ASSERT(false);
            return false;
        case UnionType::Bool:
            return v.b;
        case UnionType::Integer:
            return v.i != 0;
        case UnionType::Double:
            return v.d != 0.0;
        case UnionType::String:
            return !v.s->empty();
        case UnionType::DateTime:
            return true;
        default:
            ASSERT(false);
    }
    return false;
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
    ASSERT(t == UnionType::Bool);
    return v.b;
}

// =============================================================================
// (public)
int UnionValue::getInt() const
{
    ASSERT(t == UnionType::Integer);
    return v.i;
}

// =============================================================================
// (public)
double UnionValue::getDouble() const
{
    ASSERT(t == UnionType::Double);
    return v.d;
}

// =============================================================================
// (public)
const std::string& UnionValue::getCString() const
{
    ASSERT(t == UnionType::String);
    return *v.s;
}

// =============================================================================
// (public)
std::string& UnionValue::getString()
{
    ASSERT(t == UnionType::String);
    return *v.s;
}

// =============================================================================
// (public)
bool UnionValue::canGet(UnionRef &target, bool allowConversion, Conversion *properties) const
{
    UnionRef sourceRef(*this);
    return sourceRef.canGet(target, allowConversion, properties);
}

// =============================================================================
// (public)
bool UnionValue::get(UnionRef &target, bool allowConversion, Conversion *properties) const
{
    UnionRef sourceRef(*this);
    return sourceRef.get(target, allowConversion, properties);
}

// =============================================================================
// (public)
bool UnionValue::canGet(UnionValue &target, bool allowConversion, Conversion *properties) const
{
    UnionRef sourceRef(*this);
    UnionRef targetRef(target);
    return sourceRef.canGet(targetRef, allowConversion, properties);
}

// =============================================================================
// (public)
bool UnionValue::get(UnionValue &target, bool allowConversion, Conversion *properties) const
{
    UnionRef sourceRef(*this);
    UnionRef targetRef(target);
    return sourceRef.get(targetRef, allowConversion, properties);
}

} // namespace Oak::Model

