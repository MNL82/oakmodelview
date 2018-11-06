/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Item.h"

#include <algorithm>

#include "OakModel.h"
#include "EntryQuery.h"
#include "ServiceFunctions.h"
#include "QueryBuilder.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
Item::Item()
    : m_def(nullptr),
      m_model(nullptr)
{

}

// =============================================================================
// (public)
Item::Item(const NodeDef* nodeDef, Node node, const OakModel* model)
    : m_node(node),
      m_model(model)
{
    if (nodeDef == nullptr || node.isNull()) {
        m_def = nodeDef;
    } else {
        m_def = nodeDef->validVariant(node);
    }
}

// =============================================================================
// (public)
Item::Item(const Item& copy)
    : m_def(copy.m_def),
      m_node(copy.m_node),
      m_model(copy.m_model),
      m_entryList(copy.m_entryList)
{

}

// =============================================================================
// (public)
Item::Item(Item&& move)
    : m_def(std::move(move.m_def)),
      m_node(std::move(move.m_node)),
      m_model(move.m_model),
      m_entryList(std::move(move.m_entryList))
{

}

// =============================================================================
// (public)
Item& Item::operator=(const Item& copy)
{
    m_def = copy.m_def;
    m_node = copy.m_node;
    m_model = copy.m_model;

    if (m_def == copy.m_def) {  // NodeDef is the same: Entry list is still valid
        if (!m_entryList.empty()) { // Entry list is initialized
            // Update node for entry lists
            for (auto &entry: m_entryList) {
                entry.m_node = m_node;
            }
        }
    } else {
        m_entryList.assign(copy.m_entryList.begin(), copy.m_entryList.end());
    }

    return *this;
}

// =============================================================================
// (public)
Item& Item::operator=(Item&& move)
{
    m_entryList = std::move(move.m_entryList);
    m_def = std::move(move.m_def);
    m_node = std::move(move.m_node);
    m_model = move.m_model;
    return *this;
}

// =============================================================================
// (public)
bool Item::operator==(const Item& _item) const
{
    return m_def == _item.m_def && m_node == _item.m_node;
}

// =============================================================================
// (public)
bool Item::operator!=(const Item& _item) const
{
    return !(operator==(_item));
}

// =============================================================================
// (public)
const Entry& Item::operator()(const std::string &valueName) const
{
    return entry(valueName);
}

// =============================================================================
// (public)
Item Item::operator[](int index) const
{
    return childAt(index);
}

// =============================================================================
// (public)
bool Item::isNull() const
{
    return isNodeNull() || isDefNull();
}

// =============================================================================
// (public)
bool Item::isDefNull() const
{
    return m_def == nullptr || m_def->isNull();
}

// =============================================================================
// (public)
bool Item::isNodeNull() const
{
    return m_node.isNull();
}

// =============================================================================
// (public)
void Item::clear()
{
    m_def = nullptr;
    m_node.clear();
    m_model = nullptr;
    m_entryList.clear();
}

// =============================================================================
// (public)
const OakModel* Item::model() const
{
    return m_model;
}

// =============================================================================
// (public)
void Item::setCurrent()
{
    ASSERT(m_def);
    if (m_model) {
        m_model->setCurrentItem(*this);
    }
}

// =============================================================================
// (public)
std::vector<std::string> Item::valueNameList() const
{
    initEntryList();
    std::vector<std::string> nameList;
    for (const Entry& iv: m_entryList)
    {
        nameList.push_back(iv.name());
    }
    return nameList;
}

// =============================================================================
// (public)
std::vector<std::string> Item::childNameList() const
{
    ASSERT(m_def);
    std::vector<const ContainerDef*> containerList;
    m_def->getContainerList(containerList);
    std::vector<std::string> nameList;
    for (const ContainerDef* container: containerList)
    {
        nameList.push_back(container->containerDef()->name());
    }
    return nameList;
}

// =============================================================================
// (public)
int Item::entryCount() const
{
    initEntryList();
    return static_cast<int>(m_entryList.size());
}

// =============================================================================
// (public)
bool Item::hasEntry(const std::string &entryName) const
{
    return m_def->hasValue(entryName);
}

