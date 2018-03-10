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
    NDB::addValueDefAsKey(NodeDef, VDB::Make("", "Name", "", "Noname"));
    NDB::addValueDef(NodeDef, VDB::Make("", "TagName", "XML Tag Name"));
    NDB::addValueDef(NodeDef, VDB::Make("", "DisplayName", "Display Name"));

    auto keyValueDef = VDB::Make("", "KeyValue", "Key Value", "");
    VDB::setStaticOptions(keyValueDef, std::vector<std::string>{""});
    VDB::setQueryOptions(keyValueDef, QueryRef::MakeSPtr()->children("Value")->setValueName("Name"));
    VDB::setQueryOptionsExcluded(keyValueDef, QueryRef::MakeSPtr()->setValueName("DerivedIDValue"));
    VDB::settings(keyValueDef).setOptionsOnly(true);
    NDB::addValueDef(NodeDef, std::move(keyValueDef));

    NDB::addValueDefAsDerivedId(NodeDef, VDB::Make("", "Type", "Node Type"));
    NDB::addContainerDef(sPtr(), CDB::Make(NodeDef));

    /************************** Node(DerivedRoot) **************************/
    auto NodeRootDef = NDB::MakeDerived(NodeDef, "DerivedRoot");
    NDB::addValueDef(NodeRootDef, VDB::Make("", "DerivedId", "Derived ID"));

    auto DerivedIDValueDef = VDB::Make("", "DerivedIDValue", "Derived ID Value");
    VDB::setQueryOptions(DerivedIDValueDef, QueryRef::MakeSPtr()->children("Value")->setValueName("Name"));
    VDB::setQueryOptionsExcluded(DerivedIDValueDef, QueryRef::MakeSPtr()->setValueName("KeyValue"));
    VDB::settings(DerivedIDValueDef).setOptionsOnly(true);
    NDB::addValueDef(NodeRootDef, std::move(DerivedIDValueDef));

    /************************** Node(Rerived) **************************/
    auto NodeDerivedDef = NDB::MakeDerived(NodeDef, "Derived");
    NDB::addValueDef(NodeDerivedDef, VDB::Make("", "DerivedBase", "Derived Base"));

    /************************** Value(String) **************************/
    auto ValueDef = NDB::MakeDerivedRoot("Value", "String");
    NDB::addValueDefAsKey(ValueDef, VDB::Make("", "Name", "", "Value"));
    NDB::addValueDef(ValueDef, VDB::Make("", "DisplayName", "Display Name"));
    NDB::addValueDefAsDerivedId(ValueDef, VDB::Make("", "Type"));

    NDB::addContainerDef(NodeDef, CDB::Make(ValueDef));

    /************************** Value(Integer) **************************/
    auto ValueDefInt = NDB::MakeDerived(ValueDef, "Integer");
    NDB::addValueDef(ValueDefInt, VDB::Make(0, "Min", "", -std::numeric_limits<int>::max()));
    NDB::addValueDef(ValueDefInt, VDB::Make(0, "Max", "", std::numeric_limits<int>::max()));

    /************************** Value(Double) **************************/
    auto ValueDefReal = NDB::MakeDerived(ValueDef, "Real");
    NDB::addValueDef(ValueDefReal, VDB::Make(0.0, "Min", "", -std::numeric_limits<double>::max()));
    NDB::addValueDef(ValueDefReal, VDB::Make(0.0, "Max", "", std::numeric_limits<double>::max()));

    /************************** Container **************************/
    auto ContainerDef = NDB::Make("Container");
    auto cNameValueDef = VDB::Make("", "Name");
    VDB::setQueryOptions(cNameValueDef, QueryRef::MakeSPtr()->parent()->parent()->children("Node")->setValueName("Name"));
    VDB::setQueryOptionsExcluded(cNameValueDef, QueryRef::MakeSPtr()->ignore()->parent()->children("Container")->setValueName("Name"));
    VDB::settings(cNameValueDef).setOptionsOnly(true);
    NDB::addValueDefAsKey(ContainerDef, std::move(cNameValueDef));
    NDB::addValueDef(ContainerDef, VDB::Make(0, "Min", "", 0));
    NDB::addValueDef(ContainerDef, VDB::Make(0, "Max", "", std::numeric_limits<int>::max()));
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
