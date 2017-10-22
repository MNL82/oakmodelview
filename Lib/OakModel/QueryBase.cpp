/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QueryBase.h"

#include "QueryChildren.h"

namespace Oak {
namespace Model {

// =============================================================================
// (protected)
QueryBase::QueryBase(Item item)
    : m_item(item)
{

}

// =============================================================================
// (public)
QueryBase::~QueryBase()
{
    if (m_queryPtr) {
        delete m_queryPtr;
        m_queryPtr = nullptr;
    }
}

// =============================================================================
// (public)
QueryBaseSPtr QueryBase::children(const std::string nodeName)
{
    add(new QueryChildren(nodeName));
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
int QueryBase::count()
{
    if (!m_queryPtr) { return -1; }

    m_queryPtr->reset(m_item);
    int count = 0;
    while(m_queryPtr->moveNext()) {
        count++;
    }
    return count;
}

// =============================================================================
// (public static)
QueryBaseSPtr QueryBase::MakeSPtr(Item item)
{
    QueryBaseSPtr sPtr = QueryBaseSPtr(new QueryBase(item));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (protected)
void QueryBase::add(Query *query)
{
    if (m_queryPtr) {
        m_queryPtr->add(query);
    } else {
        m_queryPtr = query;
    }
}

} // namespace Model
} // namespace Oak
