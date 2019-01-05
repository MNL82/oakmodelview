/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerGroupDef.h"

#include "NodeDef.h"
#include "ContainerDef.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (public)
ContainerGroupDef::ContainerGroupDef(const NodeDef *hostDef)
{
    ASSERT(hostDef);
    m_hostDef = hostDef;

    updateContainerList();
}

// =============================================================================
// (public)
ContainerGroupDef::ContainerGroupDef(const ContainerGroupDef &copy)
    : m_hostDef(copy.m_hostDef),
      m_containerList(copy.m_containerList)
{
}

// =============================================================================
// (public)
ContainerGroupDef::ContainerGroupDef(ContainerGroupDef &&move)
    : m_hostDef(std::move(move.m_hostDef)),
      m_containerList(std::move(move.m_containerList))
{
}

// =============================================================================
// (public)
ContainerGroupDef&  ContainerGroupDef::operator=(const ContainerGroupDef &copy)
{
    m_hostDef = copy.m_hostDef;
    m_containerList.assign(copy.m_containerList.begin(), copy.m_containerList.end());
    return *this;
}

// =============================================================================
// (public)
ContainerGroupDef& ContainerGroupDef::operator=(ContainerGroupDef &&move)
{
    m_hostDef = std::move(move.m_hostDef);
    m_containerList = std::move(move.m_containerList);
    return *this;
}

// =============================================================================
// (public)
ContainerGroupDefUPtr ContainerGroupDef::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
const NodeDef* ContainerGroupDef::containerDef(const std::string &name, const UnionRef& variantId) const
{
    return m_hostDef->container(name).containerDef(variantId);
}

// =============================================================================
// (public)
const NodeDef* ContainerGroupDef::containerDef(const NodeData &_nodeData) const
{
    const ContainerDef& ci = m_hostDef->container(_nodeData);
    if (ci.isNull()) { return nullptr; }
    return ci.containerDef(_nodeData);
}

// =============================================================================
// (public)
bool ContainerGroupDef::validate(const NodeData &_nodeData) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(_nodeData)) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// (public)
int ContainerGroupDef::nodeCount(const NodeData &_nodeData) const
{
    int count = 0;
    for (auto child: m_containerList) {
        count += child->nodeCount(_nodeData);
    }

    return count;
}

// =============================================================================
// (public)
int ContainerGroupDef::nodeIndex(const NodeData &_nodeData, const NodeData &refNode) const
{
    int index = 0;
    for (auto container: m_containerList)
    {
        if (container->validate(refNode)) {
            return index + container->nodeIndex(_nodeData, refNode);
        } else {
            // Adds all nodes to the index that belongs to an earlier Container
            index += container->nodeCount(_nodeData);
        }
    }
    return -1;
}

// =============================================================================
// (public)
NodeData ContainerGroupDef::node(const NodeData &_nodeData, int index, const NodeDef** nodeDef) const
{
    for (auto container: m_containerList)
    {
        NodeData childNodeData = container->node(_nodeData, index, nodeDef);
        if (childNodeData.isNull()) {
            index -= container->nodeCount(_nodeData);
        } else {
            return childNodeData;
        }
    }
    if (nodeDef) { *nodeDef = nullptr; }
    return NodeData();
}

// =============================================================================
// (public)
NodeData ContainerGroupDef::firstNode(const NodeData &_nodeData, const NodeDef** nodeDef) const
{
    for (auto container: m_containerList)
    {
        NodeData childNodeData = container->firstNode(_nodeData, nodeDef);
        if (!childNodeData.isNull()) {
            return childNodeData;
        }
    }
    if (nodeDef) { *nodeDef = nullptr; }
    return NodeData();
}

// =============================================================================
// (public)
NodeData ContainerGroupDef::lastNode(const NodeData &_nodeData, const NodeDef** nodeDef) const
{
    auto it = m_containerList.end();
    while (it != m_containerList.begin()) {
        it--;
        NodeData childNodeData = (*it)->lastNode(_nodeData, nodeDef);
        if (!childNodeData.isNull()) {
            return childNodeData;
        }
    }
    if (nodeDef) { *nodeDef = nullptr; }
    return NodeData();
}

// =============================================================================
// (public)
NodeData ContainerGroupDef::nextNode(const NodeData &_nodeData, const NodeData &refNode, const NodeDef** nodeDef) const
{
    auto it = m_containerList.begin();
    while (it != m_containerList.end()) {
        if ((*it)->nodeIndex(_nodeData, refNode) != -1) {
            NodeData childNodeData = (*it)->nextNode(refNode, nodeDef);
            if (childNodeData.isNull()) {
                // Faild to find next node in the same Container
                // Looking in the next one ...
                it++;
                while (it != m_containerList.end()) {
                    childNodeData = (*it)->firstNode(_nodeData, nodeDef);
                    if (!childNodeData.isNull()) {
                        // Found the next node as the fist node in an other Container
                        return childNodeData;
                    }
                    it++;
                }
                // Found the refNode but there is no next node
                return NodeData();
            } else {
                return childNodeData;
            }
        }
        it++;
    }
    // Never found the refNode
    ASSERT(false);
    if (nodeDef) { *nodeDef = nullptr; }
    return NodeData();
}

