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

#include <assert.h>
//#include <QDebug>

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

} // namespace Model
} // namespace Oak
