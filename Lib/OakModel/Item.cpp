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
#include "LeafQuery.h"
#include "OakModelServiceFunctions.h"
#include "QueryBuilder.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (public)
Item::Item()
    : m_def(nullptr),
      m_model(nullptr)
{

}

// =============================================================================
// (public)
Item::Item(const NodeDef* nodeDef, const NodeData &nodeData, const OakModel* model)
    : m_nodeData(nodeData),
      m_model(model)
{
    if (nodeDef == nullptr || nodeData.isNull()) {
        m_def = nodeDef;
    } else {
        m_def = nodeDef->validVariant(nodeData);
    }
}

// =============================================================================
// (public)
Item::Item(const Item& copy)
    : m_def(copy.m_def),
      m_nodeData(copy.m_nodeData),
      m_model(copy.m_model),
      m_leafList(copy.m_leafList)
{

}

// =============================================================================
// (public)
Item::Item(Item &&move)
    : m_def(std::move(move.m_def)),
      m_nodeData(std::move(move.m_nodeData)),
      m_model(move.m_model),
      m_leafList(std::move(move.m_leafList))
{

}

// =============================================================================
// (public)
Item& Item::operator=(const Item& copy)
{
    m_def = copy.m_def;
    m_nodeData = copy.m_nodeData;
    m_model = copy.m_model;

    if (m_def == copy.m_def) {  // NodeDef is the same: Leaf list is still valid
        if (!m_leafList.empty()) { // Leaf list is initialized
            // Update node for leaf lists
            for (auto &leaf: m_leafList) {
                leaf.m_nodeData = m_nodeData;
            }
        }
    } else {
        m_leafList.assign(copy.m_leafList.begin(), copy.m_leafList.end());
    }

    return *this;
}

// =============================================================================
// (public)
Item& Item::operator=(Item&& move)
{
    m_leafList = std::move(move.m_leafList);
    m_def = std::move(move.m_def);
    m_nodeData = std::move(move.m_nodeData);
    m_model = move.m_model;
    return *this;
}

// =============================================================================
// (public)
bool Item::operator==(const Item& _item) const
{
    return m_def == _item.m_def && m_nodeData == _item.m_nodeData;
}

// =============================================================================
// (public)
bool Item::operator!=(const Item& _item) const
{
    return !(operator==(_item));
}

// =============================================================================
// (public)
const Leaf& Item::operator()(const std::string &valueName) const
{
    return leaf(valueName);
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
    return m_nodeData.isNull();
}

