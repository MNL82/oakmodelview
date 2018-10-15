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

#include "XMLDocument.h"

#include "XMLServiceFunctions.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak {
namespace XML {

// =============================================================================
//
Document::Document()
    : m_document(new pugi::xml_document())
{
}

// =============================================================================
//
Document::Document(const Document &copy)
{
    *this = copy;
}

// =============================================================================
//
Document & Document::operator=(const Document &copy)
{
    m_document = copy.m_document;
    return *this;
}

// =============================================================================
//
bool Document::operator==(const Document &doc) const
{
    return m_document.get() == doc.m_document.get();
}

// =============================================================================
//
bool Document::operator !=(const Document &doc) const
{
    return !(operator ==(doc));
}

// =============================================================================
//
bool Document::isContentEqual(const Document &doc) const
{
    // TODO: Implement this
    return m_document == doc.m_document;
}

// =============================================================================
//
bool Document::isNull() const
{
    return m_document->first_child().empty();
}

// =============================================================================
//
Element Document::appendChild(const std::string &tagName)
{
    return Element(m_document->append_child(tagName.c_str()));
}

// =============================================================================
//
void Document::clear()
{
    m_document->reset();
}

// =============================================================================
//
bool Document::save(std::string filePath) const
{
    return m_document->save_file(filePath.c_str(), "  ", pugi::format_indent | pugi::format_save_file_text);
}

// =============================================================================
//
void Document::save(std::string &str, int indent) const
{
    // TODO
    UNUSED(str);
    UNUSED(indent);
}

// =============================================================================
//
void Document::clone(const Document &copy)
{
    m_document->reset(*copy.m_document.get());
}

// =============================================================================
//
bool Document::load(std::string filePath)
{
    return m_document->load_file(filePath.c_str(), pugi::parse_full).status == pugi::status_ok;
}

// =============================================================================
//
bool Document::load(std::istream stream)
{
    return m_document->load(stream).status == pugi::status_ok;
}

// =============================================================================
//
Element Document::documentElement()
{
    return Element(m_document->document_element());
}

} // namespace XML
} // namespace Oak

#endif // XML_BACKEND
