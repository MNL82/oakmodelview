/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeData.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::Model {

// =============================================================================
// (public)
NodeData::NodeData()
{
    m_type = Type::UNDEFINED;
}

// =============================================================================
// (public)
NodeData::NodeData(void *dataPointer, NodeData::Type type)
{
    m_type = type;
    switch (m_type) {
    case Type::UNDEFINED:
        break;
#ifdef XML_BACKEND
    case Type::XML:
        m_xmlNode = XML::Element(pugi::xml_node(static_cast<pugi::xml_node_struct*>(dataPointer)));
        break;
#endif // XML_BACKEND
    default:
        // m_type contains an unhandled type that needs to be implemented
        ASSERT(false);
    }
}

// =============================================================================
// (public)
NodeData::NodeData(const NodeData &copy)
{
    memcpy(this, &copy, sizeof(NodeData));
}

// =============================================================================
// (public)
NodeData::NodeData(NodeData &&move)
{
    memcpy(this, &move, sizeof(NodeData));
    move.m_type = Type::UNDEFINED;
}

// =============================================================================
// (public)
NodeData &NodeData::operator=(const NodeData &copy)
{
    memcpy(this, &copy, sizeof(NodeData));
    return *this;
}

// =============================================================================
// (public)
NodeData &NodeData::operator=(NodeData &&move)
{
    memcpy(this, &move, sizeof(NodeData));
    move.m_type = Type::UNDEFINED;
    return *this;
}

// =============================================================================
// (public)
bool NodeData::operator==(const NodeData &_node) const
{
    if (m_type != _node.m_type) { return false; }

    switch (m_type) {
    case Type::UNDEFINED:
        return false;
#ifdef XML_BACKEND
    case Type::XML:
        return m_xmlNode == _node.m_xmlNode;
#endif // XML_BACKEND
    default:
        // m_type contains an unhandled type that needs to be implemented
        ASSERT(false);
    }

    return false;
}

// =============================================================================
// (public)
bool NodeData::operator!=(const NodeData& _node) const
{
    return !(operator==(_node));
}

// =============================================================================
// (public)
void NodeData::clear()
{
    switch (m_type) {
    case Type::UNDEFINED:
        return;
#ifdef XML_BACKEND
    case Type::XML:
        m_xmlNode = XML::Element();
        break;
#endif // XML_BACKEND
    default:
        // m_type contains an unhandled type that needs to be implemented
        ASSERT(false);
    }
    m_type = Type::UNDEFINED;
}

// =============================================================================
// (public)
bool NodeData::isNull() const
{
    if (m_type == Type::UNDEFINED) { return true; }
    switch (m_type) {
    case Type::UNDEFINED:
        return false;
#ifdef XML_BACKEND
    case Type::XML:
        return m_xmlNode.isNull();
#endif // XML_BACKEND
    }

    // m_type contains an unhandled type that needs to be implemented
    ASSERT(false);
    return true;
}

// =============================================================================
// (public)
NodeData::Type NodeData::type() const
{
    return m_type;
}

// =============================================================================
// (public)
void *NodeData::internalPtr() const
{
    switch (m_type) {
    case Type::UNDEFINED:
        return nullptr;
#ifdef XML_BACKEND
    case Type::XML:
        return m_xmlNode.internalObject().internal_object();
#endif // XML_BACKEND
    }
    return nullptr;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
NodeData::NodeData(XML::Element _xmlNode)
    : m_xmlNode(_xmlNode)
{
    m_type = Type::XML;
}

// =============================================================================
// (public)
bool NodeData::isXML() const
{
    return m_type == Type::XML;
}

// =============================================================================
// (public)
XML::Element NodeData::xmlNode() const
{
    ASSERT(m_type ==Type::XML);
    return m_xmlNode;
}
#endif // XML_BACKEND

} // namespace Oak::Model

