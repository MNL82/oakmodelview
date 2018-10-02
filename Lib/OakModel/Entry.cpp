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
Entry::Entry(const ValueDef* valueDef, const Node& node, const Item *model)
    : m_valueDef(valueDef),
      m_node(node),
      m_item(model)
{

}

// =============================================================================
// (public)
Entry::Entry(const Entry& copy)
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
    assert(m_valueDef != nullptr);
    return m_valueDef->name();
}

// =============================================================================
// (public)
const std::string & Entry::displayName() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->displayName();
}

// =============================================================================
// (public)
const std::string &Entry::tooltip() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->tooltip();
}

// =============================================================================
// (public)
const Node& Entry::node() const
{
    assert(m_valueDef != nullptr);
    return m_node;
}

// =============================================================================
// (public)
const ValueDef*Entry::valueDef() const
{
    assert(m_valueDef != nullptr);
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
    assert(m_valueDef != nullptr);
    return m_valueDef->value(m_node, useDefault);
}

// =============================================================================
// (public)
std::string Entry::toString(bool useDefault) const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->toString(m_node, useDefault);
}

// =============================================================================
// (public)
bool Entry::hasDefaultValue() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->hasDefaultValue();
}

//// =============================================================================
//// (public)
//bool Entry::getOptions(std::vector<VariantCRef>& value) const
//{
//    assert(m_valueDef != nullptr);
//    return m_valueDef->getOptions(value);
//}

// =============================================================================
// (public)
const ValueSettings &Entry::settings() const
{
    assert(m_valueDef != nullptr);
    return m_valueDef->settings();
}

// =============================================================================
// (public)
const Entry& Entry::emptyEntry()
{
    static Entry empty;
    return empty;
}

// =============================================================================
// (public)
void Entry::onEntryChanged() const
{
    if (m_item->model() == nullptr) { return; }

    int index = m_item->entryIndex(*this);
    ItemIndexUPtr iIndex = ItemIndex::create(*m_item);

    if (m_item->def()->derivedIdValueDefIndex() == index) {
        m_item->model()->onEntryTypeChangeAfter(*iIndex.get());
    } else if (m_item->def()->keyValueDefIndex() == index) {
        m_item->model()->onEntryKeyChangeAfter(*iIndex.get());
    }

    m_item->model()->onEntryChangeAfter(*iIndex.get(), index);
}

} // namespace Model
} // namespace Oak
