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
class NodeData
{
public:
    enum class Type { UNDEFINED, XML };

    NodeData();
    NodeData(void * internalPtr, Type type);
    NodeData(const NodeData& copy);
    NodeData(NodeData&& move);

    ~NodeData() {}

    NodeData& operator=(const NodeData& copy);
    NodeData& operator=(NodeData&& move);

    bool operator==(const NodeData& _node) const;
    bool operator!=(const NodeData& _node) const;

    void clear();

    bool isNull() const;

    Type type() const;

    void * internalPtr() const;

#ifdef XML_BACKEND
    NodeData(XML::Element _xmlNode);
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

