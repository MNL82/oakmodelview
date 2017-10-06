/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "DesignTreeDataModel.h"

#include "NodeDefinitionBuilder.h"
#include "ContainerDefinitionBuilder.h"
#include "ValueDefinitionBuilder.h"

#include "XMLRefFactory.h"

#include <QDebug>

namespace Oak {
namespace Model {

std::string DesignTreeDataModel::s_design        = "design";
std::string DesignTreeDataModel::s_node          = "node";
std::string DesignTreeDataModel::s_value         = "value";
std::string DesignTreeDataModel::s_container     = "container";
std::string DesignTreeDataModel::s_name          = "name";
std::string DesignTreeDataModel::s_type          = "type";
std::string DesignTreeDataModel::s_minCount      = "minCount";
std::string DesignTreeDataModel::s_maxCount      = "maxCount";
std::string DesignTreeDataModel::s_default       = "default";
std::string DesignTreeDataModel::s_keyValue      = "keyValue";
std::string DesignTreeDataModel::s_tagName       = "tagName";
std::string DesignTreeDataModel::s_attributeName = "attributeName";

// Value types
std::string DesignTreeDataModel::s_string        = "String";
std::string DesignTreeDataModel::s_integer       = "Integer";
std::string DesignTreeDataModel::s_decimal       = "Decimal";

// =============================================================================
// (public)
DesignTreeDataModel::DesignTreeDataModel()
    : TreeDataModel()
{
    createDesignDefinition();
}

// =============================================================================
// (public)
NodeDefinitionSPtr DesignTreeDataModel::generateModelDefinition() const
{
    std::string nodeName;
    Item designItem = rootItem();

    std::map<std::string, NodeDefinitionSPtr> m_nodeMap;

    Item nodeItem = designItem.firstChild(s_node);
    while (!nodeItem.isNull()) {
        auto node = createNodeDefinition(nodeItem);
        if (node) {
            nodeItem.value(s_name).getValue(nodeName);
            m_nodeMap[nodeName] = node;
        }
        nodeItem = designItem.nextChild(s_node, nodeItem);
    }

    nodeItem = designItem.firstChild(s_node);
    while (!nodeItem.isNull()) {
        nodeItem.value(s_name).getValue(nodeName);
        auto node = m_nodeMap[nodeName];
        if (node) {
            Item nodeContainer = nodeItem.firstChild(s_container);
            while (!nodeContainer.isNull()) {
                nodeContainer.value(s_name).getValue(nodeName);
                auto containerNode = m_nodeMap[nodeName];
                if (containerNode) {
                    int minCount, maxCount;
                    nodeContainer.value(s_minCount).getValue(minCount);
                    nodeContainer.value(s_maxCount).getValue(maxCount);
                    NDB::addContainerDef(node, CDB::Make(containerNode, minCount, maxCount));
                }
                nodeContainer = nodeItem.nextChild(s_container, nodeContainer);
            }
        }
        nodeItem = designItem.nextChild(s_node, nodeItem);
    }

    auto it = m_nodeMap.begin();
    while (it != m_nodeMap.end()) {
        auto node = it->second;
        if (node && node->parentContainerCount() == 0) {
            return node;
        }
        it++;
    }
    // TODO: Write out a warning that no root node was found
    return NodeDefinitionSPtr();
}

// =============================================================================
// (protected)
NodeDefinitionSPtr DesignTreeDataModel::createNodeDefinition(const Item& item) const
{
    std::string name;
    std::string type;
    std::string tagName;
    std::string attributeName;
    name << item(s_name);
    if (!name.empty()) {
        auto nodeDefinition = NDB::Make(name);
        std::string keyValue;
        keyValue << item(s_keyValue);
        Item valueItem = item.firstChild(s_value);
        while (!valueItem.isNull()) {
            name << valueItem(s_name);
            type << valueItem(s_type);
            tagName << valueItem(s_tagName);
            attributeName << valueItem(s_attributeName);
            if (tagName.empty() && attributeName.empty()) {
                tagName = name;
            }
            if (!name.empty()) {
                auto valueI = VDB::MakeXML(std::string(), tagName, attributeName);
                VDB::setValueId(valueI, name);
                if (name == keyValue) {
                    NDB::addValueDefAsKey(nodeDefinition, std::move(valueI));
                } else {
                    NDB::addValueDef(nodeDefinition, std::move(valueI));
                }
            }
            valueItem = item.nextChild(s_value, valueItem);
        }
        return nodeDefinition;
    }
    return NodeDefinitionSPtr();
}

// =============================================================================
// (protected)
void DesignTreeDataModel::createDesignDefinition()
{
    auto nodeDesign = NDB::Make(s_design);
    auto nodeNode = NDB::Make(s_node);
    auto nodeValue = NDB::Make(s_value);
    auto nodeContainer = NDB::Make(s_container);

    // Setup node "design"
    NDB::addContainerDef(nodeDesign, CDB::Make(nodeNode));

    // Setup node "node"
    NDB::addValueDefAsKey(nodeNode, VDB::MakeXML(std::string(), std::string(), s_name));
    NDB::addValueDef(nodeNode, VDB::Make(std::string(), s_keyValue));

    NDB::addContainerDef(nodeNode, CDB::Make(nodeValue));
    NDB::addContainerDef(nodeNode, CDB::Make(nodeContainer));

    // Setup node "Value"
    NDB::addValueDefAsKey(nodeValue, VDB::MakeXML(std::string(), std::string(), s_name));
    ValueDefinitionUPtr valueType = VDB::Make(std::string(), s_type);
    std::vector<VariantCRef> options = {s_string, s_integer, s_decimal};
    VDB::setOptions(valueType, options);
    VDB::settings(valueType).setOptionsOnly(true);
    NDB::addValueDef(nodeValue, std::move(valueType));
    NDB::addValueDef(nodeValue, VDB::Make(std::string(), s_tagName));
    NDB::addValueDef(nodeValue, VDB::Make(std::string(), s_attributeName));

    // Setup node "Container"
    NDB::addValueDefAsKey(nodeContainer, VDB::Make(std::string(), s_name));
    NDB::addValueDef(nodeContainer, VDB::Make(0, s_minCount, 0));
    NDB::addValueDef(nodeContainer, VDB::Make(0, s_maxCount, std::numeric_limits<int>::max()));

    setRootNodeDefinition(nodeDesign);
}

} // namespace Model
} // namespace Oak
