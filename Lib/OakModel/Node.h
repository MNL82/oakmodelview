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

#ifdef XML_BACKEND
#include "XMLElement.h"
#endif // XML_BACKEND


namespace Oak::Model {

// =============================================================================
// Class definition
// =============================================================================
class Node
{
public:
    enum class Type { UNDEFINED, XML };

    Node();
    Node(const Node& copy);
    Node(Node&& move);

    ~Node() {}

    Node& operator=(const Node& copy);
    Node& operator=(Node&& move);

    bool operator==(const Node& _node) const;
    bool operator!=(const Node& _node) const;

    void clear();

    bool isNull() const;

    Type type() const;

#ifdef XML_BACKEND
    Node(XML::Element _xmlNode);
    bool isXML() const;
    XML::Element xmlNode() const;
#endif // XML_BACKEND

private:
    Type m_type;

    //union {
    #ifdef XML_BACKEND
        XML::Element m_xmlNode;
    #endif // XML_BACKEND
    //};
};

} // namespace Oak::Model

