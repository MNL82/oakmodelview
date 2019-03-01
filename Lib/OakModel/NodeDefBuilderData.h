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

#include "Color.h"

#include <string>
#include <memory>
#include <vector>

#include "LeafDefBuilderData.h"
#include "ContainerDefBuilderData.h"

#include "NodeDef.h"

namespace Oak::Model {

class NodeDefBuilderData;
typedef std::unique_ptr<NodeDefBuilderData> NodeDefBuilderDataUPtr;

class NodeDefBuilder;
typedef std::shared_ptr<NodeDefBuilder> NodeDefBuilderSPtr;

// =============================================================================
// Class definition
// =============================================================================
class NodeDefBuilderData
{
public:
    NodeDefBuilderData();
    virtual ~NodeDefBuilderData();

    void validate(std::vector<std::string> &errorMessages) const;
    void validateDerived(std::vector<std::string> &errorMessages) const;
    NodeDefSPtr create();
    bool createVariant(NodeDefBuilderSPtr baseNodeDefBuilder) const;

    bool createContainers(NodeDefSPtr nodeDef, std::vector<NodeDefSPtr> nodeDefList) const;

private:
    void createShared(NodeDefBuilderSPtr builder) const;

public:
    std::string name;
    std::string displayName;
    std::string tooltip;
    Color color;
    std::string imagePath;
    std::string variantId;

#ifdef XML_BACKEND
    std::string tagName;
#endif // XML_BACKEND

    LeafDefBuilderDataUPtr keyLeaf;
    LeafDefBuilderDataUPtr variantLeaf;

    std::vector<LeafDefBuilderDataUPtr> leafs;
    std::vector<ContainerDefBuilderDataUPtr> containers;
    std::vector<NodeDefBuilderDataUPtr> derivedNodeDefs;
};


typedef std::unique_ptr<NodeDefBuilderData> NodeDefBuilderDataUPtr;

} // namespace Oak::Model
