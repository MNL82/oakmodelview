/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Entry.h"
#include "Item.h"
#include "OakModel.h"
#include "ItemIndex.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
Entry::Entry()
    : m_valueDef(nullptr),
      m_item(nullptr)
{

}

// =============================================================================
// (public)
Entry::Entry(const ValueDef* valueDef, const Node &node, const Item *model)
    : m_valueDef(valueDef),
      m_node(node),
      m_item(model)
{

}

// =============================================================================
// (public)
Entry::Entry(const Entry &copy)
    : m_valueDef(copy.m_valueDef),
      m_node(copy.m_node),
      m_item(copy.m_item)
{

}

// =============================================================================
// (public)
Entry::Entry(Entry&& move)
    : m_valueDef(move.m_valueDef),
      m_node(std::move(move.m_node)),
      m_item(move.m_item)
{

}

// =============================================================================
// (public)
Entry& Entry::operator=(const Entry& copy)
{
    m_valueDef = copy.m_valueDef;
    m_node = copy.m_node;
    m_item = copy.m_item;
    return *this;
}

// =============================================================================
// (public)
Entry& Entry::operator=(Entry&& move)
{
    m_valueDef = move.m_valueDef;
    m_node = std::move(move.m_node);
    m_item = move.m_item;
    return *this;
}

// =============================================================================
// (public)
bool Entry::operator==(const Entry &_entry) const
{
    return m_valueDef == _entry.m_valueDef && m_node == _entry.m_node;
}

// =============================================================================
// (public)
bool Entry::operator!=(const Entry &_entry) const
{
    return m_valueDef != _entry.m_valueDef || m_node != _entry.m_node;
}

// =============================================================================
// (public)
bool Entry::isNull() const
{
    return isDefNull() || isNodeNull();
}

// =============================================================================
// (public)
bool Entry::isDefNull() const
{
    return !m_valueDef || m_valueDef->isNull();
}

// =============================================================================
// (public)
bool Entry::isNodeNull() const
{
    return m_node.isNull();
}

// =============================================================================
// (public)
const std::string &Entry::name() const
{
    ASSERT(m_valueDef != nullptr);
    return m_valueDef->name();
}

// =============================================================================
// (public)
const std::string & Entry::displayName() const
{
    ASSERT(m_valueDef != nullptr);
    return m_valueDef->displayName();
}

// =============================================================================
// (public)
const std::string &Entry::tooltip() const
{
    ASSERT(m_valueDef != nullptr);
    return m_valueDef->tooltip();
}

// =============================================================================
// (public)
const Node& Entry::node() const
{
    ASSERT(m_valueDef != nullptr);
    return m_node;
}

// =============================================================================
// (public)
const ValueDef*Entry::valueDef() const
{
    ASSERT(m_valueDef != nullptr);
    return m_valueDef;
}

// =============================================================================
// (public)
const Item* Entry::item() const
{
    return m_item;
}

// =============================================================================
// (public)
UnionValue Entry::value(bool useDefault) const
{
    ASSERT(m_valueDef != nullptr);
    return m_valueDef->value(m_node, useDefault);
}

// =============================================================================
// (public)
std::string Entry::toString(bool useDefault) const
{
    ASSERT(m_valueDef != nullptr);
    return m_valueDef->toString(m_node, useDefault);
}

// =============================================================================
// (public)
bool Entry::hasDefaultValue() const
{
    ASSERT(m_valueDef != nullptr);
    return m_valueDef->hasDefaultValue();
}

//// =============================================================================
//// (public)
//bool Entry::getOptions(std::vector<VariantCRef>& value) const
//{
//    ASSERT(m_valueDef != nullptr);
//    return m_valueDef->getOptions(value);
//}

// =============================================================================
// (public)
const ValueSettings &Entry::settings() const
{
    ASSERT(m_valueDef != nullptr);
    return m_valueDef->settings();
}

// =============================================================================
// (public)
Entry& Entry::emptyEntry()
{
    static Entry empty;
    return empty;
}

// =============================================================================
// (protected)
void Entry::onEntryChangeBefore() const
{
    if (m_item->model() == nullptr) { return; }
    ItemIndexUPtr iIndex = ItemIndex::create(*m_item);
    m_item->model()->onEntryChangeBefore(*iIndex.get(), m_valueDef->name());
}

// =============================================================================
// (protected)
void Entry::onEntryChangeAfter() const
{
    if (m_item->model() == nullptr) { return; }

    int index = m_item->entryIndex(*this);
    ItemIndexUPtr iIndex = ItemIndex::create(*m_item);

    if (m_item->def()->indexOfVariantValueDef() == index) {
        m_item->model()->onEntryTypeChangeAfter(*iIndex.get());
    } else if (m_item->def()->indexOfKeyValueDef() == index) {
        m_item->model()->onEntryKeyChangeAfter(*iIndex.get());
    }

    m_item->model()->onEntryChangeAfter(*iIndex.get(), m_valueDef->name());
}

} // namespace Model
} // namespace Oak
