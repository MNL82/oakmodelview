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


namespace Oak::Model {

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
Callback_NodeIntNodeInt::Callback_NodeIntNodeInt()
{

}

// =============================================================================
// (public)
void Callback_NodeIntNodeInt::remove(void *funcObj)
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
void Callback_NodeIntNodeInt::trigger(const Node &sourceNode, int sourceIndex, const Node &targetNode, int targetIndex) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_NodeIntNodeInt::trigger()";
        func.second(sourceNode, sourceIndex, targetNode, targetIndex);
    }
}

// =============================================================================
// (public)
Callback_NodeInt::Callback_NodeInt()
{

}

// =============================================================================
// (public)
void Callback_NodeInt::remove(void *funcObj)
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
void Callback_NodeInt::trigger(const Node &parentNode, int index) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_NodeInt::trigger()";
        func.second(parentNode, index);
    }
}

// =============================================================================
// (public)
Callback_Node::Callback_Node()
{

}

// =============================================================================
// (public)
void Callback_Node::remove(void *funcObj)
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
void Callback_Node::trigger(const Node &parentNode) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_Node::trigger()";
        func.second(parentNode);
    }
}

// =============================================================================
// (public)
Callback_NodeIndex::Callback_NodeIndex()
{

}

// =============================================================================
// (public)
void Callback_NodeIndex::remove(void *funcObj)
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
void Callback_NodeIndex::trigger(const NodeIndex &nodeIndex) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_NodeIndex::trigger()";
        func.second(nodeIndex);
    }
}

// =============================================================================
// (public)
Callback_NodeIndexNodeIndex::Callback_NodeIndexNodeIndex()
{

}

// =============================================================================
// (public)
void Callback_NodeIndexNodeIndex::remove(void *funcObj)
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
void Callback_NodeIndexNodeIndex::trigger(const NodeIndex &nodeIndex1, const NodeIndex &nodeIndex2) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_NodeIndexNodeIndex::trigger()";
        func.second(nodeIndex1, nodeIndex2);
    }
}

// =============================================================================
// (public)
Callback_NodeIndexInt::Callback_NodeIndexInt()
{

}

// =============================================================================
// (public)
void Callback_NodeIndexInt::remove(void *funcObj)
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
void Callback_NodeIndexInt::trigger(const NodeIndex &nodeIndex, int index) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_NodeIndexInt::trigger()";
        func.second(nodeIndex, index);
    }
}

// =============================================================================
// (public)
Callback_NodeIndexString::Callback_NodeIndexString()
{

}

// =============================================================================
// (public)
void Callback_NodeIndexString::remove(void *funcObj)
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
void Callback_NodeIndexString::trigger(const NodeIndex &nodeIndex, const std::string &name) const
{
    for (auto func: m_functionMap)
    {
        //qDebug() << "Callback_NodeIndexInt::trigger()";
        func.second(nodeIndex, name);
    }
}

} // namespace Oak::Model

