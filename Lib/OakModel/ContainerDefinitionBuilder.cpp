/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerDefinitionBuilder.h"

#include "NodeDefinition.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
ContainerDefinitionUPtr ContainerDefinitionBuilder::Make(NodeDefinitionSPtr containerDefinition, int minCount, int maxCount)
{
    assert(containerDefinition);
    assert(minCount >= 0);
    assert(maxCount > 0);
    assert(maxCount >= minCount);

    auto container = ContainerDefinition::MakeUPtr();

    container->m_containerDefinition = containerDefinition;
    container->m_minCount = minCount;
    container->m_maxCount = maxCount;

    containerDefinition->m_parentContainerDefinitions.push_back(container.get());

#ifdef XML_BACKEND
    container->m_elementListRef.setTagName(containerDefinition->tagName());
#endif // XML_BACKEND

    return std::move(container);
}

// =============================================================================
// (public)
bool ContainerDefinitionBuilder::setNodeDefElement(const ContainerDefinitionUPtr &nContainer, NodeDefinitionSPtr containerDefinition)
{
    if (!nContainer) { return false; }
    if (!containerDefinition) { return false; }

#ifdef XML_BACKEND
    // todo: Update m_elementListRef so that it refers to a alement with the same name as the new container definition
#endif // XML_BACKEND
    nContainer->m_containerDefinition = containerDefinition;
    containerDefinition->m_parentContainerDefinitions.push_back(nContainer.get());
    return true;
}

// =============================================================================
// (public)
bool ContainerDefinitionBuilder::setNodeDefParent(const ContainerDefinitionUPtr &nContainer, NodeDefinitionSPtr hostDefinition)
{
    if (!nContainer) { return false; }

    nContainer->m_hostDefinition = hostDefinition;
    return true;
}

// =============================================================================
// (public)
bool ContainerDefinitionBuilder::setMinMaxCount(const ContainerDefinitionUPtr &nContainer, int minCount, int maxCount)
{
    if (!nContainer) { return false; }
    if (minCount < 0 || maxCount < minCount) { return false; }

    nContainer->m_minCount = minCount;
    nContainer->m_maxCount = maxCount;

    return true;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
bool ContainerDefinitionBuilder::setElementListRef(const ContainerDefinitionUPtr &nContainer, XML::ListRef listRef)
{
    if (!nContainer) { return false; }

    // todo: Validate listRef
    // It needs to reference a list of tags with the same tag name as the container node definition
    nContainer->m_elementListRef = listRef;
    return true;
}
#endif // XML_BACKEND

} // namespace Model
} // namespace Oak
