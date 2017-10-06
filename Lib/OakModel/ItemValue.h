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

    VariantCRef valueId() const;

    const Node& node() const;
    const ValueDefinition* valueDefinition() const;
    const Item* item() const;

    bool canGetValue(VariantRef value, bool useDefault = true) const;
    bool getValue(VariantRef value, bool useDefault = true) const;
    Variant value(bool useDefault = true) const;
    std::string toString(bool useDefault = true) const;

    bool canSetValue(VariantCRef value) const;
    bool setValue(VariantCRef value) const;

    bool hasDefaultValue() const;
    bool getDefaultValue(VariantRef value) const;

    bool getOptions(std::vector<VariantCRef>& value) const;

    template<typename T>
    bool getOptions(std::vector<T>& value) const
    {
        assert(m_valueDefinition != nullptr);
        value.clear();
        std::vector<VariantCRef> optionList;
        if (!m_valueDefinition->getOptions(optionList)) { return false; }

        for (const VariantCRef& option: optionList)
        {
            value.push_back(option.valueCRef<T>());
        }
        return true;
    }

    const ValueSettings& settings();

    static const ItemValue& emptyItemValue();

    template<typename T>
    bool operator<<(const T& value) const
    {
        return setValue(value);
    }

protected:
    const ValueDefinition* m_valueDefinition;
    Node m_node;
    const Item* m_item;
};

template<typename T>
bool operator<<(T & value, const ItemValue &item)
{
    assert(!item.isNull());
    return item.getValue(value);
}

} // namespace Model
} // namespace Oak
