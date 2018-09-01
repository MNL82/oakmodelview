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

#include "ItemQuery.h"

namespace Oak {
namespace Model {

class EntryQuery;
typedef std::shared_ptr<EntryQuery> EntryQuerySPtr;
typedef std::weak_ptr<EntryQuery> EntryQueryWPtr;

// =============================================================================
// Class definition
// =============================================================================
class EntryQuery
{
protected:
    EntryQuery(const std::string &valueName = "");
    EntryQuery(ItemQueryUPtr itemQueryUPtr, const std::string &entryName = "");

public:
    ~EntryQuery();

    EntryQuerySPtr setValueName(const std::string &entryName);

    int count(const Item &item);

    void addValueList(const Item &item, std::vector<UnionValue> &valueList) const;
    std::vector<UnionValue> getValueList(const Item &item) const;

    void getValue(const Item &item, int index, UnionValue value) const;

    const Entry &entry(const Item &item, int index) const;

    template<typename T>
    T value(const Item &item, int index) const;

    template<typename T>
    std::vector<T> toValueList(const Item &item);

    std::vector<Item> toItemList(const Item &item);

    static EntryQuerySPtr create(const std::string &valueName = "");
    static EntryQuerySPtr create(ItemQueryUPtr itemQueryUPtr, const std::string &valueName = "");

protected:
    std::string m_entryName;
    ItemQueryUPtr m_itemQueryPtr = ItemQueryUPtr();

    EntryQueryWPtr m_thisWPtr;
};

// =============================================================================
// (public)
template<typename T>
T EntryQuery::value(const Item &item, int index) const
{
    assert(!m_entryName.empty());

    if (m_itemQueryPtr) {
        m_itemQueryPtr->reset(item);
        int i = 0;
        while(m_itemQueryPtr->moveNext()) {
            if (i == index) {
                Item tempItem = m_itemQueryPtr->current();
                if (tempItem.hasEntry(m_entryName)) {
                    return item.entry(m_entryName).value<T>();
                }
                return T();
            }
            i++;
        }
        assert(false);
    } else {
        assert(index == 0);
        if (item.hasEntry(m_entryName)) {
            return item.entry(m_entryName).value<T>();
        }
    }
    return T();
}

// =============================================================================
// (public)
template<typename T>
std::vector<T> EntryQuery::toValueList(const Item &item)
{
    assert(!m_entryName.empty());

    std::vector<T> valueList;
    if (m_itemQueryPtr) {
        m_itemQueryPtr->reset(item);
        while(m_itemQueryPtr->moveNext()) {
            Item tempItem = m_itemQueryPtr->current();
            if (tempItem.hasEntry(m_entryName)) {
                valueList.push_back(tempItem.entry(m_entryName).value<T>());
            }
        }
    } else {
        if (item.hasEntry(m_entryName)) {
            valueList.push_back(item.entry(m_entryName).value<T>());
        }
    }

    return valueList;
}

} // namespace Model
} // namespace Oak
