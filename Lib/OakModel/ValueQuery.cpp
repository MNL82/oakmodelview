/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueQuery.h"

#include "ItemQueryChildren.h"
#include "ItemQueryParent.h"
#include "ItemQueryIgnoreSelf.h"

namespace Oak {
namespace Model {

// =============================================================================
// (protected)
ValueQuery::ValueQuery(const std::string &valueName)
{
    m_valueName = valueName;
}

// =============================================================================
// (public)
ValueQuery::ValueQuery(ItemQueryUPtr itemQueryUPtr, const std::string &valueName)
{
    m_itemQueryPtr = std::move(itemQueryUPtr);
    m_valueName = valueName;
}

// =============================================================================
// (public)
ValueQuery::~ValueQuery()
{
}

// =============================================================================
// (public)
ValueQuerySPtr ValueQuery::setValueName(const std::string &valueName)
{
    m_valueName = valueName;
    return m_thisWPtr.lock();
}

// =============================================================================
// (public)
int ValueQuery::count(const Item &item)
{
    if (!m_itemQueryPtr) { return -1; }

    m_itemQueryPtr->reset(item);
    int count = 0;
    while(m_itemQueryPtr->moveNext()) {
        count++;
    }
    return count;
}

// =============================================================================
// (public)
void ValueQuery::addValueList(const Item &item, std::vector<UnionValue> &valueList) const
{
    assert(!m_valueName.empty());

    if (m_itemQueryPtr) {
        m_itemQueryPtr->reset(item);
        while(m_itemQueryPtr->moveNext()) {
            Item tempItem = m_itemQueryPtr->current();
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
std::vector<UnionValue> ValueQuery::getValueList(const Item &item) const
{
    std::vector<UnionValue> valueList;
    addValueList(item, valueList);
    return valueList;
}

// =============================================================================
// (public)
std::vector<Item> ValueQuery::toItemList(const Item &item)
{
    std::vector<Item> itemList;
    if (!m_itemQueryPtr) { return itemList; }

    m_itemQueryPtr->reset(item);
    while(m_itemQueryPtr->moveNext()) {
        itemList.push_back(m_itemQueryPtr->current());
    }

    return itemList;
}

// =============================================================================
// (public static)
ValueQuerySPtr ValueQuery::create(const std::string &valueName)
{
    ValueQuerySPtr sPtr = ValueQuerySPtr(new ValueQuery(valueName));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

// =============================================================================
// (public static)
ValueQuerySPtr ValueQuery::create(ItemQueryUPtr itemQueryUPtr, const std::string &valueName)
{
    ValueQuerySPtr sPtr = ValueQuerySPtr(new ValueQuery(std::move(itemQueryUPtr), valueName));
    sPtr->m_thisWPtr = sPtr;
    return sPtr;
}

} // namespace Model
} // namespace Oak
