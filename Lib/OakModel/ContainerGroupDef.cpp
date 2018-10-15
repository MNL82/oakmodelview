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

namespace Oak {
namespace Model {

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
const NodeDef* ContainerGroupDef::containerDef(const std::string &name, const UnionRef& derivedId) const
{
    return m_hostDef->container(name).containerDef(derivedId);
}

// =============================================================================
// (public)
const NodeDef* ContainerGroupDef::containerDef(Node _node) const
{
    const ContainerDef& ci = m_hostDef->container(_node);
    if (ci.isNull()) { return nullptr; }
    return ci.containerDef(_node);
}

// =============================================================================
// (public)
bool ContainerGroupDef::validate(Node _node) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(_node)) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// (public)
int ContainerGroupDef::nodeCount(Node _node) const
{
    int count = 0;
    for (auto child: m_containerList) {
        count += child->nodeCount(_node);
    }

    return count;
}

// =============================================================================
// (public)
int ContainerGroupDef::nodeIndex(Node _node, Node refNode) const
{
    int index = 0;
    for (auto container: m_containerList)
    {
        if (container->validate(refNode)) {
            return index + container->nodeIndex(_node, refNode);
        } else {
            // Adds all nodes to the index that belongs to an earlier Container
            index += container->nodeCount(_node);
        }
    }
    return -1;
}

// =============================================================================
// (public)
Node ContainerGroupDef::node(Node _node, int index, const NodeDef** nodeDef) const
{
    for (auto container: m_containerList)
    {
        Node childNode = container->node(_node, index, nodeDef);
        if (childNode.isNull()) {
            index -= container->nodeCount(_node);
        } else {
            return childNode;
        }
    }
    if (nodeDef) { *nodeDef = nullptr; }
    return Node();
}

// =============================================================================
// (public)
Node ContainerGroupDef::firstNode(Node _node, const NodeDef** nodeDef) const
{
    for (auto container: m_containerList)
    {
        Node childNode = container->firstNode(_node, nodeDef);
        if (!childNode.isNull()) {
            return childNode;
        }
    }
    if (nodeDef) { *nodeDef = nullptr; }
    return Node();
}

// =============================================================================
// (public)
Node ContainerGroupDef::lastNode(Node _node, const NodeDef** nodeDef) const
{
    auto it = m_containerList.end();
    while (it != m_containerList.begin()) {
        it--;
        Node childNode = (*it)->lastNode(_node, nodeDef);
        if (!childNode.isNull()) {
            return childNode;
        }
    }
    if (nodeDef) { *nodeDef = nullptr; }
    return Node();
}

// =============================================================================
// (public)
Node ContainerGroupDef::nextNode(Node _node, Node refNode, const NodeDef** nodeDef) const
{
    auto it = m_containerList.begin();
    while (it != m_containerList.end()) {
        if ((*it)->nodeIndex(_node, refNode) != -1) {
            Node childNode = (*it)->nextNode(refNode, nodeDef);
            if (childNode.isNull()) {
                // Faild to find next node in the same Container
                // Looking in the next one ...
                it++;
                while (it != m_containerList.end()) {
                    childNode = (*it)->firstNode(_node, nodeDef);
                    if (!childNode.isNull()) {
                        // Found the next node as the fist node in an other Container
                        return childNode;
                    }
                    it++;
                }
                // Found the refNode but there is no next node
                return Node();
            } else {
                return childNode;
            }
        }
        it++;
    }
    // Never found the refNode
    ASSERT(false);
    if (nodeDef) { *nodeDef = nullptr; }
    return Node();
}

// =============================================================================
// (public)
Node ContainerGroupDef::previousNode(Node _node, Node refNode, const NodeDef** nodeDef) const
{
    auto it = m_containerList.end();
    while (it != m_containerList.begin()) {
        it--;
        if ((*it)->nodeIndex(_node, refNode) != -1) {
            Node childNode = (*it)->nextNode(refNode, nodeDef);
            if (childNode.isNull()) {
                // Faild to find previous node in the same Container
                // Looking in the previous one ...
                while (it != m_containerList.begin()) {
                    it--;
                    childNode = (*it)->lastNode(_node, nodeDef);
                    if (!childNode.isNull()) {
                        // Found the previous node as the last node in an other Container
                        return childNode;
                    }
                }
                // Found the refNode but there is no previous node
                return Node();
            } else {
                return childNode;
            }
        }
    }
    // Never found the refNode
    ASSERT(false);
    if (nodeDef) { *nodeDef = nullptr; }
    return Node();
}

// =============================================================================
// (public)
bool ContainerGroupDef::canInsertNode(Node _node, const UnionRef &name, int &index) const
{
    for (auto container: m_containerList)
    {
        if (name == container->containerDef()->name()) {
            return container->canInsertNode(_node, index);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given name
    ASSERT(false);
    return false;
}

// =============================================================================
// (public)
Node ContainerGroupDef::insertNode(Node _node, const UnionRef &name, int &index) const
{
    for (auto container: m_containerList)
    {
        if (name == container->containerDef()->name()) {
            return container->insertNode(_node, index);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given name
    ASSERT(false);
    return Node();
}

// =============================================================================
// (public)
bool ContainerGroupDef::canCloneNode(Node _node, int &index, Node cloneNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(cloneNode)) {
            return container->canCloneNode(_node, index, cloneNode);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given name
    return false;
}

// =============================================================================
// (public)
Node ContainerGroupDef::cloneNode(Node _node, int &index, Node cloneNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(cloneNode)) {
            return container->cloneNode(_node, index, cloneNode);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given name
    ASSERT(false);
    return Node();
}

// =============================================================================
// (public)
bool ContainerGroupDef::canMoveNode(Node _node, int &index, Node moveNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(moveNode)) {
            return container->canMoveNode(_node, index, moveNode);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given name
    return false;
}

// =============================================================================
// (public)
Node ContainerGroupDef::moveNode(Node _node, int &index, Node moveNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(moveNode)) {
            return container->moveNode(_node, index, moveNode);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given name
    ASSERT(false);
    return Node();
}

// =============================================================================
// (public)
bool ContainerGroupDef::canRemoveNode(Node _node, int index) const
{
    int count;
    for (auto container: m_containerList)
    {
        count = container->nodeCount(_node);
        if (index < count) {
            return container->canRemoveNode(_node, index);
        } else {
            index -= count;
        }
    }
    // index is out of range
    return false;
}

// =============================================================================
// (public)
bool ContainerGroupDef::removeNode(Node _node, int index) const
{
    int count;
    for (auto container: m_containerList)
    {
        count = container->nodeCount(_node);
        if (index < count) {
            return container->removeNode(_node, index);
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

} // namespace Model
} // namespace Oak
