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
#include "ValueDefBuilder.h"
#include "EntryQuery.h"
#include <memory>

namespace Oak {
namespace Model {

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
    auto edmx = NDB::use(this->sPtr())
        ->setDisplayName("Edmx")
        ->addValueDef(VDB::create(UnionType::String, "Version")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Version")))
        ->addValueDef(VDB::create(UnionType::String, "Edmx")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("xmlns:edmx")));

    auto dataServices = NDB::create("edmx:DataServices")
        ->setDisplayName("DataSercices");

    edmx->addContainerDef(CDB::create(dataServices, 1, 1));

    auto schema = NDB::create("Schema")
        ->addValueKey(VDB::create(UnionType::String, "Namespace")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Namespace")))
        ->addValueDef(VDB::create(UnionType::String, "xmlns")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("xmlns")));

    dataServices->addContainerDef(CDB::create(schema, 1));

    auto entityType = NDB::create("EntityType")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")));

    schema->addContainerDef(CDB::create(entityType));

    auto propertyKey = NDB::create("PropertyRef")
        ->setDisplayName("Property Key")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")));

    entityType->addContainerDef(CDB::create(propertyKey)
        ->setElementListRef(Oak::XML::ListRef(Oak::XML::ChildRef::MakeUPtr("Key"), "PropertyRef")));

    auto property = NDB::create("Property")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")))
        ->addValueDef(VDB::create(UnionType::Bool, "Nullable")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Nullable"))
            ->setDefaultValue(true));

    entityType->addContainerDef(CDB::create(property));

    auto navigationProperty = NDB::create("NavigationProperty")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")));

    entityType->addContainerDef(CDB::create(navigationProperty));

    auto action = NDB::create("Action")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::Bool, "IsBound")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("IsBound"))
            ->setDefaultValue(false));

    schema->addContainerDef(CDB::create(action));

    auto parameter = NDB::create("Parameter")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type"))
            ->setDefaultValue(false));

    action->addContainerDef(CDB::create(parameter));

    auto returnType = NDB::create("ReturnType")
        ->addValueKey(VDB::create(UnionType::String, "Type")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Type")));

    action->addContainerDef(CDB::create(returnType));

    auto entityContainer = NDB::create("EntityContainer")
        ->addValueKey(VDB::create(UnionType::String, "Name")
            ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")));

    schema->addContainerDef(CDB::create(entityContainer, 1, 1));

    auto entitySet = NDB::create("EntitySet")
        ->addValueKey(VDB::create(UnionType::String, "Name")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Name")))
        ->addValueDef(VDB::create(UnionType::String, "EntityType")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("EntityType")));

    entityContainer->addContainerDef(CDB::create(entitySet));

    auto navigationPropertyBinding = NDB::create("NavigationPropertyBinding")
        ->addValueKey(VDB::create(UnionType::String, "Path")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Path")))
        ->addValueKey(VDB::create(UnionType::String, "Target")
             ->setXMLValueRef(Oak::XML::ValueRef::MakeUPtr("Target")));

    entitySet->addContainerDef(CDB::create(navigationPropertyBinding));
}

// =============================================================================
// (public)
NodeDefSPtr ODataMetadataDef::MakeSPtr()
{

    ODataMetadataDefSPtr sPtr(new ODataMetadataDef());
    sPtr->setWPtr(sPtr);
    sPtr->createModelDesign();
    return sPtr;
}

} // namespace Model
} // namespace Oak