// =============================================================================
// (public)
int Item::entryIndex(const Entry &entry) const
{
    initEntryList();
    auto it = std::find(m_entryList.begin(), m_entryList.end(), entry);
    if (it == m_entryList.end()) {
        return -1;
    } else {
        return static_cast<int>(std::distance(m_entryList.begin(), it));
    }
}

// =============================================================================
// (public)
const Entry& Item::entryAt(int index) const
{
    initEntryList();
    return m_entryList.at(static_cast<vSize>(index));
}

// =============================================================================
// (public)
const Entry& Item::entry(const std::string &entryName) const
{
    initEntryList();
    for (const Entry& iv: m_entryList)
    {
        if (iv.name() == entryName) {
            return iv;
        }
    }
    return Entry::emptyEntry();
}

// =============================================================================
// (public)
Item::entryIterator Item::entryBegin() const
{
    initEntryList();
    return m_entryList.begin();
}

// =============================================================================
// (public)
Item::entryIterator Item::entryEnd() const
{
    initEntryList();
    return m_entryList.end();
}

// =============================================================================
// (public)
bool Item::hasKey() const
{
    ASSERT(m_def);
    return m_def->hasKey();
}

// =============================================================================
// (public)
const Entry& Item::entryKey() const
{
    initEntryList();
    if (hasKey()) { return m_entryList[static_cast<vSize>(m_def->indexOfKeyValueDef())]; }
    return Entry::emptyEntry();
}

// =============================================================================
// (public)
bool Item::hasVariants() const
{
    ASSERT(m_def);
    return m_def->hasVariants();
}

// =============================================================================
// (public)
const Entry& Item::variantEntry() const
{
    initEntryList();
    if (hasVariants()) { return m_entryList[static_cast<vSize>(m_def->indexOfVariantValueDef())]; }
    return Entry::emptyEntry();
}

// =============================================================================
// (public)
int Item::childCount() const
{
    ASSERT(m_def);
    return m_def->containerGroup().nodeCount(m_node);
}

// =============================================================================
// (public)
int Item::childCount(const std::string &name) const
{
    ASSERT(m_def);
    return m_def->container(name).nodeCount(m_node);
}

// =============================================================================
// (public)
int Item::childIndex(const Item& refChild) const
{
    ASSERT(m_def);
    return m_def->containerGroup().nodeIndex(m_node, refChild.node());
}

// =============================================================================
// (public)
int Item::childIndex(const std::string &name, const Item &refChild) const
{
    ASSERT(m_def);
    return m_def->container(name).nodeIndex(m_node, refChild.node());
}

