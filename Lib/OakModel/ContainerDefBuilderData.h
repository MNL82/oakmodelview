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

#include <memory>
#include <string>
#include <vector>

namespace Oak::Model {

class NodeDef;
typedef std::shared_ptr<NodeDef> NodeDefSPtr;

class NodeDefBuilder;
typedef std::shared_ptr<NodeDefBuilder> NodeDefBuilderSPtr;

// =============================================================================
// Class definition
// =============================================================================
class ContainerDefBuilderData
{
public:
    ContainerDefBuilderData();
    virtual ~ContainerDefBuilderData() {}

    void validate(std::vector<std::string> &errorMessages) const;

    bool set(NodeDefBuilderSPtr builder, std::vector<NodeDefSPtr> nodeDefList) const;

    int minCount = 0;
    int maxCount = std::numeric_limits<int>::max();
    std::string name;
    std::string variantId;
};

typedef std::unique_ptr<ContainerDefBuilderData> ContainerDefBuilderDataUPtr;

} // namespace Oak::Model
