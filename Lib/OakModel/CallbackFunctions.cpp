/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "CallbackFunctions.h"

#include "../ServiceFunctions/Assert.h"

namespace Oak {
namespace Model {

// =============================================================================
// (public)
Callback::Callback()
{

}

// =============================================================================
// (public)
void Callback::remove(void* funcObj)
{
    if (funcObj == nullptr) {
        m_functionMap.clear();
    } else {
        auto it = m_functionMap.find(funcObj);
        while (it != m_functionMap.end()) {
            m_functionMap.erase(it);
            it = m_functionMap.find(funcObj);
        }
    }
}

// =============================================================================
// (public)
void Callback::trigger() const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback::trigger()";
        func.second();
    }
}

// =============================================================================
// (public)
Callback_ItemIntItemInt::Callback_ItemIntItemInt()
{

}

// =============================================================================
// (public)
void Callback_ItemIntItemInt::remove(void *funcObj)
{
    if (funcObj == nullptr) {
        m_functionMap.clear();
    } else {
        auto it = m_functionMap.find(funcObj);
        while (it != m_functionMap.end()) {
            m_functionMap.erase(it);
            it = m_functionMap.find(funcObj);
        }
    }
}

// =============================================================================
// (public)
void Callback_ItemIntItemInt::trigger(const Item &sourceItem, int sourceIndex, const Item &targetItem, int targetIndex) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_ItemIntItemInt::trigger()";
        func.second(sourceItem, sourceIndex, targetItem, targetIndex);
    }
}

// =============================================================================
// (public)
Callback_ItemInt::Callback_ItemInt()
{

}

// =============================================================================
// (public)
void Callback_ItemInt::remove(void *funcObj)
{
    if (funcObj == nullptr) {
        m_functionMap.clear();
    } else {
        auto it = m_functionMap.find(funcObj);
        while (it != m_functionMap.end()) {
            m_functionMap.erase(it);
            it = m_functionMap.find(funcObj);
        }
    }
}

// =============================================================================
// (public)
void Callback_ItemInt::trigger(const Item &parentItem, int index) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_ItemInt::trigger()";
        func.second(parentItem, index);
    }
}

// =============================================================================
// (public)
Callback_Item::Callback_Item()
{

}

// =============================================================================
// (public)
void Callback_Item::remove(void *funcObj)
{
    if (funcObj == nullptr) {
        m_functionMap.clear();
    } else {
        auto it = m_functionMap.find(funcObj);
        while (it != m_functionMap.end()) {
            m_functionMap.erase(it);
            it = m_functionMap.find(funcObj);
        }
    }
}

// =============================================================================
// (public)
void Callback_Item::trigger(const Item &parentItem) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_Item::trigger()";
        func.second(parentItem);
    }
}

// =============================================================================
// (public)
Callback_ItemIndex::Callback_ItemIndex()
{

}

// =============================================================================
// (public)
void Callback_ItemIndex::remove(void *funcObj)
{
    if (funcObj == nullptr) {
        m_functionMap.clear();
    } else {
        auto it = m_functionMap.find(funcObj);
        while (it != m_functionMap.end()) {
            m_functionMap.erase(it);
            it = m_functionMap.find(funcObj);
        }
    }
}

// =============================================================================
// (public)
void Callback_ItemIndex::trigger(const ItemIndex &itemIndex) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_ItemIndex::trigger()";
        func.second(itemIndex);
    }
}

// =============================================================================
// (public)
Callback_ItemIndexItemIndex::Callback_ItemIndexItemIndex()
{

}

// =============================================================================
// (public)
void Callback_ItemIndexItemIndex::remove(void *funcObj)
{
    if (funcObj == nullptr) {
        m_functionMap.clear();
    } else {
        auto it = m_functionMap.find(funcObj);
        while (it != m_functionMap.end()) {
            m_functionMap.erase(it);
            it = m_functionMap.find(funcObj);
        }
    }
}

// =============================================================================
// (public)
void Callback_ItemIndexItemIndex::trigger(const ItemIndex &itemIndex1, const ItemIndex &itemIndex2) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_ItemIndexItemIndex::trigger()";
        func.second(itemIndex1, itemIndex2);
    }
}

// =============================================================================
// (public)
Callback_ItemIndexInt::Callback_ItemIndexInt()
{

}

// =============================================================================
// (public)
void Callback_ItemIndexInt::remove(void *funcObj)
{
    if (funcObj == nullptr) {
        m_functionMap.clear();
    } else {
        auto it = m_functionMap.find(funcObj);
        while (it != m_functionMap.end()) {
            m_functionMap.erase(it);
            it = m_functionMap.find(funcObj);
        }
    }
}

// =============================================================================
// (public)
void Callback_ItemIndexInt::trigger(const ItemIndex &itemIndex, int index) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_ItemIndexInt::trigger()";
        func.second(itemIndex, index);
    }
}

// =============================================================================
// (public)
Callback_ItemIndexString::Callback_ItemIndexString()
{

}

// =============================================================================
// (public)
void Callback_ItemIndexString::remove(void *funcObj)
{
    if (funcObj == nullptr) {
        m_functionMap.clear();
    } else {
        auto it = m_functionMap.find(funcObj);
        while (it != m_functionMap.end()) {
            m_functionMap.erase(it);
            it = m_functionMap.find(funcObj);
        }
    }
}

// =============================================================================
// (public)
void Callback_ItemIndexString::trigger(const ItemIndex &itemIndex, const std::string &name) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_ItemIndexInt::trigger()";
        func.second(itemIndex, name);
    }
}

} // namespace Model
} // namespace Oak