// =============================================================================
// (public)
Item Item::childAt(int index) const
{
    ASSERT(m_def);
    const NodeDef* childeNodeDef;
    Node childNode = m_def->containerGroup().node(m_node, index, &childeNodeDef);
    return Item(childeNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::childAt(const std::string &name, int index) const
{
    ASSERT(m_def);
    const NodeDef* childeNodeDef;
    Node childNode = m_def->container(name).node(m_node, index, &childeNodeDef);
    return Item(childeNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::firstChild() const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->containerGroup().firstNode(m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::firstChild(const std::string &name) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->container(name).firstNode(m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::lastChild() const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->containerGroup().lastNode(m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::lastChild(const std::string &name) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->container(name).lastNode(m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::nextChild(const Item& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->containerGroup().nextNode(m_node, refChild.m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::nextChild(const std::string &name, const Item& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->container(name).nextNode(refChild.m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::previousChild(const Item& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->containerGroup().previousNode(m_node, refChild.m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::previousChild(const std::string &name, const Item& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->container(name).previousNode(refChild.m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::parent() const
{
    ASSERT(m_def);
    const NodeDef* parentNodeDef;
    Node parentNode = m_def->parentNode(m_node, &parentNodeDef);
    return Item(parentNodeDef, parentNode, m_model);
}

// =============================================================================
// (public)
bool Item::canInsertChild(const std::string &name, int &index) const
{
    ASSERT(m_def);
    return m_def->container(name).canInsertNode(m_node, index);
}

// =============================================================================
// (public)
Item Item::insertChild(const std::string &name, int &index) const
{
    ASSERT(m_def);
    const auto& container = m_def->container(name);
    Node node = container.insertNode(m_node, index);
    Item childItem(container.containerDef(), node, m_model);
    if (m_model && !childItem.isNull()) {
        ItemIndexUPtr iIndex = ItemIndex::create(childItem);
        m_model->onItemInserteAfter(*iIndex.get());
    }
    return childItem;
}

// =============================================================================
// (public)
bool Item::canCloneChild(int& index, const Item &cloneItem) const
{
    ASSERT(m_def);
    return m_def->containerGroup().canCloneNode(m_node, index, cloneItem.m_node);
}

// =============================================================================
// (public)
bool Item::canCloneChild(const std::string &name, int &index, const Item &cloneItem) const
{
    ASSERT(m_def);
    return m_def->container(name).canCloneNode(m_node, index, cloneItem.m_node);
}

// =============================================================================
// (public)
Item Item::cloneChild(int& index, const Item &cloneItem) const
{
    ASSERT(m_def);
    if (m_model) {
        // Cash data needed to notify change
        ItemIndexUPtr sourceItemIndex = ItemIndex::create(cloneItem);
        // Perform the cloneing
        Item item = Item(cloneItem.m_def, m_def->containerGroup().cloneNode(m_node, index, cloneItem.m_node), m_model);

        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            ItemIndexUPtr targetItemIndex = ItemIndex::create(item);
            m_model->onItemCloneAfter(*sourceItemIndex.get(), *targetItemIndex.get());
        }
        updateUniqueValues(item);

        return item;
    } else {
        return Item(cloneItem.m_def, m_def->containerGroup().cloneNode(m_node, index, cloneItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
Item Item::cloneChild(const std::string &name, int &index, const Item &cloneItem) const
{
    ASSERT(m_def);
    if (m_model) {
        // Cash data needed to notify change
        ItemIndexUPtr sourceItemIndex = ItemIndex::create(cloneItem);

        // Perform the cloneing
        Item item = Item(cloneItem.m_def, m_def->container(name).cloneNode(m_node, index, cloneItem.m_node), m_model);

        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            ItemIndexUPtr targetItemIndex = ItemIndex::create(item);
            m_model->onItemCloneAfter(*sourceItemIndex.get(), *targetItemIndex.get());
        }
        updateUniqueValues(item);

        return item;
    } else {
        return Item(cloneItem.m_def, m_def->container(name).cloneNode(m_node, index, cloneItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
bool Item::canMoveChild(int& index, const Item &moveItem) const
{
    ASSERT(m_def);
    return m_def->containerGroup().canMoveNode(m_node, index, moveItem.m_node);
}

// =============================================================================
// (public)
bool Item::canMoveChild(const std::string &name, int &index, const Item &moveItem) const
{
    ASSERT(m_def);
    return m_def->container(name).canMoveNode(m_node, index, moveItem.m_node);
}

// =============================================================================
// (public)
Item Item::moveChild(int& index, Item moveItem) const
{
    ASSERT(m_def);
    if (m_model) {
        // Check if item can be moved
        if (!m_def->containerGroup().canMoveNode(m_node, index, moveItem.m_node)) { return Item(); }

        // Cash data needed to notify change
        ItemIndexUPtr sourceItemIndex = ItemIndex::create(moveItem);
        ItemIndexUPtr targetItemIndex = ItemIndex::create(*this);
        targetItemIndex->setChildItemIndex(new ItemIndex(index));

        // Notify before move
        m_model->onItemMoveBefore(*sourceItemIndex.get(), *targetItemIndex.get());

        // Perform the move
        Item item = Item(moveItem.m_def, m_def->containerGroup().moveNode(m_node, index, moveItem.m_node), m_model);

        ASSERT(!item.isNull());

        targetItemIndex = ItemIndex::create(item);

        // Notify after move
        m_model->onItemMoveAfter(*sourceItemIndex.get(), *targetItemIndex.get());

        updateUniqueValues(item);

        return item;
    } else {
        return Item(moveItem.m_def, m_def->containerGroup().moveNode(m_node, index, moveItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
Item Item::moveChild(const std::string &name, int &index, Item moveItem) const
{
    ASSERT(m_def);
    if (m_model) {
        // Check if item can be moved
        if (!m_def->container(name).canMoveNode(m_node, index, moveItem.m_node)) { return Item(); }

        // Cash data needed to notify change
        ItemIndexUPtr sourceItemIndex = ItemIndex::create(moveItem);
        ItemIndexUPtr targetItemIndex = ItemIndex::create(*this);
        targetItemIndex->setChildItemIndex(new ItemIndex(name, index));

        // Notify before move
        m_model->onItemMoveBefore(*sourceItemIndex.get(), *targetItemIndex.get());

        // Perform the move
        Item item = Item(moveItem.m_def, m_def->container(name).moveNode(m_node, index, moveItem.m_node), m_model);

        ASSERT(!item.isNull());

        targetItemIndex = ItemIndex::create(item);

        // Notify after move
        m_model->onItemMoveAfter(*sourceItemIndex.get(), *targetItemIndex.get());

        updateUniqueValues(item);

        return item;
    } else {
        return Item(moveItem.m_def, m_def->container(name).moveNode(m_node, index, moveItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
bool Item::canRemoveChild(int index) const
{
    ASSERT(m_def);
    return m_def->containerGroup().canRemoveNode(m_node, index);
}

// =============================================================================
// (public)
bool Item::canRemoveChild(const std::string &name, int index) const
{
    ASSERT(m_def);
    return m_def->container(name).canRemoveNode(m_node, index);
}

// =============================================================================
// (public)
bool Item::removeChild(int index) const
{
    ASSERT(m_def);
    if (m_def->containerGroup().canRemoveNode(m_node, index)) {

        ItemIndexUPtr iIndex = ItemIndex::create(childAt(index));
        if (m_model) {
            m_model->onItemRemoveBefore(*iIndex.get());
        }

        m_def->containerGroup().removeNode(m_node, index);
        if (m_model) {
            m_model->onItemRemoveAfter(*iIndex.get());
        }
        return true;
    }
    return false;
}

// =============================================================================
// (public)
bool Item::removeChild(const std::string &name, int index) const
{
    ASSERT(m_def);
    if (m_def->container(name).canRemoveNode(m_node, index)) {

        ItemIndexUPtr iIndex = ItemIndex::create(childAt(name, index));
        if (m_model) {
            m_model->onItemRemoveBefore(*iIndex.get());
        }

        m_def->container(name).removeNode(m_node, index);
        if (m_model) {
            m_model->onItemRemoveAfter(*iIndex.get());
        }
        return true;
    }
    return false;
}

// =============================================================================
// (public)
int Item::convertChildIndexToUnnamed(const std::string &name, int index) const
{
    if (m_def->containerCount() == 1) { return index; }
    Item childItem = childAt(name, index);
    return childIndex(childItem);
}

// =============================================================================
// (public)
int Item::convertChildIndexToNamed(std::string &name, int index) const
{
    if (m_def->containerCount() == 1) {
        name = m_def->container(0).containerDef()->name();
        return index;
    }
    Item childItem = childAt(index);
    name = childItem.def()->name();
    return childIndex(name, childItem);
}

// =============================================================================
// (protected)
void Item::initEntryList() const
{
    ASSERT(m_def);
    if (m_entryList.empty() && m_def && !m_node.isNull()) {
        auto vList = m_def->valueList();
        for (const ValueDef* vi: vList) {
            m_entryList.push_back(Entry(vi, m_node, this));
        }
    }
}

// =============================================================================
// (protected)
void Item::updateUniqueValues(Item item)
{
    Model::Item::entryIterator vIt = item.entryBegin();
    Model::Item::entryIterator vItEnd = item.entryEnd();
    while (vIt != vItEnd) {
        if (vIt->settings().value(REQUIRED) > 0 &&
            vIt->settings().value(UNIQUE) > 0 &&
            vIt->hasDefaultValue()) {

            std::vector<std::string> valueList = QB::createParent()->children(item.def()->name())->EntryUPtr(vIt->name())->toValueList<std::string>(item);
            std::string value = vIt->value<std::string>();
            if (count(valueList, value) == 1) {
                return;
            }

            std::string defaultValue = vIt->defaultValue<std::string>();
            value = defaultValue;
            int i = 1;
            while (contains(valueList, value)) {
                value = defaultValue + "_" + std::to_string(i++);
            }
            vIt->setValue(value);
        }

        vIt++;
    }
}

} // namespace Model
} // namespace Oak