// =============================================================================
// (public)
void Item::clear()
{
    m_def = nullptr;
    m_nodeData.clear();
    m_model = nullptr;
    m_leafList.clear();
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
    initLeafList();
    std::vector<std::string> nameList;
    for (const Leaf& iv: m_leafList)
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
int Item::leafCount() const
{
    initLeafList();
    return static_cast<int>(m_leafList.size());
}

// =============================================================================
// (public)
bool Item::hasLeaf(const std::string &leafName) const
{
    return m_def->hasValue(leafName);
}

// =============================================================================
// (public)
int Item::leafIndex(const Leaf &leaf) const
{
    initLeafList();
    auto it = std::find(m_leafList.begin(), m_leafList.end(), leaf);
    if (it == m_leafList.end()) {
        return -1;
    } else {
        return static_cast<int>(std::distance(m_leafList.begin(), it));
    }
}

// =============================================================================
// (public)
const Leaf& Item::leafAt(int index) const
{
    initLeafList();
    return m_leafList.at(static_cast<vSize>(index));
}

// =============================================================================
// (public)
Leaf &Item::leafAt(int index)
{
    initLeafList();
    return m_leafList.at(static_cast<vSize>(index));
}

// =============================================================================
// (public)
const Leaf& Item::leaf(const std::string &leafName) const
{
    initLeafList();
    for (const Leaf& iv: m_leafList)
    {
        if (iv.name() == leafName) {
            return iv;
        }
    }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
Leaf &Item::leaf(const std::string &leafName)
{
    initLeafList();
    for (Leaf& iv: m_leafList)
    {
        if (iv.name() == leafName) {
            return iv;
        }
    }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
const Item::LeafIterator Item::leafBegin() const
{
    initLeafList();
    return m_leafList.begin();
}

// =============================================================================
// (public)
Item::LeafIterator Item::leafBegin()
{
    initLeafList();
    return m_leafList.begin();
}

// =============================================================================
// (public)
const Item::LeafIterator Item::leafEnd() const
{
    initLeafList();
    return m_leafList.end();
}

// =============================================================================
// (public)
Item::LeafIterator Item::leafEnd()
{
    initLeafList();
    return m_leafList.end();
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
const Leaf& Item::keyLeaf() const
{
    initLeafList();
    if (hasKey()) { return m_leafList[static_cast<vSize>(m_def->indexOfKeyLeafDef())]; }
    return Leaf::emptyLeaf();
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
const Leaf& Item::variantLeaf() const
{
    initLeafList();
    if (hasVariants()) { return m_leafList[static_cast<vSize>(m_def->indexOfVariantLeafDef())]; }
    return Leaf::emptyLeaf();
}

// =============================================================================
// (public)
int Item::childCount() const
{
    ASSERT(m_def);
    return m_def->containerGroup().nodeCount(m_nodeData);
}

// =============================================================================
// (public)
int Item::childCount(const std::string &name) const
{
    ASSERT(m_def);
    return m_def->container(name).nodeCount(m_nodeData);
}

// =============================================================================
// (public)
int Item::childIndex(const Item& refChild) const
{
    ASSERT(m_def);
    return m_def->containerGroup().nodeIndex(m_nodeData, refChild.nodeData());
}

// =============================================================================
// (public)
int Item::childIndex(const std::string &name, const Item &refChild) const
{
    ASSERT(m_def);
    return m_def->container(name).nodeIndex(m_nodeData, refChild.nodeData());
}

// =============================================================================
// (public)
Item Item::childAt(int index) const
{
    ASSERT(m_def);
    const NodeDef* childeNodeDef;
    NodeData childNodeData = m_def->containerGroup().node(m_nodeData, index, &childeNodeDef);
    return Item(childeNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::childAt(const std::string &name, int index) const
{
    ASSERT(m_def);
    const NodeDef* childeNodeDef;
    NodeData childNodeData = m_def->container(name).node(m_nodeData, index, &childeNodeDef);
    return Item(childeNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::firstChild() const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->containerGroup().firstNode(m_nodeData, &childNodeDef);
    return Item(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::firstChild(const std::string &name) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->container(name).firstNode(m_nodeData, &childNodeDef);
    return Item(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::lastChild() const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->containerGroup().lastNode(m_nodeData, &childNodeDef);
    return Item(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::lastChild(const std::string &name) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->container(name).lastNode(m_nodeData, &childNodeDef);
    return Item(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::nextChild(const Item& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->containerGroup().nextNode(m_nodeData, refChild.m_nodeData, &childNodeDef);
    return Item(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::nextChild(const std::string &name, const Item& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->container(name).nextNode(refChild.m_nodeData, &childNodeDef);
    return Item(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::previousChild(const Item& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->containerGroup().previousNode(m_nodeData, refChild.m_nodeData, &childNodeDef);
    return Item(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::previousChild(const std::string &name, const Item& refChild) const
{
    ASSERT(m_def);
    const NodeDef* childNodeDef;
    NodeData childNodeData = m_def->container(name).previousNode(refChild.m_nodeData, &childNodeDef);
    return Item(childNodeDef, childNodeData, m_model);
}

// =============================================================================
// (public)
Item Item::parent() const
{
    ASSERT(m_def);
    const NodeDef* parentNodeDef;
    NodeData parentNodeData = m_def->parentNode(m_nodeData, &parentNodeDef);
    return Item(parentNodeDef, parentNodeData, m_model);
}

// =============================================================================
// (public)
bool Item::canInsertChild(const std::string &name, int &index) const
{
    ASSERT(m_def);
    return m_def->container(name).canInsertNode(m_nodeData, index);
}

// =============================================================================
// (public)
Item Item::insertChild(const std::string &name, int &index) const
{
    ASSERT(m_def);
    const auto& container = m_def->container(name);
    NodeData nodeData = container.insertNode(m_nodeData, index);
    Item childItem(container.containerDef(), nodeData, m_model);
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
    return m_def->containerGroup().canCloneNode(m_nodeData, index, cloneItem.m_nodeData);
}

// =============================================================================
// (public)
bool Item::canCloneChild(const std::string &name, int &index, const Item &cloneItem) const
{
    ASSERT(m_def);
    return m_def->container(name).canCloneNode(m_nodeData, index, cloneItem.m_nodeData);
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
        Item item = Item(cloneItem.m_def, m_def->containerGroup().cloneNode(m_nodeData, index, cloneItem.m_nodeData), m_model);

        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            ItemIndexUPtr targetItemIndex = ItemIndex::create(item);
            m_model->onItemCloneAfter(*sourceItemIndex.get(), *targetItemIndex.get());
        }
        updateUniqueValues(item);

        return item;
    } else {
        return Item(cloneItem.m_def, m_def->containerGroup().cloneNode(m_nodeData, index, cloneItem.m_nodeData), m_model);
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
        Item item = Item(cloneItem.m_def, m_def->container(name).cloneNode(m_nodeData, index, cloneItem.m_nodeData), m_model);

        // Notify everyone if cloning did not fail
        if (!item.isNull()) {
            ItemIndexUPtr targetItemIndex = ItemIndex::create(item);
            m_model->onItemCloneAfter(*sourceItemIndex.get(), *targetItemIndex.get());
        }
        updateUniqueValues(item);

        return item;
    } else {
        return Item(cloneItem.m_def, m_def->container(name).cloneNode(m_nodeData, index, cloneItem.m_nodeData), m_model);
    }
}

// =============================================================================
// (public)
bool Item::canMoveChild(int& index, const Item &moveItem) const
{
    ASSERT(m_def);
    return m_def->containerGroup().canMoveNode(m_nodeData, index, moveItem.m_nodeData);
}

// =============================================================================
// (public)
bool Item::canMoveChild(const std::string &name, int &index, const Item &moveItem) const
{
    ASSERT(m_def);
    return m_def->container(name).canMoveNode(m_nodeData, index, moveItem.m_nodeData);
}

// =============================================================================
// (public)
Item Item::moveChild(int& index, const Item &moveItem) const
{
    ASSERT(m_def);
    if (m_model) {
        // Check if item can be moved
        if (!m_def->containerGroup().canMoveNode(m_nodeData, index, moveItem.m_nodeData)) { return Item(); }

        // Cash data needed to notify change
        ItemIndexUPtr sourceItemIndex = ItemIndex::create(moveItem);
        ItemIndexUPtr targetItemIndex = ItemIndex::create(*this);
        targetItemIndex->setChildItemIndex(new ItemIndex(index));

        // Notify before move
        m_model->onItemMoveBefore(*sourceItemIndex.get(), *targetItemIndex.get());

        // Perform the move
        Item item = Item(moveItem.m_def, m_def->containerGroup().moveNode(m_nodeData, index, moveItem.m_nodeData), m_model);

        ASSERT(!item.isNull());

        targetItemIndex = ItemIndex::create(item);

        // Notify after move
        m_model->onItemMoveAfter(*sourceItemIndex.get(), *targetItemIndex.get());

        updateUniqueValues(item);

        return item;
    } else {
        return Item(moveItem.m_def, m_def->containerGroup().moveNode(m_nodeData, index, moveItem.m_nodeData), m_model);
    }
}

// =============================================================================
// (public)
Item Item::moveChild(const std::string &name, int &index, const Item &moveItem) const
{
    ASSERT(m_def);
    if (m_model) {
        // Check if item can be moved
        if (!m_def->container(name).canMoveNode(m_nodeData, index, moveItem.m_nodeData)) { return Item(); }

        // Cash data needed to notify change
        ItemIndexUPtr sourceItemIndex = ItemIndex::create(moveItem);
        ItemIndexUPtr targetItemIndex = ItemIndex::create(*this);
        targetItemIndex->setChildItemIndex(new ItemIndex(name, index));

        // Notify before move
        m_model->onItemMoveBefore(*sourceItemIndex.get(), *targetItemIndex.get());

        // Perform the move
        Item item = Item(moveItem.m_def, m_def->container(name).moveNode(m_nodeData, index, moveItem.m_nodeData), m_model);

        ASSERT(!item.isNull());

        targetItemIndex = ItemIndex::create(item);

        // Notify after move
        m_model->onItemMoveAfter(*sourceItemIndex.get(), *targetItemIndex.get());

        updateUniqueValues(item);

        return item;
    } else {
        return Item(moveItem.m_def, m_def->container(name).moveNode(m_nodeData, index, moveItem.m_nodeData), m_model);
    }
}

// =============================================================================
// (public)
bool Item::canRemoveChild(int index) const
{
    ASSERT(m_def);
    return m_def->containerGroup().canRemoveNode(m_nodeData, index);
}

// =============================================================================
// (public)
bool Item::canRemoveChild(const std::string &name, int index) const
{
    ASSERT(m_def);
    return m_def->container(name).canRemoveNode(m_nodeData, index);
}

// =============================================================================
// (public)
bool Item::removeChild(int index) const
{
    ASSERT(m_def);
    if (m_def->containerGroup().canRemoveNode(m_nodeData, index)) {

        ItemIndexUPtr iIndex = ItemIndex::create(childAt(index));
        if (m_model) {
            m_model->onItemRemoveBefore(*iIndex.get());
        }

        m_def->containerGroup().removeNode(m_nodeData, index);
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
    if (m_def->container(name).canRemoveNode(m_nodeData, index)) {

        ItemIndexUPtr iIndex = ItemIndex::create(childAt(name, index));
        if (m_model) {
            m_model->onItemRemoveBefore(*iIndex.get());
        }

        m_def->container(name).removeNode(m_nodeData, index);
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
void Item::initLeafList() const
{
    ASSERT(m_def);
    if (m_leafList.empty() && m_def && !m_nodeData.isNull()) {
        auto vList = m_def->valueList();
        for (const LeafDef* vi: vList) {
            m_leafList.push_back(Leaf(vi, m_nodeData, this));
        }
    }
}

// =============================================================================
// (protected)
void Item::updateUniqueValues(const Item &item)
{
    Model::Item::LeafIterator vIt = item.leafBegin();
    Model::Item::LeafIterator vItEnd = item.leafEnd();
    while (vIt != vItEnd) {
        if (vIt->settings().value(REQUIRED) > 0 &&
            vIt->settings().value(UNIQUE) > 0 &&
            vIt->hasDefaultValue()) {

            std::vector<std::string> valueList = QB::createParent()->children(item.def()->name())->leafSPtr(vIt->name())->toValueList<std::string>(item);
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

} // namespace Oak::Model

