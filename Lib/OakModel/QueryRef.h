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

#include "Query.h"

namespace Oak {
namespace Model {

class QueryRef;
typedef std::shared_ptr<QueryRef> QueryRefSPtr;
typedef std::weak_ptr<QueryRef> QueryRefWPtr;

// =============================================================================
// Class definition
// =============================================================================
class QueryRef
{
protected:
    QueryRef();

public:
    ~QueryRef();

    QueryRefSPtr setValueName(const std::string &valueName);

    QueryRefSPtr children(const std::string nodeName);
    QueryRefSPtr parent();
    QueryRefSPtr ignore();

    int count(const Item &item);

    void addValueList(Item item, std::vector<UnionValue> &valueList) const;
    std::vector<UnionValue> getValueList(Item item) const;

    template<typename T>
    std::vector<T> toValueList(Item item);

    std::vector<Item> toItemList(Item item);

    static QueryRefSPtr MakeSPtr();

protected:
    void add(Query * query);

protected:
    std::string m_valueName;
    Query *m_queryPtr = nullptr;

    QueryRefWPtr m_thisWPtr;
};

// =============================================================================
// (public)
template<typename T>
std::vector<T> QueryRef::toValueList(Item item)
{
    assert(!m_valueName.empty());

    std::vector<T> valueList;
    if (m_queryPtr) {
        m_queryPtr->reset(item);
        while(m_queryPtr->moveNext()) {
            Item tempItem = m_queryPtr->current();
            if (tempItem.hasValue(m_valueName)) {
                valueList.push_back(tempItem.value(m_valueName).value<T>());
            }
        }
    } else {
        if (item.hasValue(m_valueName)) {
            valueList.push_back(item.value(m_valueName).value<T>());
        }
    }

    return valueList;
}

} // namespace Model
} // namespace Oak
