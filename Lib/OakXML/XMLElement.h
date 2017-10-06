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

// This class is ment to be an definition to a xml parser
// It is created in the hopes that the TDMLib will be independent of a specific xml parser

#include <map>
#include <string>
typedef std::basic_ostream<char, std::char_traits<char> > stdTextStream;

#include "pugixml/pugixml.hpp"

#ifndef UNUSED
#define UNUSED(x) (void)x;
#endif

namespace Oak {
namespace XML {

// =============================================================================
// Class definition
// =============================================================================
class Element
{
public:
    Element();
    Element(pugi::xml_node element);

    bool operator==(const Element &element) const;
    bool operator!=(const Element &element) const { return !(*this == element); }

    std::string tagName() const;
    int compareTagName(const std::string &tName) const;

    bool isNull() const;
    bool empty() const;

    // Create
    bool cloneElement(const Element &element);
    Element prependChild(const std::string &tagName);
    Element appendChild(const std::string &tagName);
    Element insertBefore(const std::string &tagName, const Element &refChild);
    Element insertAfter(const std::string &tagName, const Element &refChild);
    Element moveBefore(const Element &target, const Element &refChild);
    Element moveAfter(const Element &target, const Element &refChild);
    bool removeChild(const Element &child);
    void clear();
    void save(std::string &str, int indent) const;

    // Element navigation
    Element firstChild(const std::string &tagName = std::string()) const;
    Element lastChild(const std::string &tagName = std::string()) const;

    Element nextSibling(const std::string &tagName = std::string()) const;
    Element previousSibling(const std::string &tagName = std::string()) const;

    Element parentElement() const;

    // Attribute
    bool hasAttribute(const std::string &name) const;
    std::string attribute(const std::string &name) const;
    bool getAttribute(const std::string &name, std::string &value) const;
    std::map<std::string,std::string> attributeMap()const;
    bool setAttribute(const std::string &name, const std::string &value);
    int compareAttribute(const std::string& name, const std::string& value);
    bool removeAttribute(const std::string &name);

    // Element text
    bool hasText() const;
    std::string text() const;
    bool getText(std::string &str) const;
    bool setText(const std::string &text);
    int compareText(const std::string& value);
    bool removeText();

//    Tag names are case-sensitive
//    Tag names must start with a letter or underscore
//    Tag names cannot start with the letters xml (or XML, or Xml, etc)
//    Tag names can contain letters, digits, hyphens(-), underscores(_), and periods(.)
//    Tag names cannot contain spaces
    static bool validateTagName(const std::string& tagName);
    static bool validateAttributeName(const std::string& attributeName);
    static bool validateTagText(const std::string& text);
    static bool validateAttributeText(const std::string& text);

private:
    pugi::xml_node m_element;

//    friend class ParserDocument;
};

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
