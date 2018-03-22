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
#include "QueryRef.h"
#include <memory>

namespace Oak {
namespace Model {

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
    auto NodeDef = NDB::MakeDerivedRoot("Node", "Standard");
    NDB::setDisplayName(NodeDef, "Node");

    NDB::addValueDefAsKey(NodeDef, VDB::create(UnionType::String, "Name")
                          ->setDefaultValue("Noname"));
    NDB::addValueDef(NodeDef, VDB::create(UnionType::String, "DisplayName")
                     ->setDisplayName("Display Name"));

    NDB::addValueDef(NodeDef, VDB::create(UnionType::String, "KeyValue")
            ->setDisplayName("Key Value")
            ->setOptionsStatic(std::vector<std::string>{""})
            ->setOptionsQuery(QueryRef::MakeSPtr()->children("Value")->setValueName("Name"))
            ->setOptionsExcludedQuery(QueryRef::MakeSPtr()->setValueName("DerivedIDValue"))
            ->setSetting("OptionsOnly", true));

    NDB::addValueDefAsDerivedId(NodeDef, VDB::create(UnionType::String, "Type")
                                ->setDisplayName("Node Type"));
    NDB::addContainerDef(sPtr(), CDB::Make(NodeDef));

    /************************** Node(DerivedRoot) **************************/
    auto NodeRootDef = NDB::MakeDerived(NodeDef, "DerivedRoot");
    NDB::addValueDef(NodeRootDef, VDB::create(UnionType::String, "DerivedId")
                     ->setDisplayName("Derived ID"));

    NDB::addValueDef(NodeRootDef, VDB::create(UnionType::String, "DerivedIDValue")
            ->setDisplayName("Derived ID Value")
            ->setOptionsQuery(QueryRef::MakeSPtr()->children("Value")->setValueName("Name"))
            ->setOptionsExcludedQuery(QueryRef::MakeSPtr()->setValueName("KeyValue"))
            ->setSetting("OptionsOnly", true));

    /************************** Node(Rerived) **************************/
    auto NodeDerivedDef = NDB::MakeDerived(NodeDef, "Derived");
    NDB::addValueDef(NodeDerivedDef, VDB::create(UnionType::String, "DerivedBase")
                     ->setDisplayName("Derived Base"));

    /************************** Value(String) **************************/
    auto ValueDef = NDB::MakeDerivedRoot("Value", "String");
    NDB::addValueDefAsKey(ValueDef, VDB::create(UnionType::String, "Name")
                     ->setDefaultValue("Value"));
    NDB::addValueDef(ValueDef, VDB::create(UnionType::String, "DisplayName")
                     ->setDisplayName("Display Name"));
    NDB::addValueDefAsDerivedId(ValueDef, VDB::create(UnionType::String, "Type"));
    NDB::addValueDef(ValueDef, VDB::create(UnionType::String, "Tooltip"));

    NDB::addContainerDef(NodeDef, CDB::Make(ValueDef));

    /************************** Value(Integer) **************************/
    auto ValueDefInt = NDB::MakeDerived(ValueDef, "Integer");
    NDB::addValueDef(ValueDefInt, VDB::create(UnionType::Integer, "Min")
                     ->setDefaultValue(-std::numeric_limits<int>::max()));
    NDB::addValueDef(ValueDefInt, VDB::create(UnionType::Integer, "Max")
                     ->setDefaultValue(std::numeric_limits<int>::max()));

    /************************** Value(Double) **************************/
    auto ValueDefReal = NDB::MakeDerived(ValueDef, "Real");
    NDB::addValueDef(ValueDefReal, VDB::create(UnionType::Double, "Min")
                     ->setDefaultValue(-std::numeric_limits<double>::max()));
    NDB::addValueDef(ValueDefReal, VDB::create(UnionType::Double, "Max")
                     ->setDefaultValue(std::numeric_limits<double>::max()));

    /************************** Container **************************/
    auto ContainerDef = NDB::Make("Container");

    NDB::addValueDef(ContainerDef, VDB::create(UnionType::String, "Name")
            ->setDisplayName("Derived ID Value")
            ->setOptionsQuery(QueryRef::MakeSPtr()->parent()->parent()->children("Node")->setValueName("Name"))
            ->setOptionsExcludedQuery(QueryRef::MakeSPtr()->ignore()->parent()->children("Container")->setValueName("Name"))
            ->setSetting("OptionsOnly", true));

    NDB::addValueDef(ContainerDef, VDB::create(UnionType::Integer, "Min")
                     ->setDefaultValue(0));
    NDB::addValueDef(ContainerDef, VDB::create(UnionType::Integer, "Max")
                     ->setDefaultValue(std::numeric_limits<int>::max()));
    NDB::addContainerDef(NodeDef, CDB::Make(ContainerDef));
}

// =============================================================================
// (public)
NodeDefSPtr ModelDesignDef::MakeSPtr()
{
    ModelDesignDefSPtr sPtr(new ModelDesignDef());
    sPtr->setWPtr(sPtr);
    sPtr->createModelDesign();
    return sPtr;
}

} // namespace Model
} // namespace Oak
