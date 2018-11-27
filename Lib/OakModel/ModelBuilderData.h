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

#include "NodeDefBuilderData.h"

#include "NodeDef.h"

namespace Oak::Model {

// =============================================================================
// Class definition
// =============================================================================
class ModelBuilderData
{
public:
    ModelBuilderData();
    virtual ~ModelBuilderData() {}

    bool validateModel();
    NodeDefSPtr createModel();

    std::vector<NodeDefBuilderDataUPtr> nodeDefs;
    std::vector<std::string> errorMessages;
};

typedef std::unique_ptr<ModelBuilderData> ModelBuilderDataUPtr;

} // namespace Oak::Model
