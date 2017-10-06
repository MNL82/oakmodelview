/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Test_ServiceFunctions.h"

#include "NodeDefinitionBuilder.h"
#include "ContainerDefinitionBuilder.h"
#include "ValueDefinitionBuilder.h"

NodeDefSPtr createNodeDefinition()
{
    auto node_oakModel = NDB::Make(nodeId_oakModel);
    auto node_model = NDB::Make(nodeId_model);
    auto node_nodeDef = NDB::Make(nodeId_nodeDef);
    auto node_valueDef = NDB::Make(nodeId_valueDef);
    auto node_data = NDB::Make(nodeId_data);
    auto node_node = NDB::Make(nodeId_node);

    NDB::addContainerDef(node_oakModel, CDB::Make(node_model,1,1));
    NDB::addContainerDef(node_oakModel, CDB::Make(node_data,1,1));
    NDB::addContainerDef(node_model, CDB::Make(node_nodeDef));
    NDB::addContainerDef(node_nodeDef, CDB::Make(node_valueDef));

    NDB::addContainerDef(node_data, CDB::Make(node_node));


    NDB::addValueDefAsKey(node_nodeDef, VDB::MakeXML(std::string(), "", valueId_name));

    NDB::addValueDefAsKey(node_valueDef, VDB::MakeXML(std::string(), "", valueId_idName));
    NDB::addValueDefAsDerivedId(node_valueDef, VDB::MakeXML(std::string(), valueId_type));
    NDB::addValueDef(node_valueDef, VDB::MakeXML(0, valueId_value));

    return node_oakModel;
}
