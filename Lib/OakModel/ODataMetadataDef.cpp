/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ODataMetadataDef.h"

#include "NodeDefBuilder.h"
#include "ContainerDefBuilder.h"
#include "LeafDefBuilder.h"
#include "LeafQuery.h"
#include "QueryBuilder.h"
#include <memory>


namespace Oak::Model {

// =============================================================================
// (protected)
ODataMetadataDef::ODataMetadataDef()
    : NodeDef("edmx:Edmx")
{
}

// =============================================================================
// (protected)
void ODataMetadataDef::createModelDesign()
{
    auto edmx = NDB::use(m_thisWPtr.lock())
        ->setDisplayName("Edmx")
        ->addLeafDef(VDB::create(UnionType::String, "Version")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Version")))
        ->addLeafDef(VDB::create(UnionType::String, "Edmx")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("xmlns:edmx")));

    auto dataServices = NDB::create("edmx:DataServices")
        ->setDisplayName("DataSercices")
        ->addLeafDef(VDB::create(UnionType::String, "xmlns:m")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("xmlns:m")))
        ->addLeafDef(VDB::create(UnionType::String, "DataServiceVersion")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("m:DataServiceVersion")))
        ->addLeafDef(VDB::create(UnionType::String, "m:MaxDataServiceVersion")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("m:MaxDataServiceVersion")));

    edmx->addContainerDef(CDB::create(dataServices, 1, 1));

    auto schema = NDB::create("Schema")
        ->addKeyLeaf(VDB::create(UnionType::String, "Namespace")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Namespace")))
        ->addLeafDef(VDB::create(UnionType::String, "xmlns")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("xmlns")));

    dataServices->addContainerDef(CDB::create(schema, 1));

    auto entityType = NDB::create("EntityType")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addLeafDef(VDB::create(UnionType::String, "KeyProperty")
            ->setOptionsQuery(QB::createChildren("Property")->leafSPtr("Name"))
            ->setSetting(OPTION_ONLY, true)
            ->setXMLValueRef(Oak::XML::RefFactory::MakeValueRef("Key;PropertyRef", "Name")))
        ->addLeafDef(VDB::create(UnionType::String, "BaseType")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("BaseType"))
            ->setOptionsQuery(QB::createSiblings()->leafSPtr("Name"))
            ->setOptionsExcludedQuery(QB::createLeaf("Name")))
        ->addLeafDef(VDB::create(UnionType::Bool, "m:HasStream")
            ->setDefaultValue(false)
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("m:HasStream")));

    schema->addContainerDef(CDB::create(entityType));

//    auto propertyKey = NDB::create("PropertyRef")
//        ->setDisplayName("Property Key")
//        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
//            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")));

//    entityType->addContainerDef(CDB::create(propertyKey)
//        ->setElementListRef(Oak::XML::ListRef(Oak::XML::ChildRef::MakeUPtr("Key"), "PropertyRef")));

    auto property = NDB::create("Property")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setSetting(UNIQUE, true)
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addLeafDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")))
        ->addLeafDef(VDB::create(UnionType::Bool, "Nullable")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Nullable"))
            ->setDefaultValue(true))
            ->addLeafDef(VDB::create(UnionType::String, "m:FC_TargetPath")
                ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("m:FC_TargetPath")))
            ->addLeafDef(VDB::create(UnionType::String, "m:FC_ContentKind")
                ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("m:FC_ContentKind")))
            ->addLeafDef(VDB::create(UnionType::Bool, "m:FC_KeepInContent")
                ->setDefaultValue(false)
                ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("m:FC_KeepInContent")));

    entityType->addContainerDef(CDB::create(property));

    auto navigationProperty = NDB::create("NavigationProperty")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addLeafDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")))
        ->addLeafDef(VDB::create(UnionType::String, "Relationship")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Relationship")))
        ->addLeafDef(VDB::create(UnionType::String, "ToRole")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("ToRole")))
        ->addLeafDef(VDB::create(UnionType::String, "FromRole")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("FromRole")));

    entityType->addContainerDef(CDB::create(navigationProperty));

    auto action = NDB::create("Action")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addLeafDef(VDB::create(UnionType::Bool, "IsBound")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("IsBound"))
            ->setDefaultValue(false));

    schema->addContainerDef(CDB::create(action));

    auto parameter = NDB::create("Parameter")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addLeafDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type"))
            ->setDefaultValue(false));

    action->addContainerDef(CDB::create(parameter));

    auto returnType = NDB::create("ReturnType")
        ->addKeyLeaf(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")));

    action->addContainerDef(CDB::create(returnType));

    auto entityContainer = NDB::create("EntityContainer")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")));

    schema->addContainerDef(CDB::create(entityContainer, 1, 1));

    auto entitySet = NDB::create("EntitySet")
        ->addKeyLeaf(VDB::create(UnionType::String, "Name")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addLeafDef(VDB::create(UnionType::String, "EntityType")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("EntityType")));

    entityContainer->addContainerDef(CDB::create(entitySet));

    auto navigationPropertyBinding = NDB::create("NavigationPropertyBinding")
        ->addLeafDef(VDB::create(UnionType::String, "Path")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Path")))
        ->addLeafDef(VDB::create(UnionType::String, "Target")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Target")));

    entitySet->addContainerDef(CDB::create(navigationPropertyBinding));
}

// =============================================================================
// (public)
NodeDefSPtr ODataMetadataDef::MakeSPtr()
{

    ODataMetadataDefSPtr sPtr(new ODataMetadataDef());
    sPtr->m_thisWPtr = sPtr;
    sPtr->createModelDesign();
    return sPtr;
}

} // namespace Oak::Model

