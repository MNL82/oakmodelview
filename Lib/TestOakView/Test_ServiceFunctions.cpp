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

NodeDefSPtr createNodeDef()
{
    auto node_oakModel = NDB::Make(nodeId_oakModel);
    auto node_model = NDB::Make(nodeId_model);
    auto node_nodeInterface = NDB::Make(nodeId_nodeInterface);
    auto node_valueInterface = NDB::Make(nodeId_valueInterface);
    auto node_data = NDB::Make(nodeId_data);
    auto node_node = NDB::Make(nodeId_node);

    NDB::setName(node_oakModel, "Tree Data Model");
    NDB::setName(node_model, "Model");
    NDB::setName(node_nodeInterface, "Node Interface");
    NDB::setName(node_valueInterface, "Value Interface");
    NDB::setName(node_data, "Data");
    NDB::setName(node_node, "Node");

    NDB::addContainerDef(node_oakModel, CDB::Make(node_model,1,1));
    NDB::addContainerDef(node_oakModel, CDB::Make(node_data,1,1));
    NDB::addContainerDef(node_model, CDB::Make(node_nodeInterface));
    NDB::addContainerDef(node_nodeInterface, CDB::Make(node_valueInterface));
    NDB::addContainerDef(node_data, CDB::Make(node_node));


    NDB::addValueDefAsKey(node_nodeInterface, VDB::MakeXML(std::string(), "", valueId_name));

    NDB::addValueDefAsKey(node_valueInterface, VDB::MakeXML(std::string(), "", valueId_idName));
    NDB::addValueDefAsDerivedId(node_valueInterface, VDB::MakeXML(std::string(), valueId_type));
    NDB::addValueDef(node_valueInterface, VDB::MakeXML(0, valueId_value));

    return node_oakModel;
}
