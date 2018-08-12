/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#ifdef XML_BACKEND

#include "XMLElement.h"

#include "XMLServiceFunctions.h"

#include <regex>
#include <assert.h>

namespace Oak {
namespace XML {

// =============================================================================
//
Element::Element()
    : m_element()
{
}

// =============================================================================
//
Element::Element(pugi::xml_node element)
{
    m_element = element;
}

// =============================================================================
//
bool Element::operator==(const Element &element) const
{
    return m_element == element.m_element;
}

// =============================================================================
//
std::string Element::tagName() const
{
    return m_element.name();
}

// =============================================================================
//
int Element::compareTagName(const std::string &tName) const
{
    return tName.compare(m_element.name());
}

// =============================================================================
//
bool Element::isNull() const
{
    return m_element.empty() || *m_element.name() == 0;
}

// =============================================================================
//
bool Element::empty() const
{
    return m_element.empty() || (m_element.internal_object()->first_child == nullptr && m_element.internal_object()->first_attribute == nullptr);
}

// =============================================================================
// Replaces this element with a copy of the argument element
bool Element::cloneElement(const Element &element)
{
    pugi::impl::recursive_copy_skip(m_element, element.m_element, m_element);
    return !m_element.empty();
}

// =============================================================================
//
Element Element::prependChild(const std::string &tagName)
{
    return Element(m_element.prepend_child(tagName.c_str()));
}

// =============================================================================
//
Element Element::appendChild(const std::string &tagName)
{
    return Element(m_element.append_child(tagName.c_str()));
}

// =============================================================================
//
Element Element::insertBefore(const std::string &tagName, const Element &refChild)
{
    if (refChild.isNull()) {
        return Element(m_element.prepend_child(tagName.c_str()));
    } else {
        return Element(m_element.insert_child_before(tagName.c_str(), refChild.m_element));
    }
}

// =============================================================================
//
Element Element::insertAfter(const std::string &tagName, const Element &refChild)
{
    if (refChild.isNull()) {
        return Element(m_element.append_child(tagName.c_str()));
    } else {
        return Element(m_element.insert_child_after(tagName.c_str(), refChild.m_element));
    }
}

// =============================================================================
//
Element Element::moveBefore(const Element &target, const Element &refChild)
{
    if (!refChild.isNull() && refChild.previousSibling() == target) {
        // Target is already before refChild
        return target;
    }
    Element element = insertBefore(target.tagName(), refChild);
    element.cloneElement(target);
    target.parentElement().removeChild(target);
    return element;
}

// =============================================================================
//
Element Element::moveAfter(const Element &target, const Element &refChild)
{
    if (!refChild.isNull() && refChild.nextSibling() == target) {
        // Target is already before refChild
        return target;
    }
    Element element = insertAfter(target.tagName(), refChild);
    element.cloneElement(target);
    target.parentElement().removeChild(target);
    return element;
}

// =============================================================================
//
bool Element::removeChild(const Element &child)
{
    return m_element.remove_child(child.m_element);
}

// =============================================================================
//
void Element::clear()
{
    m_element.parent().remove_child(m_element);
}

// =============================================================================
//
void Element::save(std::string &str, int indent) const
{
    UNUSED(str);
    UNUSED(indent);
    //TODO
}

// Element navigation
// =============================================================================
//
Element Element::firstChild(const std::string &tagName) const
{
    pugi::xml_node node = m_element.first_child();
    while (!node.empty()) {
        if (node.type() == pugi::node_element) {
            if (tagName.empty() || tagName.compare(node.name()) == 0) {
                return Element(node);
            }
        }
        node = node.next_sibling();
    }
    return Element();
}

// =============================================================================
//
Element Element::lastChild(const std::string &tagName) const
{
    pugi::xml_node node = m_element.last_child();
    while (!node.empty()) {
        if (node.type() == pugi::node_element) {
            if (tagName.empty() || tagName.compare(node.name()) == 0) {
                return Element(node);
            }
        }
        node = node.previous_sibling();
    }
    return Element();
}

// =============================================================================
//
Element Element::nextSibling(const std::string &tagName) const
{
    pugi::xml_node node = m_element.next_sibling();
    while (!node.empty()) {
        if (node.type() == pugi::node_element) {
            if (tagName.empty() || tagName.compare(node.name()) == 0) {
                return Element(node);
            }
        }
        node = node.next_sibling();
    }
    return Element();
}

// =============================================================================
//
Element Element::previousSibling(const std::string &tagName) const
{
    pugi::xml_node node = m_element.previous_sibling();
    while (!node.empty()) {
        if (node.type() == pugi::node_element) {
            if (tagName.empty() || tagName.compare(node.name()) == 0) {
                return Element(node);
            }
        }
        node = node.previous_sibling();
    }
    return Element();
}

// =============================================================================
//
Element Element::parentElement() const
{
    return Element(m_element.parent());
}

// Attribute
// =============================================================================
//
bool Element::hasAttribute(const std::string &name) const
{
    auto it = m_element.attributes_begin();
    auto itEnd = m_element.attributes_end();
    while (it != itEnd) {
        if (name.compare(it->name()) == 0) {
            return true;
        }
        it++;
    }
    return false;
}

// =============================================================================
//
std::string Element::attribute(const std::string &name) const
{
    auto it = m_element.attributes_begin();
    auto itEnd = m_element.attributes_end();
    while (it != itEnd) {
        if (name.compare(it->name()) == 0) {
            return std::string(it->value());
        }
        it++;
    }
    return std::string();
}

// =============================================================================
//
bool Element::getAttribute(const std::string &name, std::string &value) const
{
    auto it = m_element.attributes_begin();
    auto itEnd = m_element.attributes_end();
    while (it != itEnd) {
        if (name.compare(it->name()) == 0) {
            value = it->value();
            return true;
        }
        it++;
    }
    value.clear();
    return false;
}

// =============================================================================
//
std::map<std::string,std::string> Element::attributeMap()const
{
    std::map<std::string,std::string> attMap;
    auto it = m_element.attributes_begin();
    auto itEnd = m_element.attributes_end();
    while (it != itEnd) {
        attMap[std::string(it->name())] = std::string(it->value());
        it++;
    }
    return attMap;
}

// =============================================================================
//
bool Element::setAttribute(const std::string &name, const std::string &value)
{
    auto it = m_element.attributes_begin();
    auto itEnd = m_element.attributes_end();
    while (it != itEnd) {
        if (name.compare(it->name()) == 0) {
            if (value.compare(it->value()) == 0) {
                // Strings are equal
                return false;
            }
            it->set_value(value.c_str());
            return true;
        }
        it++;
    }
    m_element.append_attribute(name.c_str()).set_value(value.c_str());
    return true;
}

// =============================================================================
//
int Element::compareAttribute(const std::string &name, const std::string &value)
{
    auto it = m_element.attributes_begin();
    auto itEnd = m_element.attributes_end();
    while (it != itEnd) {
        if (name.compare(it->name()) == 0) {
            return value.compare(it->value());
        }
        it++;
    }
    return -2;
}

// =============================================================================
//
bool Element::removeAttribute(const std::string &name)
{
    return m_element.remove_attribute(name.c_str());
}

// Element text
// =============================================================================
//
bool Element::hasText() const
{
    return !m_element.text().empty();
}

// =============================================================================
//
std::string Element::text() const
{
    return std::string(m_element.text().get());
}

// =============================================================================
//
bool Element::getText(std::string &str) const
{
    str = m_element.text().get();
    return true;
}

// =============================================================================
//
bool Element::setText(const std::string &text)
{
    if (text.empty()) {
        if (m_element.text().empty()) {
            // Both strings is empty
            return false;
        } else {
            // Remove existing string
            m_element.remove_child(m_element.text().data());
            return true;
        }
    } else {
        if (text.compare(m_element.text().get()) == 0) {
            // Strings are equal
            return false;
        } else {
            // Set the new text string
            m_element.text().set(text.c_str());
            return true;
        }
    }
}

// =============================================================================
//
int Element::compareText(const std::string &value)
{
    return value.compare(m_element.text().get());
}

// =============================================================================
//
bool Element::removeText()
{
    if (m_element.text().empty()) {
        // There is no text to remove
        return false;
    } else {
        // Remove existing text string
        m_element.remove_child(m_element.text().data());
        return true;
    }
}

//    Tag names are case-sensitive
//    Tag names must start with a letter or underscore
//    Tag names cannot start with the letters xml (or XML, or Xml, etc)
//    Tag names can contain letters, digits, hyphens(-), underscores(_), and periods(.)
//    Tag names cannot contain spaces

//function isXMLTagName ( tag ) // returns true if meets cond. 1-5 above
//{
//    var t = !/^[xX][mM][lL].*/.test(tag); // condition 3
//    t = t && /^[a-zA-Z_].*/.test(tag);  // condition 2
//    t = t && /^[a-zA-Z0-9_\-\.]+$/.test(tag); // condition 4
//    return t;
//}
bool Element::validateTagName(const std::string &tagName)
{
    static std::regex reserved_text("^[xX][mM][lL].*");
    static std::regex start_with("^[a-zA-Z_].*");
    static std::regex valid_chars("^[a-zA-Z0-9_\\-\\.:]+$");


    return !std::regex_match(tagName, reserved_text) &&
            std::regex_match(tagName, start_with) &&
            std::regex_match(tagName, valid_chars);
}

bool Element::validateAttributeName(const std::string &attributeName)
{
    static std::regex start_with("^[a-zA-Z_].*");
    static std::regex valid_chars("^[a-zA-Z0-9_\\-\\.:]+$");

    return std::regex_match(attributeName, start_with) &&
           std::regex_match(attributeName, valid_chars);
}

bool Element::validateTagText(const std::string &text)
{
    UNUSED(text);
    return true;
}

bool Element::validateAttributeText(const std::string &text)
{
    UNUSED(text);
    return true;
}

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
