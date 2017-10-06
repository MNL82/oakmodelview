/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include "ContainerDefinition.h"

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class ContainerDefinitionBuilder
{
    ContainerDefinitionBuilder() = delete;
public:
    static ContainerDefinitionUPtr Make(NodeDefinitionSPtr containerDefinition, int minCount = 0, int maxCount = std::numeric_limits<int>::max());

    static bool setNodeDefElement(const ContainerDefinitionUPtr& nContainer, NodeDefinitionSPtr containerDefinition);
    static bool setNodeDefParent(const ContainerDefinitionUPtr& nContainer, NodeDefinitionSPtr hostDefinition);

    static bool setMinMaxCount(const ContainerDefinitionUPtr& nContainer, int minCount = 0, int maxCount = std::numeric_limits<int>::max());

#ifdef XML_BACKEND
    static bool setElementListRef(const ContainerDefinitionUPtr& nContainer, XML::ListRef listRef);
#endif // XML_BACKEND
};

typedef ContainerDefinitionBuilder CDB;

} // namespace Model
} // namespace Oak