// =============================================================================
// (public)
NodeData ContainerGroupDef::previousNode(const NodeData &_nodeData, const NodeData &refNode, const NodeDef** nodeDef) const
{
    auto it = m_containerList.end();
    while (it != m_containerList.begin()) {
        it--;
        if ((*it)->nodeIndex(_nodeData, refNode) != -1) {
            NodeData childNodeData = (*it)->nextNode(refNode, nodeDef);
            if (childNodeData.isNull()) {
                // Faild to find previous node in the same Container
                // Looking in the previous one ...
                while (it != m_containerList.begin()) {
                    it--;
                    childNodeData = (*it)->lastNode(_nodeData, nodeDef);
                    if (!childNodeData.isNull()) {
                        // Found the previous node as the last node in an other Container
                        return childNodeData;
                    }
                }
                // Found the refNode but there is no previous node
                return NodeData();
            } else {
                return childNodeData;
            }
        }
    }
    // Never found the refNode
    ASSERT(false);
    if (nodeDef) { *nodeDef = nullptr; }
    return NodeData();
}

// =============================================================================
// (public)
bool ContainerGroupDef::canInsertNode(const NodeData &_nodeData, const UnionRef &name, int &index) const
{
    for (auto container: m_containerList)
    {
        if (name == container->containerDef()->name()) {
            return container->canInsertNode(_nodeData, index);
        } else {
            index -= container->nodeCount(_nodeData);
        }
    }
    // Failed to find a Container with the given name
    ASSERT(false);
    return false;
}

// =============================================================================
// (public)
NodeData ContainerGroupDef::insertNode(const NodeData &_nodeData, const UnionRef &name, int &index) const
{
    for (auto container: m_containerList)
    {
        if (name == container->containerDef()->name()) {
            return container->insertNode(_nodeData, index);
        } else {
            index -= container->nodeCount(_nodeData);
        }
    }
    // Failed to find a Container with the given name
    ASSERT(false);
    return NodeData();
}

// =============================================================================
// (public)
bool ContainerGroupDef::canCloneNode(const NodeData &_nodeData, int &index, const NodeData &cloneNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(cloneNode)) {
            return container->canCloneNode(_nodeData, index, cloneNode);
        } else {
            index -= container->nodeCount(_nodeData);
        }
    }
    // Failed to find a Container with the given name
    return false;
}

// =============================================================================
// (public)
NodeData ContainerGroupDef::cloneNode(const NodeData &_nodeData, int &index, const NodeData &cloneNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(cloneNode)) {
            return container->cloneNode(_nodeData, index, cloneNode);
        } else {
            index -= container->nodeCount(_nodeData);
        }
    }
    // Failed to find a Container with the given name
    ASSERT(false);
    return NodeData();
}

// =============================================================================
// (public)
bool ContainerGroupDef::canMoveNode(const NodeData &_nodeData, int &index, const NodeData &moveNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(moveNode)) {
            return container->canMoveNode(_nodeData, index, moveNode);
        } else {
            index -= container->nodeCount(_nodeData);
        }
    }
    // Failed to find a Container with the given name
    return false;
}

// =============================================================================
// (public)
NodeData ContainerGroupDef::moveNode(const NodeData &_nodeData, int &index, const NodeData &moveNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(moveNode)) {
            return container->moveNode(_nodeData, index, moveNode);
        } else {
            index -= container->nodeCount(_nodeData);
        }
    }
    // Failed to find a Container with the given name
    ASSERT(false);
    return NodeData();
}

// =============================================================================
// (public)
bool ContainerGroupDef::canRemoveNode(const NodeData &_nodeData, int index) const
{
    int count;
    for (auto container: m_containerList)
    {
        count = container->nodeCount(_nodeData);
        if (index < count) {
            return container->canRemoveNode(_nodeData, index);
        } else {
            index -= count;
        }
    }
    // index is out of range
    return false;
}

// =============================================================================
// (public)
bool ContainerGroupDef::removeNode(const NodeData &_nodeData, int index) const
{
    int count;
    for (auto container: m_containerList)
    {
        count = container->nodeCount(_nodeData);
        if (index < count) {
            return container->removeNode(_nodeData, index);
        } else {
            index -= count;
        }
    }
    // index is out of range
    return false;
}

// =============================================================================
// (protected)
void ContainerGroupDef::updateContainerList() const
{
    m_containerList = m_hostDef->containerList();
}

} // namespace Oak::Model

