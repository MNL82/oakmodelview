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

#include "OakModel.h"
#include "QueryBase.h"
#include "ServiceFunctions.h"

#include <algorithm>

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
    : m_def(nodeDef),
      m_node(node),
      m_model(model)
{

}

// =============================================================================
// (public)
Item::Item(const Item& copy)
    : m_def(copy.m_def),
      m_node(copy.m_node),
      m_itemValueList(copy.m_itemValueList),
      m_model(copy.m_model)
{

}

// =============================================================================
// (public)
Item::Item(Item&& move)
    : m_def(std::move(move.m_def)),
      m_node(std::move(move.m_node)),
      m_itemValueList(std::move(move.m_itemValueList)),
      m_model(move.m_model)
{

}

// =============================================================================
// (public)
Item& Item::operator=(const Item& copy)
{
    if (m_def != copy.m_def) {
        m_itemValueList.assign(copy.m_itemValueList.begin(), copy.m_itemValueList.end());
    }
    m_def = copy.m_def;
    m_node = copy.m_node;
    m_model = copy.m_model;
    return *this;
}

// =============================================================================
// (public)
Item& Item::operator=(Item&& move)
{
    m_itemValueList = std::move(move.m_itemValueList);
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
const ItemValue& Item::operator()(const std::string &valueName) const
{
    return value(valueName);
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
    m_itemValueList.clear();
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
    assert(m_def);
    if (m_model) {
        m_model->setCurrentItem(*this);
    }
}

// =============================================================================
// (public)
std::vector<std::string> Item::valueNameList() const
{
    initItemValueList();
    std::vector<std::string> nameList;
    for (const ItemValue& iv: m_itemValueList)
    {
        nameList.push_back(iv.name());
    }
    return std::move(nameList);
}

// =============================================================================
// (public)
std::vector<std::string> Item::childNameList() const
{
    assert(m_def);
    std::vector<const ContainerDef*> containerList;
    m_def->getContainerList(containerList);
    std::vector<std::string> nameList;
    for (const ContainerDef* container: containerList)
    {
        nameList.push_back(container->containerDef()->name());
    }
    return std::move(nameList);
}

// =============================================================================
// (public)
int Item::valueCount() const
{
    initItemValueList();
    return (int)m_itemValueList.size();
}

// =============================================================================
// (public)
const bool Item::hasValue(const std::string &valueName) const
{
    return m_def->hasValue(valueName);
}

// =============================================================================
// (public)
int Item::valueIndex(const ItemValue &value) const
{
    initItemValueList();
    auto it = std::find(m_itemValueList.begin(), m_itemValueList.end(), value);
    if (it == m_itemValueList.end()) {
        return -1;
    } else {
        return (int)std::distance(m_itemValueList.begin(), it);
    }
}

// =============================================================================
// (public)
const ItemValue& Item::valueAt(int index) const
{
    initItemValueList();
    return m_itemValueList.at(index);
}

// =============================================================================
// (public)
const ItemValue& Item::value(const std::string &valueName) const
{
    initItemValueList();
    for (const ItemValue& iv: m_itemValueList)
    {
        if (iv.name() == valueName) {
            return iv;
        }
    }
    return ItemValue::emptyItemValue();
}

// =============================================================================
// (public)
Item::ValueIterator Item::valueBegin() const
{
    initItemValueList();
    return m_itemValueList.begin();
}

// =============================================================================
// (public)
Item::ValueIterator Item::valueEnd() const
{
    initItemValueList();
    return m_itemValueList.end();
}

// =============================================================================
// (public)
bool Item::hasKey() const
{
    assert(m_def);
    return m_def->hasKey();
}

// =============================================================================
// (public)
const ItemValue& Item::valueKey() const
{
    initItemValueList();
    if (hasKey()) { return m_itemValueList[m_def->keyValueDefIndex()]; }
    return ItemValue::emptyItemValue();
}

// =============================================================================
// (public)
bool Item::hasDerivedId() const
{
    assert(m_def);
    return m_def->hasDerivedId();
}

// =============================================================================
// (public)
const ItemValue& Item::valueDerivedId() const
{
    initItemValueList();
    if (hasDerivedId()) { return m_itemValueList[m_def->derivedIdValueDefIndex()]; }
    return ItemValue::emptyItemValue();
}

// =============================================================================
// (public)
int Item::childCount() const
{
    assert(m_def);
    return m_def->containerGroup().nodeCount(m_node);
}

// =============================================================================
// (public)
int Item::childCount(const std::string &name) const
{
    assert(m_def);
    return m_def->container(name).nodeCount(m_node);
}

// =============================================================================
// (public)
int Item::childIndex(const Item& refChild) const
{
    assert(m_def);
    return m_def->containerGroup().nodeIndex(m_node, refChild.node());
}

// =============================================================================
// (public)
int Item::childIndex(const std::string &name, const Item &refChild) const
{
    assert(m_def);
    return m_def->container(name).nodeIndex(m_node, refChild.node());
}

// =============================================================================
// (public)
Item Item::childAt(int index) const
{
    assert(m_def);
    const NodeDef* childeNodeDef;
    Node childNode = m_def->containerGroup().node(m_node, index, &childeNodeDef);
    return Item(childeNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::childAt(const std::string &name, int index) const
{
    assert(m_def);
    const NodeDef* childeNodeDef;
    Node childNode = m_def->container(name).node(m_node, index, &childeNodeDef);
    return Item(childeNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::firstChild() const
{
    assert(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->containerGroup().firstNode(m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::firstChild(const std::string &name) const
{
    assert(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->container(name).firstNode(m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::lastChild() const
{
    assert(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->containerGroup().lastNode(m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::lastChild(const std::string &name) const
{
    assert(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->container(name).lastNode(m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::nextChild(const Item& refChild) const
{
    assert(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->containerGroup().nextNode(m_node, refChild.m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::nextChild(const std::string &name, const Item& refChild) const
{
    assert(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->container(name).nextNode(refChild.m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::previousChild(const Item& refChild) const
{
    assert(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->containerGroup().previousNode(m_node, refChild.m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::previousChild(const std::string &name, const Item& refChild) const
{
    assert(m_def);
    const NodeDef* childNodeDef;
    Node childNode = m_def->container(name).previousNode(refChild.m_node, &childNodeDef);
    return Item(childNodeDef, childNode, m_model);
}

// =============================================================================
// (public)
Item Item::parent() const
{
    assert(m_def);
    const NodeDef* parentNodeDef;
    Node parentNode = m_def->parentNode(m_node, &parentNodeDef);
    return Item(parentNodeDef, parentNode, m_model);
}

// =============================================================================
// (public)
bool Item::canInsertChild(const std::string &name, int &index) const
{
    assert(m_def);
    return m_def->container(name).canInsertNode(m_node, index);
}

// =============================================================================
// (public)
Item Item::insertChild(const std::string &name, int &index) const
{
    assert(m_def);
    const auto& container = m_def->container(name);
    Item childItem(container.containerDef(), container.insertNode(m_node, index), m_model);
    if (m_model && !childItem.isNull()) {
        onItemInserted(*this, childIndex(childItem));
    }
    return std::move(childItem);
}

// =============================================================================
// (public)
bool Item::canCloneChild(int& index, Item cloneItem) const
{
    assert(m_def);
    return m_def->containerGroup().canCloneNode(m_node, index, cloneItem.m_node);
}

// =============================================================================
// (public)
bool Item::canCloneChild(const std::string &name, int &index, Item cloneItem) const
{
    assert(m_def);
    return m_def->container(name).canCloneNode(m_node, index, cloneItem.m_node);
}

// =============================================================================
// (public)
Item Item::cloneChild(int& index, Item cloneItem) const
{
    assert(m_def);
    if (m_model) {
        // Cash data needed to notify change
        Item sourceParentItem = cloneItem.parent();
        int sourceIndex = sourceParentItem.childIndex(cloneItem);
        // Perform the cloneing
        Item item = Item(cloneItem.m_def, m_def->containerGroup().cloneNode(m_node, index, cloneItem.m_node), m_model);
        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            m_model->onItemCloned(sourceParentItem, sourceIndex, *this, index);
        }
        updateUniqueValues(item);

        return std::move(item);
    } else {
        return Item(cloneItem.m_def, m_def->containerGroup().cloneNode(m_node, index, cloneItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
Item Item::cloneChild(const std::string &name, int &index, Item cloneItem) const
{
    assert(m_def);
    if (m_model) {
        // Cash data needed to notify change
        Item sourceParentItem = cloneItem.parent();
        int sourceIndex = sourceParentItem.childIndex(cloneItem);
        // Perform the cloneing
        Item item = Item(cloneItem.m_def, m_def->container(name).cloneNode(m_node, index, cloneItem.m_node), m_model);
        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            onItemCloned(sourceParentItem, sourceIndex, *this, childIndex(item));
        }
        updateUniqueValues(item);

        return std::move(item);
    } else {
        return Item(cloneItem.m_def, m_def->container(name).cloneNode(m_node, index, cloneItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
bool Item::canMoveChild(int& index, Item moveItem) const
{
    assert(m_def);
    return m_def->containerGroup().canMoveNode(m_node, index, moveItem.m_node);
}

// =============================================================================
// (public)
bool Item::canMoveChild(const std::string &name, int &index, Item moveItem) const
{
    assert(m_def);
    return m_def->container(name).canMoveNode(m_node, index, moveItem.m_node);
}

// =============================================================================
// (public)
Item Item::moveChild(int& index, Item moveItem) const
{
    assert(m_def);
    if (m_model) {
        // Cash data needed to notify change
        Item sourceParentItem = moveItem.parent();
        int sourceIndex = sourceParentItem.childIndex(moveItem);
        // Perform the cloneing
        Item item = Item(moveItem.m_def, m_def->containerGroup().moveNode(m_node, index, moveItem.m_node), m_model);
        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            m_model->onItemMoved(sourceParentItem, sourceIndex, *this, index);
        }
        updateUniqueValues(item);

        return std::move(item);
    } else {
        return Item(moveItem.m_def, m_def->containerGroup().moveNode(m_node, index, moveItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
Item Item::moveChild(const std::string &name, int &index, Item moveItem) const
{
    assert(m_def);
    if (m_model) {
        // Cash data needed to notify change
        Item sourceParentItem = moveItem.parent();
        int sourceIndex = sourceParentItem.childIndex(moveItem);
        // Perform the cloneing
        Item item = Item(moveItem.m_def, m_def->container(name).moveNode(m_node, index, moveItem.m_node), m_model);
        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            onItemMoved(sourceParentItem, sourceIndex, *this, childIndex(item));
        }
        updateUniqueValues(item);

        return std::move(item);
    } else {
        return Item(moveItem.m_def, m_def->container(name).moveNode(m_node, index, moveItem.m_node), m_model);
    }
}

// =============================================================================
// (public)
bool Item::canRemoveChild(int index) const
{
    assert(m_def);
    return m_def->containerGroup().canRemoveNode(m_node, index);
}

// =============================================================================
// (public)
bool Item::canRemoveChild(const std::string &name, int index) const
{
    assert(m_def);
    return m_def->container(name).canRemoveNode(m_node, index);
}

// =============================================================================
// (public)
bool Item::removeChild(int index) const
{
    assert(m_def);
    if (m_def->containerGroup().removeNode(m_node, index)) {
        if (m_model) {
            m_model->onItemRemoved(*this, index);
        }
        return true;
    }
    return false;
}

// =============================================================================
// (public)
bool Item::removeChild(const std::string &name, int index) const
{
    assert(m_def);
    int index2 = childIndex(childAt(name, index));
    if (m_def->container(name).removeNode(m_node, index)) {
        if (m_model) {
            onItemRemoved(*this, index2);
        }
        return true;
    }
    return false;
}

// =============================================================================
// (protected)
void Item::initItemValueList() const
{
    assert(m_def);
    if (m_itemValueList.empty() && m_def && !m_node.isNull()) {
        auto vList = m_def->valueList();
        for (const ValueDef* vi: vList) {
            m_itemValueList.push_back(ItemValue(vi, m_node, this));
        }
    }
}

// =============================================================================
// (protected)
void Item::onItemInserted(const Item &parentItem, int index) const
{
    assert(m_model);
    m_model->onItemInserted(parentItem, index);
}

// =============================================================================
// (protected)
void Item::onItemMoved(const Item &sourceParentItem, int sourceIndex, const Item &targetParentItem, int targetIndex) const
{
    assert(m_model);
    m_model->onItemMoved(sourceParentItem, sourceIndex, targetParentItem, targetIndex);
}

// =============================================================================
// (protected)
void Item::onItemCloned(const Item &sourceParentItem, int sourceIndex, const Item &targetParentItem, int targetIndex) const
{
    assert(m_model);
    m_model->onItemCloned(sourceParentItem, sourceIndex, targetParentItem, targetIndex);
}

// =============================================================================
// (protected)
void Item::onItemRemoved(const Item &parentItem, int index) const
{
    assert(m_model);
    m_model->onItemRemoved(parentItem, index);
}

// =============================================================================
// (protected)
void Item::updateUniqueValues(Item item)
{
    Model::Item::ValueIterator vIt = item.valueBegin();
    Model::Item::ValueIterator vItEnd = item.valueEnd();
    while (vIt != vItEnd) {
        if (vIt->settings().required() &&
            vIt->settings().unique() &&
            vIt->hasDefaultValue()) {

            std::vector<std::string> valueList = QueryBase::MakeSPtr(item)->parent()->children(item.def()->name())->toList<std::string>(vIt->name());
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
