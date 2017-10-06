/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerGroupDefinition.h"

#include "NodeDefinition.h"
#include "ContainerDefinition.h"

#include <assert.h>

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ContainerGroupDefinition::ContainerGroupDefinition(const NodeDefinition *hostDefinition)
{
    assert(hostDefinition);
    m_hostDefinition = hostDefinition;

    updateContainerList();
}

// =============================================================================
// (public)
ContainerGroupDefinition::ContainerGroupDefinition(const ContainerGroupDefinition &copy)
    : m_hostDefinition(copy.m_hostDefinition),
      m_containerList(copy.m_containerList)
{
}

// =============================================================================
// (public)
ContainerGroupDefinition::ContainerGroupDefinition(ContainerGroupDefinition &&move)
    : m_hostDefinition(std::move(move.m_hostDefinition)),
      m_containerList(std::move(move.m_containerList))
{
}

// =============================================================================
// (public)
ContainerGroupDefinition&  ContainerGroupDefinition::operator=(const ContainerGroupDefinition &copy)
{
    m_hostDefinition = copy.m_hostDefinition;
    m_containerList.assign(copy.m_containerList.begin(), copy.m_containerList.end());
    return *this;
}

// =============================================================================
// (public)
ContainerGroupDefinition& ContainerGroupDefinition::operator=(ContainerGroupDefinition &&move)
{
    m_hostDefinition = std::move(move.m_hostDefinition);
    m_containerList = std::move(move.m_containerList);
    return *this;
}

// =============================================================================
// (public)
ContainerGroupDefinitionUPtr ContainerGroupDefinition::copy() const
{
    return MakeUPtr(*this);
}

// =============================================================================
// (public)
const NodeDefinition* ContainerGroupDefinition::containerDefinition(VariantCRef primaryKey, VariantCRef derivedId) const
{
    return m_hostDefinition->container(primaryKey).containerDefinition(derivedId);
}

// =============================================================================
// (public)
const NodeDefinition* ContainerGroupDefinition::containerDefinition(Node _node) const
{
    const ContainerDefinition& ci = m_hostDefinition->container(_node);
    if (ci.isNull()) { return nullptr; }
    return ci.containerDefinition(_node);
}

// =============================================================================
// (public)
bool ContainerGroupDefinition::validate(Node _node) const
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
int ContainerGroupDefinition::nodeCount(Node _node) const
{
    int count = 0;
    for (auto child: m_containerList) {
        count += child->nodeCount(_node);
    }

    return count;
}

// =============================================================================
// (public)
int ContainerGroupDefinition::nodeIndex(Node _node, Node refNode) const
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
Node ContainerGroupDefinition::node(Node _node, int index, const NodeDefinition** nodeDefinition) const
{
    for (auto container: m_containerList)
    {
        Node childNode = container->node(_node, index, nodeDefinition);
        if (childNode.isNull()) {
            index -= container->nodeCount(_node);
        } else {
            return childNode;
        }
    }
    if (nodeDefinition) { *nodeDefinition = 0; }
    return Node();
}

// =============================================================================
// (public)
Node ContainerGroupDefinition::firstNode(Node _node, const NodeDefinition** nodeDefinition) const
{
    for (auto container: m_containerList)
    {
        Node childNode = container->firstNode(_node, nodeDefinition);
        if (!childNode.isNull()) {
            return childNode;
        }
    }
    if (nodeDefinition) { *nodeDefinition = 0; }
    return Node();
}

// =============================================================================
// (public)
Node ContainerGroupDefinition::lastNode(Node _node, const NodeDefinition** nodeDefinition) const
{
    auto it = m_containerList.end();
    while (it != m_containerList.begin()) {
        it--;
        Node childNode = (*it)->lastNode(_node, nodeDefinition);
        if (!childNode.isNull()) {
            return childNode;
        }
    }
    if (nodeDefinition) { *nodeDefinition = 0; }
    return Node();
}

// =============================================================================
// (public)
Node ContainerGroupDefinition::nextNode(Node _node, Node refNode, const NodeDefinition** nodeDefinition) const
{
    auto it = m_containerList.begin();
    while (it != m_containerList.end()) {
        if ((*it)->nodeIndex(_node, refNode) != -1) {
            Node childNode = (*it)->nextNode(refNode, nodeDefinition);
            if (childNode.isNull()) {
                // Faild to find next node in the same Container
                // Looking in the next one ...
                it++;
                while (it != m_containerList.end()) {
                    childNode = (*it)->firstNode(_node, nodeDefinition);
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
    assert(false);
    if (nodeDefinition) { *nodeDefinition = 0; }
    return Node();
}

// =============================================================================
// (public)
Node ContainerGroupDefinition::previousNode(Node _node, Node refNode, const NodeDefinition** nodeDefinition) const
{
    auto it = m_containerList.end();
    while (it != m_containerList.begin()) {
        it--;
        if ((*it)->nodeIndex(_node, refNode) != -1) {
            Node childNode = (*it)->nextNode(refNode, nodeDefinition);
            if (childNode.isNull()) {
                // Faild to find previous node in the same Container
                // Looking in the previous one ...
                while (it != m_containerList.begin()) {
                    it--;
                    childNode = (*it)->lastNode(_node, nodeDefinition);
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
    assert(false);
    if (nodeDefinition) { *nodeDefinition = 0; }
    return Node();
}

// =============================================================================
// (public)
bool ContainerGroupDefinition::canInsertNode(Node _node, VariantCRef primaryKey, int &index) const
{
    for (auto container: m_containerList)
    {
        if (primaryKey == container->containerDefinition()->primaryKey()) {
            return container->canInsertNode(_node, index);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given primaryKey
    assert(false);
    return false;
}

// =============================================================================
// (public)
Node ContainerGroupDefinition::insertNode(Node _node, VariantCRef primaryKey, int &index) const
{
    for (auto container: m_containerList)
    {
        if (primaryKey == container->containerDefinition()->primaryKey()) {
            return container->insertNode(_node, index);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given primaryKey
    assert(false);
    return Node();
}

// =============================================================================
// (public)
bool ContainerGroupDefinition::canCloneNode(Node _node, int &index, Node cloneNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(cloneNode)) {
            return container->canCloneNode(_node, index, cloneNode);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given primaryKey
    return false;
}

// =============================================================================
// (public)
Node ContainerGroupDefinition::cloneNode(Node _node, int &index, Node cloneNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(cloneNode)) {
            return container->cloneNode(_node, index, cloneNode);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given primaryKey
    assert(false);
    return Node();
}

// =============================================================================
// (public)
bool ContainerGroupDefinition::canMoveNode(Node _node, int &index, Node moveNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(moveNode)) {
            return container->canMoveNode(_node, index, moveNode);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given primaryKey
    return false;
}

// =============================================================================
// (public)
Node ContainerGroupDefinition::moveNode(Node _node, int &index, Node moveNode) const
{
    for (auto container: m_containerList)
    {
        if (container->validate(moveNode)) {
            return container->moveNode(_node, index, moveNode);
        } else {
            index -= container->nodeCount(_node);
        }
    }
    // Failed to find a Container with the given primaryKey
    assert(false);
    return Node();
}

// =============================================================================
// (public)
bool ContainerGroupDefinition::canRemoveNode(Node _node, int index) const
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
bool ContainerGroupDefinition::removeNode(Node _node, int index) const
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
void ContainerGroupDefinition::updateContainerList() const
{
    m_containerList.clear();
    m_hostDefinition->getContainerList(m_containerList);
}

} // namespace Model
} // namespace Oak
