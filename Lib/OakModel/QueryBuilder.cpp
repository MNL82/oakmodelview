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

#include "../ServiceFunctions/Trace.h"


namespace Oak::Model {

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
EntryQuerySPtr QueryBuilder::EntryUPtr(const std::string &entryName)
{
    return EntryQuery::create(std::move(m_itemQuery), entryName);
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::children(const std::string &nodeName, bool invertOrder)
{
    if (m_itemQuery) {
        if (invertOrder) {
            ItemQueryUPtr tempQuery = std::make_unique<ItemQueryChildren>(nodeName);
            tempQuery->addChildQuery(std::move(m_itemQuery));
            m_itemQuery = std::move(tempQuery);
        } else {
            m_itemQuery->addChildQuery(std::make_unique<ItemQueryChildren>(nodeName));
        }
    } else {
        m_itemQuery = std::make_unique<ItemQueryChildren>(nodeName);
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::parent(bool invertOrder)
{
    if (m_itemQuery) {
        if (invertOrder) {
            ItemQueryUPtr tempQuery = std::make_unique<ItemQueryParent>();
            tempQuery->addChildQuery(std::move(m_itemQuery));
            m_itemQuery = std::move(tempQuery);
        } else {
            m_itemQuery->addChildQuery(std::make_unique<ItemQueryParent>());
        }
    } else {
        m_itemQuery = std::make_unique<ItemQueryParent>();
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::siblings(bool invertOrder)
{
    if (m_itemQuery) {
        if (invertOrder) {
            ItemQueryUPtr tempQuery = std::make_unique<ItemQuerySiblings>();
            tempQuery->addChildQuery(std::move(m_itemQuery));
            m_itemQuery = std::move(tempQuery);
        } else {
            m_itemQuery->addChildQuery(std::make_unique<ItemQuerySiblings>());
        }
    } else {
        m_itemQuery = std::make_unique<ItemQuerySiblings>();
    }
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createChildren(const std::string &nodeName)
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->children(nodeName);
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createParent()
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->parent();
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createSiblings()
{
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->siblings();
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public)
QueryBuilderSPtr QueryBuilder::createInverse(const ItemQuery &query, const NodeDef *sourceNodeDef)
{
    ASSERT(sourceNodeDef);
    QueryBuilderSPtr sPtr = std::make_shared<QueryBuilder>();
    sPtr->m_thisWPtr = sPtr;

    const ItemQuery * q = &query;
    const NodeDef *nodeDef = sourceNodeDef;

    while (q != nullptr) {
        const ItemQueryChildren * itemQueryChildren = dynamic_cast<const ItemQueryChildren *>(q);
        if (itemQueryChildren) {
            sPtr->parent(true);
        }
        const ItemQueryParent * itemQueryParent = dynamic_cast<const ItemQueryParent *>(q);
        if (itemQueryParent) {
            sPtr->children(nodeDef->name(), true);
        }
        const ItemQuerySiblings * itemQuerySiblings = dynamic_cast<const ItemQuerySiblings *>(q);
        if (itemQuerySiblings) {
            sPtr->siblings(true);
        }

        nodeDef = q->_nodeDef(nodeDef);
        q = q->childQuery();
    }

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
        const ItemQueryChildren * itemQuery = dynamic_cast<const ItemQueryChildren * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQueryChildren(*itemQuery));
        }
    }

    {
        const ItemQueryParent * itemQuery = dynamic_cast<const ItemQueryParent * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQueryParent(*itemQuery));
        }
    }

    {
        const ItemQuerySiblings * itemQuery = dynamic_cast<const ItemQuerySiblings * >(c.get());
        if (itemQuery) {
            return ItemQueryUPtr(new ItemQuerySiblings(*itemQuery));
        }
    }

    return ItemQueryUPtr(new ItemQuery(*c.get()));
}

} // namespace Oak::Model

