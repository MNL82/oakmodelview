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

#include <utility>

#include "ValueDef.h"
#include "ValueOptions.h"

namespace Oak {
namespace Model {

class Item;

// =============================================================================
// Class definition
// =============================================================================
class Entry
{
public:
    Entry();
    Entry(const ValueDef* valueDef, const Node &node, const Item* item/* = nullptr*/);
    Entry(const Entry& copy);
    Entry(Entry&& move);

    Entry& operator=( const Entry& copy);
    Entry& operator=(Entry&& move);

    bool operator==(const Entry& _entry) const;
    bool operator!=(const Entry& _entry) const;

    bool isNull() const;
    bool isDefNull() const;
    bool isNodeNull() const;

    const std::string &name() const;
    const std::string &displayName() const;

    const std::string &tooltip() const;

    const Node& node() const;
    const ValueDef* valueDef() const;
    const Item* item() const;

    template<typename T>
    bool canGetValue(T &value, bool useDefault = true) const;
    template<typename T>
    bool getValue(T &value, bool useDefault = true) const;

    UnionValue value(bool useDefault = true) const;
    template<typename T>
    T value(bool useDefault = true) const;

    std::string toString(bool useDefault = true) const;

    template<typename T>
    bool canSetValue(const T &value) const;
    template<typename T>
    bool setValue(const T &value) const;

    bool hasDefaultValue() const;
    template<typename T>
    bool getDefaultValue(T &value) const;

    template<typename T>
    T defaultValue() const;

    //bool getOptions(std::vector<VariantCRef>& value) const;

    template<typename T>
    bool getOptions(std::vector<T>& value) const;

    const ValueSettings& settings() const;

    static Entry &emptyEntry();

    template<typename T>
    bool operator<<(const T& value) const;

protected:
    void onEntryChangeBefore() const;
    void onEntryChangeAfter() const;

protected:
    const ValueDef* m_valueDef;
    Node m_node;
    const Item* m_item;

    friend class Item;
};

// =============================================================================
// (public)
template<typename T>
bool Entry::canGetValue(T &value, bool useDefault) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->canGetValue(m_node, value, useDefault, true);
}

// =============================================================================
// (public)
template<typename T>
bool Entry::getValue(T &value, bool useDefault) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->getValue(m_node, value, useDefault, true);
}

// =============================================================================
// (public)
template<typename T>
T Entry::value(bool useDefault) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->value<T>(m_node, useDefault, true);
}

// =============================================================================
// (public)
template<typename T>
bool Entry::canSetValue(const T &value) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->canSetValue(m_node, value, true);
}

// =============================================================================
// (public)
template<typename T>
bool Entry::setValue(const T &value) const
{
    assert(m_valueDef != nullptr);
    if (m_item) {
        onEntryChangeBefore();
    }
    bool result = m_valueDef->setValue(m_node, value, true);
    if (m_item) {
        onEntryChangeAfter();
    }
    return result;
}

// =============================================================================
// (public)
template<typename T>
bool Entry::getDefaultValue(T &value) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->getDefaultValue(value);
}

// =============================================================================
// (public)
template<typename T>
T Entry::defaultValue() const
{
    assert(m_valueDef != nullptr);
    T value;
    m_valueDef->getDefaultValue(value);
    return std::move(value);
}

// =============================================================================
// (public)
template<typename T>
bool Entry::getOptions(std::vector<T>& value) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->options().getOptions(value, m_item);
}

// =============================================================================
// (public)
template<typename T>
bool Entry::operator<<(const T& value) const
{
    return setValue(value);
}

// =============================================================================
// (free)
template<typename T>
bool operator<<(T & value, const Entry &item)
{
    assert(!item.isNull());
    return item.getValue(value);
}


} // namespace Model
} // namespace Oak
