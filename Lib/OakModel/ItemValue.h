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

#include "ValueDefinition.h"

namespace Oak {
namespace Model {

class Item;

// =============================================================================
// Class definition
// =============================================================================
class ItemValue
{
public:
    ItemValue();
    ItemValue(const ValueDefinition* valueDefinition, const Node& node, const Item* item/* = nullptr*/);
    ItemValue(const ItemValue& copy);
    ItemValue(ItemValue&& move);

    ItemValue& operator=(const ItemValue& copy);
    ItemValue& operator=(ItemValue&& move);

    bool operator==(const ItemValue& _itemValue) const;
    bool operator!=(const ItemValue& _itemValue) const;

    bool isNull() const;
    bool isDefinitionNull() const;
    bool isNodeNull() const;

    const std::string &name() const;

    const Node& node() const;
    const ValueDefinition* valueDefinition() const;
    const Item* item() const;

    template<typename T>
    bool canGetValue(T &value, bool useDefault = true) const;
    template<typename T>
    bool getValue(T &value, bool useDefault = true) const;

    Variant value(bool useDefault = true) const;
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

    static const ItemValue& emptyItemValue();

    template<typename T>
    bool operator<<(const T& value) const;

protected:
    void onItemValueChanged() const;

protected:
    const ValueDefinition* m_valueDefinition;
    Node m_node;
    const Item* m_item;
};

// =============================================================================
// (public)
template<typename T>
bool ItemValue::canGetValue(T &value, bool useDefault) const
{
    assert(m_valueDefinition != nullptr);
    return m_valueDefinition->canGetValue(m_node, value, useDefault, true);
}

// =============================================================================
// (public)
template<typename T>
bool ItemValue::getValue(T &value, bool useDefault) const
{
    assert(m_valueDefinition != nullptr);
    return m_valueDefinition->getValue(m_node, value, useDefault, true);
}

// =============================================================================
// (public)
template<typename T>
T ItemValue::value(bool useDefault) const
{
    assert(m_valueDefinition != nullptr);
    return m_valueDefinition->value<T>(m_node, useDefault, true);
}

// =============================================================================
// (public)
template<typename T>
bool ItemValue::canSetValue(const T &value) const
{
    assert(m_valueDefinition != nullptr);
    return m_valueDefinition->canSetValue(m_node, value, true);
}

// =============================================================================
// (public)
template<typename T>
bool ItemValue::setValue(const T &value) const
{
    assert(m_valueDefinition != nullptr);
    bool result = m_valueDefinition->setValue(m_node, value, true);
    if (result && m_item) {
        onItemValueChanged();
    }
    return result;
}

// =============================================================================
// (public)
template<typename T>
bool ItemValue::getDefaultValue(T &value) const
{
    assert(m_valueDefinition != nullptr);
    return m_valueDefinition->getDefaultValue(value);
}

// =============================================================================
// (public)
template<typename T>
T ItemValue::defaultValue() const
{
    assert(m_valueDefinition != nullptr);
    T value;
    m_valueDefinition->getDefaultValue(value);
    return std::move(value);
}

// =============================================================================
// (public)
template<typename T>
bool ItemValue::getOptions(std::vector<T>& value) const
{
    assert(m_valueDefinition != nullptr);
    return m_valueDefinition->getOptions(value);
}

// =============================================================================
// (public)
template<typename T>
bool ItemValue::operator<<(const T& value) const
{
    return setValue(value);
}

// =============================================================================
// (free)
template<typename T>
bool operator<<(T & value, const ItemValue &item)
{
    assert(!item.isNull());
    return item.getValue(value);
}


} // namespace Model
} // namespace Oak
