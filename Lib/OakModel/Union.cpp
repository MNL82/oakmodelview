/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Union.h"

#include "UnionRef.h"
#include "UnionValue.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
UnionType Union::GetType(type_info info)
{
    if (typeid(std::string) == info) {
        return UnionType::String;
    }
    if (typeid(double) == info) {
        return UnionType::Double;
    }
    if (typeid(int) == info) {
        return UnionType::Integer;
    }
    if (typeid(bool) == info) {
        return UnionType::Bool;
    }
    if (typeid(char) == info) {
        return UnionType::Char;
    }
    return UnionType::Undefined;
}

// =============================================================================
// (public)
UnionType Union::GetType(const char *)
{
    return UnionType::Char;
}

// =============================================================================
// (public)
UnionType Union::GetType(bool)
{
    return UnionType::Bool;
}

// =============================================================================
// (public)
UnionType Union::GetType(int)
{
    return UnionType::Integer;
}

// =============================================================================
// (public)
UnionType Union::GetType(double)
{
    return UnionType::Double;
}

// =============================================================================
// (public)
UnionType Union::GetType(const std::string &)
{
    return UnionType::String;
}

// =============================================================================
// (public)
UnionType Union::GetType(UnionType t)
{
    return t;
}

// =============================================================================
// (public)
UnionType Union::GetType(const UnionRef &ur)
{
    return ur.type();
}

// =============================================================================
// (public)
UnionType Union::GetType(const UnionValue &uv)
{
    return uv.type();
}

} // namespace Model
} // namespace Oak
