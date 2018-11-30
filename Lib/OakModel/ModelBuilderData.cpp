/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ModelBuilderData.h"

#include "../ServiceFunctions/Trace.h"

#include "NodeDef.h"

namespace Oak::Model {

// =============================================================================
// (public)
ModelBuilderData::ModelBuilderData()
{
    //TRACE("ModelBuilderData");
}

// =============================================================================
// (public)
bool ModelBuilderData::validateModel()
{
    for (const NodeDefBuilderDataUPtr &ndb: nodeDefs)
    {
        ndb->validate(errorMessages);
    }
    return errorMessages.empty();
}

// =============================================================================
// (public)
NodeDefSPtr ModelBuilderData::createModel()
{
    if (validateModel()) {
        TRACE("OakModel is valid");
    } else {
        TRACE("OakModel is NOT valid");
        for (const std::string &errorMessage: errorMessages)
        {
            TRACE(errorMessage.c_str());
        }

        return NodeDefSPtr();
    }

    std::vector<Oak::Model::NodeDefSPtr> nodeDefList;

    for (const NodeDefBuilderDataUPtr &ndb: nodeDefs) {
        nodeDefList.push_back(ndb->create());
    }

    // All node definitions have to be created before the containers can be
    for (const NodeDefBuilderDataUPtr &ndb: nodeDefs) {
        ndb->createContainers(nodeDefList);
    }

//    for (const auto& nodeDef: nodeDefList) {
//        TRACE("Shared Count(%i): %s", nodeDef.use_count(), nodeDef->name().c_str());
//    }

    return nodeDefList[0];
}

} // namespace Oak::Model
