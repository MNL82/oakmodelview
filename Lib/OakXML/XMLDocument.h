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

#include <memory>

namespace Oak {
namespace XML {

// =============================================================================
// Class definition
// =============================================================================
class Document
{
public:
    Document();
    Document(const Document &copy);

    Document & operator=(const Document &copy);

    bool operator == (const Document &doc) const;
    bool operator != (const Document &doc) const;
    bool isContentEqual(const Document &doc) const;

    bool isNull() const;

    bool save(std::string filePath) const;
    void save(std::string &str, int indent) const;

    Element appendChild(const std::string &tagName);

    void clear();

    void clone(const Document &copy);

    bool load(std::string filePath);
    bool load(std::istream stream);
    Element documentElement();

private:
    std::shared_ptr<pugi::xml_document> m_document;
};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
