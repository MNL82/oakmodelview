/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "DesignOakModel.h"

#include "NodeDefinitionBuilder.h"
#include "ContainerDefinitionBuilder.h"
#include "ValueDefinitionBuilder.h"

#include "XMLRefFactory.h"

#include <QDebug>

namespace Oak {
namespace Model {

std::string DesignOakModel::s_design        = "design";
std::string DesignOakModel::s_node          = "node";
std::string DesignOakModel::s_value         = "value";
std::string DesignOakModel::s_container     = "container";
std::string DesignOakModel::s_displayName   = "displayName";
std::string DesignOakModel::s_type          = "type";
std::string DesignOakModel::s_minCount      = "minCount";
std::string DesignOakModel::s_maxCount      = "maxCount";
std::string DesignOakModel::s_default       = "default";
std::string DesignOakModel::s_keyValue      = "keyValue";
std::string DesignOakModel::s_tagName       = "tagName";
std::string DesignOakModel::s_attributeName = "attributeName";

// Value types
std::string DesignOakModel::s_string        = "String";
std::string DesignOakModel::s_integer       = "Integer";
std::string DesignOakModel::s_decimal       = "Decimal";

// =============================================================================
// (public)
DesignOakModel::DesignOakModel()
    : OakModel()
{
    createDesignDefinition();
}

// =============================================================================
// (public)
NodeDefinitionSPtr DesignOakModel::generateModelDefinition() const
{
    std::string nodeName;
    Item designItem = rootItem();

    std::map<std::string, NodeDefinitionSPtr> m_nodeMap;

    Item nodeItem = designItem.firstChild(s_node);
    while (!nodeItem.isNull()) {
        auto node = createNodeDefinition(nodeItem);
        if (node) {
            nodeItem.value(s_displayName).getValue(nodeName);
            m_nodeMap[nodeName] = node;
        }
        nodeItem = designItem.nextChild(s_node, nodeItem);
    }

    nodeItem = designItem.firstChild(s_node);
    while (!nodeItem.isNull()) {
        nodeItem.value(s_displayName).getValue(nodeName);
        auto node = m_nodeMap[nodeName];
        if (node) {
            Item nodeContainer = nodeItem.firstChild(s_container);
            while (!nodeContainer.isNull()) {
                nodeContainer.value(s_displayName).getValue(nodeName);
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
NodeDefinitionSPtr DesignOakModel::createNodeDefinition(const Item& item) const
{
    std::string displayName;
    std::string type;
    std::string tagName;
    std::string attributeName;
    displayName << item(s_displayName);
    if (!displayName.empty()) {
        auto nodeDefinition = NDB::Make(displayName);
        std::string name;
        name << item(s_keyValue);
        Item valueItem = item.firstChild(s_value);
        while (!valueItem.isNull()) {
            displayName << valueItem(s_displayName);
            type << valueItem(s_type);
            tagName << valueItem(s_tagName);
            attributeName << valueItem(s_attributeName);
            if (tagName.empty() && attributeName.empty()) {
                tagName = displayName;
            }
            if (!displayName.empty()) {
                auto valueI = VDB::MakeXML(std::string(), tagName, attributeName);
                VDB::setName(valueI, displayName);
                if (displayName == name) {
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
void DesignOakModel::createDesignDefinition()
{
    auto nodeDesign = NDB::Make(s_design);
    auto nodeNode = NDB::Make(s_node);
    auto nodeValue = NDB::Make(s_value);
    auto nodeContainer = NDB::Make(s_container);

    // Setup node "design"
    NDB::addContainerDef(nodeDesign, CDB::Make(nodeNode));

    // Setup node "node"
    NDB::addValueDefAsKey(nodeNode, VDB::MakeXML(std::string(), std::string(), s_displayName));
    NDB::addValueDef(nodeNode, VDB::Make(std::string(), s_keyValue));

    NDB::addContainerDef(nodeNode, CDB::Make(nodeValue));
    NDB::addContainerDef(nodeNode, CDB::Make(nodeContainer));

    // Setup node "Value"
    NDB::addValueDefAsKey(nodeValue, VDB::MakeXML(std::string(), std::string(), s_displayName));
    ValueDefinitionUPtr valueType = VDB::Make(std::string(), s_type);
    std::vector<std::string> options = {s_string, s_integer, s_decimal};
    VDB::setOptions(valueType, options);
    VDB::settings(valueType).setOptionsOnly(true);
    NDB::addValueDef(nodeValue, std::move(valueType));
    NDB::addValueDef(nodeValue, VDB::Make(std::string(), s_tagName));
    NDB::addValueDef(nodeValue, VDB::Make(std::string(), s_attributeName));

    // Setup node "Container"
    NDB::addValueDefAsKey(nodeContainer, VDB::Make(std::string(), s_displayName));
    NDB::addValueDef(nodeContainer, VDB::Make(0, s_minCount, 0));
    NDB::addValueDef(nodeContainer, VDB::Make(0, s_maxCount, std::numeric_limits<int>::max()));

    setRootNodeDefinition(nodeDesign);
}

} // namespace Model
} // namespace Oak
