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
#include "ItemQueryIgnoreSelf.h"
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
EntryQuerySPtr QueryBuilder::ValueUPtr(const std::string &valueName)
{
    return EntryQuery::create(std::move(m_itemQuery), valueName);
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::children(const std::string &nodeName)
{
    if (m_itemQuery) {
        m_itemQuery->add(ItemQueryUPtr(new ItemQueryChildren(nodeName)));
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
        m_itemQuery->add(ItemQueryUPtr(new ItemQueryParent()));
    } else {
        m_itemQuery = ItemQueryUPtr(new ItemQueryParent());
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::siblings(bool matchName)
{
    if (m_itemQuery) {
        m_itemQuery->add(ItemQueryUPtr(new ItemQuerySiblings(matchName)));
    } else {
        m_itemQuery = ItemQueryUPtr(new ItemQuerySiblings(matchName));
    }
    return m_thisWPtr.lock();
}

//// =============================================================================
//// (public)
//QueryBuilderSPtr QueryBuilder::ignoreSelf()
//{
//    if (m_itemQuery) {
//        m_itemQuery->add(ItemQueryUPtr(new ItemQueryIgnoreSelf()));
//    } else {
//        m_itemQuery = ItemQueryUPtr(new ItemQueryIgnoreSelf());
//    }
//    return m_thisWPtr.lock();
//}

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
QueryBuilderSPtr QueryBuilder::createSiblings(bool matchName)
{
    QueryBuilderSPtr sPtr = QueryBuilderSPtr(new QueryBuilder());
    sPtr->siblings(matchName);
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

//// =============================================================================
//// (public)
//QueryBuilderSPtr QueryBuilder::createIgnoreSelf()
//{
//    QueryBuilderSPtr sPtr = QueryBuilderSPtr(new QueryBuilder());
//    sPtr->ignoreSelf();
//    sPtr->m_thisWPtr = sPtr;
//    return sPtr;
//}

// =============================================================================
// (public)
EntryQuerySPtr QueryBuilder::createValue(const std::string &valueName)
{
    return EntryQuery::create(valueName);
}

} // namespace Model
} // namespace Oak
