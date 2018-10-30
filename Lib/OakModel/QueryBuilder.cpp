/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QueryBuilder.h"

#include "ItemQueryChildren.h"
#include "ItemQueryParent.h"
#include "ItemQuerySiblings.h"
#include "EntryQuery.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
QueryBuilder::QueryBuilder()
{

}

// =============================================================================
// (public)
ItemQueryUPtr QueryBuilder::UPtr()
{
    return std::move(m_itemQuery);
}

// =============================================================================
// (public)
EntryQuerySPtr QueryBuilder::ValueUPtr(const std::string &entryName)
{
    return EntryQuery::create(std::move(m_itemQuery), entryName);
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::children(const std::string &nodeName)
{
    if (m_itemQuery) {
        m_itemQuery->addChildQuery(ItemQueryUPtr(new ItemQueryChildren(nodeName)));
    } else {
        m_itemQuery = ItemQueryUPtr(new ItemQueryChildren(nodeName));
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::parent()
{
    if (m_itemQuery) {
        m_itemQuery->addChildQuery(ItemQueryUPtr(new ItemQueryParent()));
    } else {
        m_itemQuery = ItemQueryUPtr(new ItemQueryParent());
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::siblings()
{
    if (m_itemQuery) {
        m_itemQuery->addChildQuery(ItemQueryUPtr(new ItemQuerySiblings()));
    } else {
        m_itemQuery = ItemQueryUPtr(new ItemQuerySiblings());
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createChildren(const std::string &nodeName)
{
    QueryBuilderSPtr sPtr = QueryBuilderSPtr(new QueryBuilder());
    sPtr->children(nodeName);
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createParent()
{
    QueryBuilderSPtr sPtr = QueryBuilderSPtr(new QueryBuilder());
    sPtr->parent();
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createSiblings()
{
    QueryBuilderSPtr sPtr = QueryBuilderSPtr(new QueryBuilder());
    sPtr->siblings();
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
EntryQuerySPtr QueryBuilder::createEntry(const std::string &entryName)
{
    return EntryQuery::create(entryName);
}

// =============================================================================
// (public)
ItemQueryUPtr QueryBuilder::duplicate(const ItemQueryUPtr &c)
{
    {
        ItemQueryChildren * itemQuery = dynamic_cast<ItemQueryChildren * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQueryChildren(*itemQuery));
        }
    }

    {
        ItemQueryParent * itemQuery = dynamic_cast<ItemQueryParent * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQueryParent(*itemQuery));
        }
    }

    {
        ItemQuerySiblings * itemQuery = dynamic_cast<ItemQuerySiblings * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQuerySiblings(*itemQuery));
        }
    }

    return ItemQueryUPtr(new ItemQuery(*c.get()));
}

} // namespace Model
} // namespace Oak
