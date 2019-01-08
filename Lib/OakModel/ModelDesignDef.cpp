/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ModelDesignDef.h"

#include "NodeDefBuilder.h"
#include "ContainerDefBuilder.h"
#include "LeafDefBuilder.h"
#include "LeafQuery.h"
#include <memory>

#include "QueryBuilder.h"


namespace Oak::Model {

// =============================================================================
// (protected)
ModelDesignDef::ModelDesignDef()
    : NodeDef("Design")
{
    m_tagName = "Design";
}

// =============================================================================
// (protected)
void ModelDesignDef::createModelDesign()
{
    /************************** Node(Standard) **************************/
    auto NodeDef = NDB::createVariantRoot("Node", "Standard")
        ->setDisplayName("Node")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setDefaultValue("Noname"))
        ->addLeafDef(VDB::create(UnionType::String, "DisplayName")
            ->setDisplayName("Display Name")
            ->setTooltip("The name will be used if left empty"))
        ->addLeafDef(VDB::create(UnionType::String, "KeyValue")
            ->setDisplayName("Key Value")
            ->setOptionsQuery(QB::createChildren("Value")->leafSPtr("Name"))
            ->setOptionsExcludedQuery(LeafQuery::create()->setValueName("InheritanceIDValue"))
            ->setSetting("OptionsOnly", true))
        ->addVariantLeaf(VDB::create(UnionType::String, "Type")
            ->setDisplayName("Node Type"));

    /************************** Node(InheritanceRoot) **************************/
    auto NodeRootDef = NDB::createVariant(NodeDef, "InheritanceRoot")
        ->addLeafDef(VDB::create(UnionType::String, "NodeInheritanceId")
            ->setDisplayName("Root Inheritance ID"))
        ->addLeafDef(VDB::create(UnionType::String, "InheritanceIDValue")
            ->setDisplayName("Value for Inheritance ID")
            ->setOptionsQuery(QB::createChildren("Value")->leafSPtr("Name"))
            ->setOptionsExcludedQuery(LeafQuery::create()->setValueName("KeyValue"))
            ->setSetting("OptionsOnly", true));

    /************************** Node(Rerived) **************************/
    auto NodeVariantDef = NDB::createVariant(NodeDef, "Variant")
        ->addLeafDef(VDB::create(UnionType::String, "VariantId")
            ->setDisplayName("Variant ID"))
        ->addLeafDef(VDB::create(UnionType::String, "Base")
            ->setDisplayName("Base Node"));

    /************************** Value(String) **************************/
    auto LeafDef = NDB::createVariantRoot("Leaf", "String")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setDefaultValue("Leaf"))
        ->addLeafDef(VDB::create(UnionType::String, "DisplayName")
            ->setDisplayName("Display Name"))
        ->addVariantLeaf(VDB::create(UnionType::String, "Type"))
        ->addLeafDef(VDB::create(UnionType::String, "Tooltip"));

    NodeDef->addContainerDef(CDB::create(LeafDef));

    /************************** Value(Integer) **************************/
    auto LeafDefInt = NDB::createVariant(LeafDef, "Integer")
        ->addLeafDef(VDB::create(UnionType::Integer, "Min")
            ->setDefaultValue(-std::numeric_limits<int>::max()))
        ->addLeafDef(VDB::create(UnionType::Integer, "Max")
            ->setDefaultValue(std::numeric_limits<int>::max()));

    /************************** Value(Double) **************************/
    auto LeafDefReal = NDB::createVariant(LeafDef, "Real")
        ->addLeafDef(VDB::create(UnionType::Double, "Min")
            ->setDefaultValue(-std::numeric_limits<double>::max()))
        ->addLeafDef(VDB::create(UnionType::Double, "Max")
            ->setDefaultValue(std::numeric_limits<double>::max()));

    /************************** Container **************************/
    auto ContainerDef = NDB::create("Container")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setDisplayName("Container Node Name")
            ->setOptionsQuery(QB::createParent()->createSiblings()->leafSPtr("Name"))
            ->setSetting("OptionsOnly", true))
        ->addLeafDef(VDB::create(UnionType::Integer, "Min")
            ->setDefaultValue(0))
        ->addLeafDef(VDB::create(UnionType::Integer, "Max")
            ->setDefaultValue(std::numeric_limits<int>::max()));

    NodeDef->addContainerDef(CDB::create(ContainerDef));

    // Add the node definition to the Model design definition
    NDB::use(m_thisWPtr.lock())->addContainerDef(CDB::create(NodeDef));
}

// =============================================================================
// (public)
NodeDefSPtr ModelDesignDef::MakeSPtr()
{
    ModelDesignDefSPtr sPtr(new ModelDesignDef());
    sPtr->m_thisWPtr = sPtr;
    //sPtr->setWPtr(sPtr);
    sPtr->createModelDesign();
    return sPtr;
}

} // namespace Oak::Model

