/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ModelDesignDefinition.h"

#include "NodeDefinitionBuilder.h"
#include "ContainerDefinitionBuilder.h"
#include "ValueDefinitionBuilder.h"
#include "QueryRef.h"

namespace Oak {
namespace Model {

// Node tag names
std::string DesignStr::Design = "Design";
std::string DesignStr::Node = "Node";
std::string DesignStr::Value = "Value";
std::string DesignStr::Container = "Container";

// Value tag and attribute names
std::string DesignStr::Name = "Name";
std::string DesignStr::DisplayName = "DisplayName";
std::string DesignStr::Key = "Key";
std::string DesignStr::Type = "Type";

std::string DesignStr::DerivedId = "DerivedId";
std::string DesignStr::DerivedValue = "DerivedValue";
std::string DesignStr::DerivedBase = "DerivedBase";

// =============================================================================
// (protected)
ModelDesignDefinition::ModelDesignDefinition()
    : NodeDefinition("Design")
{
    m_tagName = "Design";
}

// =============================================================================
// (protected)
void ModelDesignDefinition::createModelDesign()
{
    /************************** Node(Standard) **************************/
    auto NodeDef = NDB::MakeDerivedRoot("Node", "Standard");
    NDB::addValueDefAsKey(NodeDef, VDB::Make("", "Name", "", "Noname"));
    NDB::addValueDef(NodeDef, VDB::Make("", "DisplayName", "Display Name"));
    NDB::addValueDefAsDerivedId(NodeDef, VDB::Make(std::string(), "Type"));
    NDB::addContainerDef(sPtr(), CDB::Make(NodeDef));

    /************************** Node(DerivedRoot) **************************/
    auto NodeRootDef = NDB::MakeDerived(NodeDef, "DerivedRoot");
    NDB::addValueDef(NodeRootDef, VDB::Make("", "DerivedId", "Derived ID"));
    NDB::addValueDef(NodeRootDef, VDB::Make("", "DerivedValue", "Derived Value"));

    /************************** Node(Rerived) **************************/
    auto NodeDerivedDef = NDB::MakeDerived(NodeDef, "Derived");
    NDB::addValueDef(NodeDerivedDef, VDB::Make("", "DerivedBase", "Derived Base"));

    /************************** Value(String) **************************/
    auto ValueDef = NDB::MakeDerivedRoot("Value", "String");
    NDB::addValueDefAsKey(ValueDef, VDB::Make("", "Name", "", "Noname"));
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
    auto cNameValueDef = VDB::Make("", "Name", "", "Noname");
    VDB::setQueryOptions(cNameValueDef, QueryRef::MakeSPtr()->parent()->ignore()->parent()->children("Node")->setValueName("Name"));
    VDB::setQueryOptionsExcluded(cNameValueDef, QueryRef::MakeSPtr()->ignore()->parent()->children("Container")->setValueName("Name"));
    NDB::addValueDefAsKey(ContainerDef, std::move(cNameValueDef));
    NDB::addValueDef(ContainerDef, VDB::Make(0, "Min", "", 0));
    NDB::addValueDef(ContainerDef, VDB::Make(0, "Max", "", std::numeric_limits<int>::max()));
    NDB::addContainerDef(NodeDef, CDB::Make(ContainerDef));
}

// =============================================================================
// (public)
NodeDefSPtr ModelDesignDefinition::MakeSPtr()
{
    ModelDesignDefSPtr sPtr(new ModelDesignDefinition());
    sPtr->setWPtr(sPtr);
    sPtr->createModelDesign();
    return sPtr;
}

} // namespace Model
} // namespace Oak
