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

#include "NodeDefinition.h"

using namespace Oak::Model;

static std::string nodeId_oakModel("oakModel");
static std::string nodeId_model("model");
static std::string nodeId_data("data");
static std::string nodeId_nodeDef("nodeDefinition");
static std::string nodeId_valueDef("valueDefinition");
static std::string nodeId_node("node");

static std::string valueId_name("name");
static std::string valueId_idName("idName");
static std::string valueId_type("type");
static std::string valueId_value("value");

NodeDefSPtr createNodeDefinition();

