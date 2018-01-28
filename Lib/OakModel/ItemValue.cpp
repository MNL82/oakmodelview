/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ItemValue.h"
#include "Item.h"
#include "OakModel.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ItemValue::ItemValue()
    : m_valueDef(nullptr),
      m_item(nullptr)
{

}

// =============================================================================
// (public)
ItemValue::ItemValue(const ValueDef* valueDef, const Node& node, const Item *model)
    : m_valueDef(valueDef),
      m_node(node),
      m_item(model)
{

}

// =============================================================================
// (public)
ItemValue::ItemValue(const ItemValue& copy)
    : m_valueDef(copy.m_valueDef),
      m_node(copy.m_node),
      m_item(copy.m_item)
{

}

// =============================================================================
// (public)
ItemValue::ItemValue(ItemValue&& move)
    : m_valueDef(move.m_valueDef),
      m_node(std::move(move.m_node)),
      m_item(move.m_item)
{

}

// =============================================================================
// (public)
ItemValue& ItemValue::operator=(const ItemValue& copy)
{
    m_valueDef = copy.m_valueDef;
    m_node = copy.m_node;
    m_item = copy.m_item;
    return *this;
}

// =============================================================================
// (public)
ItemValue& ItemValue::operator=(ItemValue&& move)
{
    m_valueDef = move.m_valueDef;
    m_node = std::move(move.m_node);
    m_item = move.m_item;
    return *this;
}

// =============================================================================
// (public)
bool ItemValue::operator==(const ItemValue &_itemValue) const
{
    return m_valueDef == _itemValue.m_valueDef && m_node == _itemValue.m_node;
}

// =============================================================================
// (public)
bool ItemValue::operator!=(const ItemValue &_itemValue) const
{
    return m_valueDef != _itemValue.m_valueDef || m_node != _itemValue.m_node;
}

// =============================================================================
// (public)
bool ItemValue::isNull() const
{
    return isDefNull() || isNodeNull();
}

// =============================================================================
// (public)
bool ItemValue::isDefNull() const
{
    return !m_valueDef || m_valueDef->isNull();
}

// =============================================================================
// (public)
bool ItemValue::isNodeNull() const
{
    return m_node.isNull();
}

// =============================================================================
// (public)
const std::string &ItemValue::name() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->name();
}

// =============================================================================
// (public)
const std::string & ItemValue::displayName() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->displayName();
}

// =============================================================================
// (public)
const Node& ItemValue::node() const
{
    assert(m_valueDef != nullptr);
    return m_node;
}

// =============================================================================
// (public)
const ValueDef*ItemValue::valueDef() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef;
}

// =============================================================================
// (public)
const Item* ItemValue::item() const
{
    return m_item;
}

// =============================================================================
// (public)
UnionValue ItemValue::value(bool useDefault) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->value(m_node, useDefault);
}

// =============================================================================
// (public)
std::string ItemValue::toString(bool useDefault) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->toString(m_node, useDefault);
}

// =============================================================================
// (public)
bool ItemValue::hasDefaultValue() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->hasDefaultValue();
}

//// =============================================================================
//// (public)
//bool ItemValue::getOptions(std::vector<VariantCRef>& value) const
//{
//    assert(m_valueDef != nullptr);
//    return m_valueDef->getOptions(value);
//}

// =============================================================================
// (public)
const ValueSettings &ItemValue::settings() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->settings();
}

// =============================================================================
// (public)
const ItemValue& ItemValue::emptyItemValue()
{
    static ItemValue empty;
    return empty;
}

// =============================================================================
// (public)
void ItemValue::onItemValueChanged() const
{
    m_item->model()->onItemValueChanged(*m_item, m_item->valueIndex(*this));
}

} // namespace Model
} // namespace Oak
