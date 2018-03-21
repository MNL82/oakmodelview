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

#include <string>

namespace Oak {
namespace Model {

enum class UnionType { Undefined = -1, Char = 0, Bool = 1, Integer = 2, Double = 3, String = 4 };
typedef union UValue { bool b; int i; double d; std::string *s; } UValue;
typedef union UPtr { const char *c; const bool *b; const int *i; const double *d; const std::string *s; } UPtr;

class UnionRef;
class UnionValue;

// =============================================================================
// Class definition
// =============================================================================
class Union
{
public:
    static UnionType GetType(type_info info);
    static UnionType GetType(const char*);
    static UnionType GetType(const bool&);
    static UnionType GetType(const int&);
    static UnionType GetType(const double&);
    static UnionType GetType(const std::string&);
    static UnionType GetType(const UnionRef& ur);
    static UnionType GetType(const UnionValue& uv);

    template<typename T>
    static UnionType GetValueType(const T &v);
};

// =============================================================================
// (public)
template<typename T>
UnionType Union::GetValueType(const T &v)
{
    UnionType type = GetType(v);
    return (type == UnionType::Char) ? UnionType::String : type;

}


} // namespace Model
} // namespace Oak
