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

#include "ValueDefBuilderData.h"
#include "ContainerDefBuilderData.h"

#include "NodeDef.h"

namespace Oak::Model {

class NodeDefBuilderData;
typedef std::unique_ptr<NodeDefBuilderData> NodeDefBuilderDataUPtr;

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
    bool createDerived(NodeDefSPtr baseNodeDef) const;

    std::string name;
    std::string displayName;
    Color color;
    std::string imagePath;
    std::string variantId;

    ValueDefBuilderDataUPtr keyValue;
    ValueDefBuilderDataUPtr variantValue;

    std::vector<ValueDefBuilderDataUPtr> values;
    std::vector<ContainerDefBuilderDataUPtr> containers;
    std::vector<NodeDefBuilderDataUPtr> derivedNodeDefs;
};


typedef std::unique_ptr<NodeDefBuilderData> NodeDefBuilderDataUPtr;

} // namespace Oak::Model
