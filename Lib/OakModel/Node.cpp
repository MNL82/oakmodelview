/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "Node.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
Node::Node()
{
    m_type = Type::UNDEFINED;
}

// =============================================================================
// (public)
Node::Node(const Node &copy)
{
    memcpy(this, &copy, sizeof(Node));
}

// =============================================================================
// (public)
Node::Node(Node &&move)
{
    memcpy(this, &move, sizeof(Node));
    move.m_type = Type::UNDEFINED;
}

// =============================================================================
// (public)
Node &Node::operator=(const Node &copy)
{
    memcpy(this, &copy, sizeof(Node));
    return *this;
}

// =============================================================================
// (public)
Node &Node::operator=(Node &&move)
{
    memcpy(this, &move, sizeof(Node));
    move.m_type = Type::UNDEFINED;
    return *this;
}

// =============================================================================
// (public)
bool Node::operator==(const Node &_node) const
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
bool Node::operator!=(const Node& _node) const
{
    return !(operator==(_node));
}

// =============================================================================
// (public)
void Node::clear()
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
bool Node::isNull() const
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
Node::Type Node::type() const
{
    return m_type;
}

#ifdef XML_BACKEND
// =============================================================================
// (public)
Node::Node(XML::Element _xmlNode)
    : m_xmlNode(_xmlNode)
{
    m_type = Type::XML;
}

// =============================================================================
// (public)
bool Node::isXML() const
{
    return m_type == Type::XML;
}

// =============================================================================
// (public)
XML::Element &Node::xmlNode()
{
    ASSERT(m_type ==Type::XML);
    return m_xmlNode;
}
#endif // XML_BACKEND

} // namespace Model
} // namespace Oak
