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
#include "ValueDefBuilder.h"
#include "EntryQuery.h"
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
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setDefaultValue("Noname"))
        ->addValueDef(VDB::create(UnionType::String, "DisplayName")
            ->setDisplayName("Display Name")
            ->setTooltip("The name will be used if left empty"))
        ->addValueDef(VDB::create(UnionType::String, "KeyValue")
            ->setDisplayName("Key Value")
            ->setOptionsQuery(QB::createChildren("Value")->EntryUPtr("Name"))
            ->setOptionsExcludedQuery(EntryQuery::create()->setValueName("InheritanceIDValue"))
            ->setSetting("OptionsOnly", true))
        ->addValueVariant(VDB::create(UnionType::String, "Type")
            ->setDisplayName("Node Type"));

    /************************** Node(InheritanceRoot) **************************/
    auto NodeRootDef = NDB::createVariant(NodeDef, "InheritanceRoot")
        ->addValueDef(VDB::create(UnionType::String, "NodeInheritanceId")
            ->setDisplayName("Root Inheritance ID"))
        ->addValueDef(VDB::create(UnionType::String, "InheritanceIDValue")
            ->setDisplayName("Value for Inheritance ID")
            ->setOptionsQuery(QB::createChildren("Value")->EntryUPtr("Name"))
            ->setOptionsExcludedQuery(EntryQuery::create()->setValueName("KeyValue"))
            ->setSetting("OptionsOnly", true));

    /************************** Node(Rerived) **************************/
    auto NodeVariantDef = NDB::createVariant(NodeDef, "Variant")
        ->addValueDef(VDB::create(UnionType::String, "VariantId")
            ->setDisplayName("Variant ID"))
        ->addValueDef(VDB::create(UnionType::String, "Base")
            ->setDisplayName("Base Node"));

    /************************** Value(String) **************************/
    auto ValueDef = NDB::createVariantRoot("Value", "String")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setDefaultValue("Value"))
        ->addValueDef(VDB::create(UnionType::String, "DisplayName")
            ->setDisplayName("Display Name"))
        ->addValueVariant(VDB::create(UnionType::String, "Type"))
        ->addValueDef(VDB::create(UnionType::String, "Tooltip"));

    NodeDef->addContainerDef(CDB::create(ValueDef));

    /************************** Value(Integer) **************************/
    auto ValueDefInt = NDB::createVariant(ValueDef, "Integer")
        ->addValueDef(VDB::create(UnionType::Integer, "Min")
            ->setDefaultValue(-std::numeric_limits<int>::max()))
        ->addValueDef(VDB::create(UnionType::Integer, "Max")
            ->setDefaultValue(std::numeric_limits<int>::max()));

    /************************** Value(Double) **************************/
    auto ValueDefReal = NDB::createVariant(ValueDef, "Real")
        ->addValueDef(VDB::create(UnionType::Double, "Min")
            ->setDefaultValue(-std::numeric_limits<double>::max()))
        ->addValueDef(VDB::create(UnionType::Double, "Max")
            ->setDefaultValue(std::numeric_limits<double>::max()));

    /************************** Container **************************/
    auto ContainerDef = NDB::create("Container")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setDisplayName("Container Node Name")
            ->setOptionsQuery(QB::createParent()->createSiblings()->EntryUPtr("Name"))
            ->setSetting("OptionsOnly", true))
        ->addValueDef(VDB::create(UnionType::Integer, "Min")
            ->setDefaultValue(0))
        ->addValueDef(VDB::create(UnionType::Integer, "Max")
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

