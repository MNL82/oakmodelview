/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */


#include "QueryRef.h"

#include "QueryChildren.h"
#include "QueryParent.h"
#include "QueryIgnore.h"

namespace Oak {
namespace Model {

// =============================================================================
// (protected)
QueryRef::QueryRef()
{

}

// =============================================================================
// (public)
QueryRef::~QueryRef()
{
    if (m_queryPtr) {
        delete m_queryPtr;
        m_queryPtr = nullptr;
    }
}

// =============================================================================
// (public)
QueryRefSPtr QueryRef::setValueName(const std::string &valueName)
{
    m_valueName = valueName;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryRefSPtr QueryRef::children(const std::string nodeName)
{
    add(new QueryChildren(nodeName));
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryRefSPtr QueryRef::parent()
{
    add(new QueryParent());
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
QueryRefSPtr QueryRef::ignore()
{
    add(new QueryIgnore());
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
int QueryRef::count(const Item &item)
{
    if (!m_queryPtr) { return -1; }

    m_queryPtr->reset(item);
    int count = 0;
    while(m_queryPtr->moveNext()) {
        count++;
    }
    return count;
}

// =============================================================================
// (public)
void QueryRef::addValueList(Item item, std::vector<UnionValue> &valueList) const
{
    assert(!m_valueName.empty());

    if (m_queryPtr) {
        m_queryPtr->reset(item);
        while(m_queryPtr->moveNext()) {
            Item tempItem = m_queryPtr->current();
            if (tempItem.hasValue(m_valueName)) {
                valueList.push_back(tempItem.value(m_valueName).value());
            }
        }
    } else {
        if (item.hasValue(m_valueName)) {
            valueList.push_back(item.value(m_valueName).value());
        }
    }
}

// =============================================================================
// (public)
std::vector<UnionValue> QueryRef::getValueList(Item item) const
{
    std::vector<UnionValue> valueList;
    addValueList(item, valueList);
    return valueList;
}

// =============================================================================
// (public)
std::vector<Item> QueryRef::toItemList(Item item)
{
    std::vector<Item> itemList;
    if (!m_queryPtr) { return itemList; }

    m_queryPtr->reset(item);
    while(m_queryPtr->moveNext()) {
        itemList.push_back(m_queryPtr->current());
    }

    return itemList;
}

// =============================================================================
// (public static)
QueryRefSPtr QueryRef::MakeSPtr()
{
    QueryRefSPtr sPtr = QueryRefSPtr(new QueryRef());
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (protected)
void QueryRef::add(Query *query)
{
    if (m_queryPtr) {
        m_queryPtr->add(query);
    } else {
        m_queryPtr = query;
    }
}

} // namespace Model
} // namespace Oak
