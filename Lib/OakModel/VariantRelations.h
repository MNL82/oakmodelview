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
#include <vector>
#include <map>

#include "boost/variant.hpp"

#define TDM_USE_CHAR_PTR_IN_VARIANT

namespace Oak {
namespace Model {

class InvalidVariant {
    short dummy;
public:
    bool operator==(const InvalidVariant&) const { return false; }
    bool operator<(const InvalidVariant&) const { return true; }
};

#if 0
typedef bool Bool;
#else
class Bool
{
    bool m_value;
public:
    Bool() {}
    Bool(bool value) { m_value = value; }
    Bool& operator=(bool value) { m_value = value; return *this; }
    explicit operator bool() const { return m_value; }
    bool operator==(const Bool& value) const { return m_value == value.m_value; }
    bool operator!=(const Bool& value) const { return m_value != value.m_value; }
    bool operator<(const Bool&) const { return true; }
    bool value() const { return m_value; }
};
#endif

typedef boost::variant<InvalidVariant, Bool, int, double, std::string, std::vector<Bool>, std::vector<int>, std::vector<double>, std::vector<std::string>> BaseType;
typedef boost::variant<InvalidVariant, Bool&, int&, double&, std::string&, std::vector<Bool>&, std::vector<int>&, std::vector<double>&, std::vector<std::string>&> BaseTypeRef;

#ifdef TDM_USE_CHAR_PTR_IN_VARIANT
typedef boost::variant<InvalidVariant, Bool, int, double, const std::string&, const std::vector<Bool>&, const std::vector<int>&, const std::vector<double>&, const std::vector<std::string>&, const char*> BaseTypeCRef;
#else // TDM_USE_CHAR_PTR_IN_VARIANT
typedef boost::variant<InvalidVariant, Bool, int, double, const std::string&, const std::vector<Bool>&, const std::vector<int>&, const std::vector<double>&, const std::vector<std::string>&> BaseTypeCRef;
#endif // TDM_USE_CHAR_PTR_IN_VARIANT
// =============================================================================
// Class definition
// =============================================================================
class VariantRelations
{
    static std::vector<std::string> s_nameArray;
    static std::vector<const std::type_info*> s_typeArray;

    static std::string s_emptyName;

    static void createRelationMaps();
    static void addRelation(const std::type_info& type, const std::string& name);

public:
    static bool hasWitch(int w);
    static bool hasTypeId(const std::type_info& type);
    static bool hasDisplayName(const std::string& name);

    static const std::string& displayName(int w);
    static const std::string& displayName(const std::type_info& type);

    static const std::type_info& typeId(int w);
    static const std::type_info& typeId(const std::string& name);

    static int witch(const std::type_info& type);
    static int witch(const std::string& name);

    static std::vector<std::string> displayNameArray();
};

} // namespace Model
} // namespace Oak
