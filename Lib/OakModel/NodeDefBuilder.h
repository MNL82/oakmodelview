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

#include "NodeDef.h"
#include "ValueDefBuilder.h"

namespace Oak {
namespace Model {

class NodeDefBuilder;
typedef std::shared_ptr<NodeDefBuilder> NodeDefBuilderSPtr;
typedef std::weak_ptr<NodeDefBuilder> NodeDefBuilderWPtr;

// =============================================================================
// Class definition
// =============================================================================
class NodeDefBuilder
{
protected:
    NodeDefBuilder(const std::string &name);
    NodeDefBuilder(const std::string &name, const UnionRef &derivedId);
    NodeDefBuilder(NodeDefSPtr derivedBaseNode, const UnionRef &derivedId);
    NodeDefBuilder(NodeDefSPtr nodeDef);

public:
    static NodeDefBuilderSPtr create(const std::string &name);
    static NodeDefBuilderSPtr createRoot(const std::string &name, const UnionRef &derivedId);
    static NodeDefBuilderSPtr createDerived(NodeDefBuilderSPtr derivedBaseNode, const UnionRef &derivedId);
    static NodeDefBuilderSPtr use(NodeDefSPtr nodeDef);

    NodeDefSPtr get();

    NodeDefBuilderSPtr addValueDef(ValueDefBuilderSPtr valueDef);
    NodeDefBuilderSPtr addValueDefAsKey(ValueDefBuilderSPtr valueDefKey);
    NodeDefBuilderSPtr addValueDefAsDerivedId(ValueDefBuilderSPtr valueDefDerivedId);

    NodeDefBuilderSPtr addContainerDef(ContainerDefUPtr cDef);

    NodeDefBuilderSPtr setDisplayName(const std::string& displayName);

#ifdef XML_BACKEND
    NodeDefBuilderSPtr setTagName(const std::string& tagName);
#endif // XML_BACKEND

//protected:
//    // ************* SERVICE FUNCTIONS *************
//    // Check if the ValueDef will conflict with any other ValueDef
//    bool hasValueI(const ValueDefBuilderSPtr &valueDef);
//    bool hasValueIThisAndDerived(const ValueDefBuilderSPtr &valueDef);

//    // Set the keyValue or derivedIdValue on the whole existing inheritance heiracky
//    void setKeyValueThisAndDerived(int index);
//    void setDerivedIdValueThisAndDerived(int index);

//    // Check if the ValueDef will conflict with any other ValueDef
//    bool hasContainerI(const ContainerDefUPtr& cDef);
//    bool hasContainerIThisAndDerived(const ContainerDefUPtr& cDef);






public:
//    static NodeDefSPtr Make(const std::string &name);

//    template<typename T>
//    static NodeDefSPtr MakeDerivedRoot(const std::string &name, T derivedId);

//    template<typename T>
//    static NodeDefSPtr MakeDerived(NodeDefSPtr derivedBase, T derivedId);


//    static bool addValueDef(NodeDefSPtr nodeDef, ValueDefBuilderSPtr valueDef);
//    static bool addValueDefAsKey(NodeDefSPtr nodeDef, ValueDefBuilderSPtr valueDefKey);
//    static bool addValueDefAsDerivedId(NodeDefSPtr nodeDef, ValueDefBuilderSPtr valueDefDerivedId);
    static ValueDefUPtr takeValueDef(NodeDefSPtr nodeDef, const std::string &valueName);

//    static bool addContainerDef(NodeDefSPtr nodeDef, ContainerDefUPtr cDef);
    static ContainerDefUPtr takeContainerDef(NodeDefSPtr nodeDef, const std::string &name);

//    static bool setDisplayName(NodeDefSPtr nodeDef, const std::string& displayName);

//#ifdef XML_BACKEND
//    static bool setTagName(NodeDefSPtr nodeDef, const std::string& tagName);
//#endif // XML_BACKEND

protected:
    // ************* SERVICE FUNCTIONS *************
    // Check if the ValueDef will conflict with any other ValueDef
    static bool hasValueI(NodeDefSPtr nodeDef, const ValueDefBuilderSPtr &valueDef);
    static bool hasValueIThisAndDerived(NodeDefSPtr nodeDef, const ValueDefBuilderSPtr &valueDef);

    // Set the keyValue or derivedIdValue on the whole existing inheritance heiracky
    static void setKeyValueThisAndDerived(NodeDefSPtr nodeDef, int index);
    static void setDerivedIdValueThisAndDerived(NodeDefSPtr nodeDef, int index);

    // Check if the ValueDef will conflict with any other ValueDef
    static bool hasContainerI(NodeDefSPtr nodeDef, const ContainerDefUPtr& cDef);
    static bool hasContainerIThisAndDerived(NodeDefSPtr nodeDef, const ContainerDefUPtr& cDef);




#ifdef XML_BACKEND
    static void setTagNameThisAndDerived(NodeDefSPtr nodeDef, const std::string& tagName);
#endif // XML_BACKEND

private:
    NodeDefSPtr m_nodeDef;
    NodeDefBuilderWPtr m_thisWPtr;
};

//// =============================================================================
//// (public)
//template<typename T>
//NodeDefSPtr NodeDefBuilder::MakeDerivedRoot(const std::string &name, T derivedId)
//{
//    NodeDefSPtr nodeDef = NodeDef::MakeSPtr(name, derivedId);

//#ifdef XML_BACKEND
//    if (XML::Element::validateTagName(name)) {
//        nodeDef->m_tagName = name;
//    }
//#endif // XML_BACKEND

//    return nodeDef;
//}

//// =============================================================================
//// (public)
//template<typename T>
//NodeDefSPtr NodeDefBuilder::MakeDerived(NodeDefSPtr derivedBase, T derivedId)
//{
//    // derivedBase has to be a valid pointer
//    assert(derivedBase);

//    // derivedBase has to have an valid derivedId
//    assert(!derivedBase->derivedId().isNull());

//    // The derivedId's have to be unique
//    assert(!derivedBase->validateForAny(derivedId));

//    // DerivedId's of derived definitions have to be of the same derivedId type
//    assert(derivedBase->derivedId().type() == UnionValue::GetType(derivedId));

//    NodeDefSPtr derivedDef = NodeDef::MakeSPtr(derivedBase->name(), derivedId);

//    // Adds the derived definition to the inheritance heiraki
//    derivedDef->m_derivedBase = derivedBase;
//    derivedBase->m_derivedDirectList.push_back(derivedDef);

//    // The name is the same for all definitions in the inheritance heiraki
//    derivedDef->m_name = derivedBase->m_name;

//    // The keyValue and derivedIdValue are the same for all definitions in the inheritance heiraki
//    derivedDef->m_keyValueDefIndex = derivedBase->m_keyValueDefIndex;
//    derivedDef->m_derivedIdValueDefIndex = derivedBase->m_derivedIdValueDefIndex;

//    if (derivedBase->hasDerivedId()) {
//        VDB::use(&derivedBase->derivedIdValueDef())->addOptionStatic(derivedId);
//        //VDB::addStaticOption(derivedBase->derivedIdValueDef(), );
//    }

//#ifdef XML_BACKEND
//    derivedDef->m_tagName = derivedBase->m_tagName;
//#endif // XML_BACKEND

//    return derivedDef;
//}

typedef NodeDefBuilder NDB;

} // namespace Model
} // namespace Oak
