/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "VariantRelations.h"

#include <assert.h>

namespace Oak {
namespace Model {

std::vector<std::string> VariantRelations::s_displayNameArray;
std::vector<const std::type_info*> VariantRelations::s_typeArray;
std::string VariantRelations::s_emptyName;

// =============================================================================
// (private)
void VariantRelations::createRelationMaps()
{
    // Can only be called once
    addRelation(typeid(bool), "Bool");
    addRelation(typeid(int), "Integer");
    addRelation(typeid(double), "Real");
    addRelation(typeid(std::string), "String");
    addRelation(typeid(std::vector<bool>), "Bool Array");
    addRelation(typeid(std::vector<int>), "Integer Array");
    addRelation(typeid(std::vector<double>), "Real Array");
    addRelation(typeid(std::vector<std::string>), "String Array");
    addRelation(typeid(const char*), "");
}

// =============================================================================
// (private)
void VariantRelations::addRelation(const std::type_info& type, const std::string& displayName)
{
    if (!s_displayNameArray.empty()) {
        assert(!hasTypeId(type));
        if (hasTypeId(type)) {
            return;
        }
        if (!displayName.empty()) {
            assert(!hasDisplayName(displayName));
            if (hasTypeId(type) || hasDisplayName(displayName)) {
                return;
            }
        }
    }

    s_displayNameArray.push_back(displayName);
    s_typeArray.push_back(&type);
}

// =============================================================================
// (public)
bool VariantRelations::hasWitch(int w)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    return w >= 0 && w < (int)s_displayNameArray.size();
}

// =============================================================================
// (public)
bool VariantRelations::hasTypeId(const std::type_info& type)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    for (const std::type_info* t: s_typeArray)
    {
        if (*t == type) { return true; }
    }
    return false;
}

// =============================================================================
// (public)
bool VariantRelations::hasDisplayName(const std::string& displayName)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    return std::find(s_displayNameArray.begin(), s_displayNameArray.end(), displayName) != s_displayNameArray.end();
}

// =============================================================================
// (public)
const std::string& VariantRelations::displayName(int w)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    if (!hasWitch(w)) { return s_emptyName; }
    return s_displayNameArray[w];
}

// =============================================================================
// (public)
const std::string& VariantRelations::displayName(const std::type_info& type)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    auto it = s_displayNameArray.begin();
    for (const std::type_info* t: s_typeArray)
    {
        if (*t == type) { return *it; }
        it++;
    }
    return s_emptyName;
}

// =============================================================================
// (public)
const std::type_info& VariantRelations::typeId(int w)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    if (!hasWitch(w)) { return typeid(VariantRelations); }
    return *s_typeArray[w];
}

// =============================================================================
// (public)
const std::type_info& VariantRelations::typeId(const std::string& displayName)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    auto it = s_typeArray.begin();
    for (const std::string& s: s_displayNameArray)
    {
        if (s == displayName) { return *(*it); }
        it++;
    }
    return typeid(VariantRelations);
}

// =============================================================================
// (public)
int VariantRelations::witch(const std::type_info& type)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    int w = 0;
    for (const std::type_info* t: s_typeArray)
    {
        if (*t == type) {
            return w;
        }
        w++;
    }
    return -1;
}

// =============================================================================
// (public)
int VariantRelations::witch(const std::string& displayName)
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    auto it = std::find(s_displayNameArray.begin(), s_displayNameArray.end(), displayName);
    if (it == s_displayNameArray.end()) { return -1; }
    return (int)(it - s_displayNameArray.begin());
}

// =============================================================================
// (public)
std::vector<std::string> VariantRelations::displayNameArray()
{
    if (s_displayNameArray.empty()) { createRelationMaps(); }

    return s_displayNameArray;
}

} // namespace Model
} // namespace Oak
