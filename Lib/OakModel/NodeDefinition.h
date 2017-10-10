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

// =============================================================================
// Description: Node definition classes can access data from its underlieing data
// node in the tree data structure all the data nodes creates.
// =============================================================================


#include <string>
#include <vector>
#include <assert.h>

#include <Node.h>
#include <NodeSettings.h>

#include "ValueDefinition.h"

#include "ContainerDefinition.h"
#include "ContainerGroupDefinition.h"

#ifdef XML_BACKEND
#include "XMLElement.h"
#endif // XML_BACKEND

namespace Oak {
namespace Model {

class NodeDefinition;
typedef std::shared_ptr<NodeDefinition> NodeDefinitionSPtr;
typedef std::shared_ptr<NodeDefinition> NodeDefSPtr;
typedef std::weak_ptr<NodeDefinition> NodeDefinitionWPtr;
typedef std::weak_ptr<NodeDefinition> NodeDefWPtr;

// =============================================================================
// Enum definition
// =============================================================================
enum ValidationState {
    VALIDATION_STATE_INVALID = -1,
    VALIDATION_STATE_VALID = 0,
    VALIDATION_STATE_VALID_SIBLING_BASE = 1,
    VALIDATION_STATE_VALID_SIBLING_DERIVED = 2,
    VALIDATION_STATE_VALID_SIBLING_OTHER = 3
};

// =============================================================================
// Class definition
// =============================================================================
class NodeDefinition
{
protected:
    NodeDefinition(VariantCRef _primaryKey);
    NodeDefinition(VariantCRef _primaryKey, VariantCRef _derivedId);

public:
    virtual ~NodeDefinition();

    NodeDefinition(const NodeDefinition &copy);
    NodeDefinition(NodeDefinition &&move);

    NodeDefinition& operator=(const NodeDefinition &copy);
    NodeDefinition& operator=(NodeDefinition&& move);

    bool isNull() const;

    VariantCRef primaryKey() const;

    /// The 'nodeName' is the displayed name of the node
    virtual std::string name() const;

    const NodeSettings& settings() const;

#ifdef XML_BACKEND
    const std::string &tagName() const;
#endif // XML_BACKEND

protected:
    NodeDefinitionWPtr m_thisWPtr;

    /// The 'm_primaryKey' is a unike identifier for the node definition
    Variant m_primaryKey;

    std::string m_name;

    NodeSettings m_settings;

#ifdef XML_BACKEND
    /// The 'm_tagName' is the name of the XMLElement tag that the definition controls
    std::string m_tagName;
#endif // XML_BACKEND

// *****************************************************************************
// Node definition inheritance functionality
// *****************************************************************************
public:
    /// 'NodeDefinitions' can be part of an inheritance hieraki with derived and base nodes in a tree like structure.
    /// All 'NodeDefinition's in the same hieraki will have the same 'primaryKey' but different 'derivedId'.
    /// Derived 'NodeDefinition's will inherate all 'ValueDefinition's and "ContainerDefinition's of its base 'NodeDefinition'.
    /// The 'derivedId' of a node is stored as a 'ValueDefinition' belonging to the derivedRoot so all 'NodeDefinition's
    /// of the inheritance hieraki will have the value.
    VariantCRef derivedId() const;

    void derivedIdListAll(std::vector<VariantCRef> &idList) const;
    void derivedIdListFromDerived(std::vector<VariantCRef> &idList) const;
    void derivedIdListFromThisAndDerived(std::vector<VariantCRef> &idList) const;

    inline bool hasDerivedBase() const    { return !m_derivedBase.expired(); }
    inline bool hasDerivedDiviations() const { return !m_derivedDirectList.empty(); }
    inline bool hasDiviations() const      { return hasDerivedBase() || hasDerivedDiviations(); }

    inline const NodeDefinition* derivedBase() const { return m_derivedBase.lock().get(); }
    const NodeDefinition* derivedRoot() const;

    // Returns the valid NodeDefinition in the inheritance hierarki if any otherwise it returns 0.
    virtual const NodeDefinition* getDerivedAny(VariantCRef derivedId) const;
    virtual const NodeDefinition* getDerivedAny(Node node) const;

    // Returns the valid NodeDefinition in the inheritance hierarki if any otherwise it returns 0.
    // Only test derived definitions.
    virtual const NodeDefinition* getDerived(VariantCRef derivedId, const NodeDefinition* excluding = nullptr) const;
    virtual const NodeDefinition* getDerived(Node node, const NodeDefinition* excluding = nullptr) const;

    // Returns the valid NodeDefinition in the inheritance hierarki if any otherwise it returns 0.
    // Only test this definition and its derived definitions.
    virtual const NodeDefinition* getDerivedOrThis(VariantCRef derivedId, const NodeDefinition* excluding = nullptr) const;
    virtual const NodeDefinition* getDerivedOrThis(Node node, const NodeDefinition* excluding = nullptr) const;

protected:
    Variant m_derivedId;
    NodeDefinitionWPtr m_derivedBase = NodeDefinitionWPtr();
    std::vector<NodeDefinitionSPtr> m_derivedDirectList;
// *****************************************************************************


// *****************************************************************************
// Validation of data nodes
// *****************************************************************************
public:
    virtual bool validateForThis(VariantCRef derivedId) const;
    virtual bool validateForThis(Node _node) const;

    virtual bool validateForDerived(VariantCRef derivedId, const NodeDefinition* excluding = nullptr) const;
    virtual bool validateForDerived(Node node, const NodeDefinition* excluding = nullptr) const;

    virtual bool validateForThisOrDerived(VariantCRef derivedId, const NodeDefinition* excluding = nullptr) const;
    virtual bool validateForThisOrDerived(Node node, const NodeDefinition* excluding = nullptr) const;

    virtual bool validateForAny(VariantCRef derivedId) const;
    virtual bool validateForAny(Node node) const;

    virtual ValidationState validationState(VariantCRef _derivedId) const;
    virtual ValidationState validationState(Node node) const;
// *****************************************************************************


// *****************************************************************************
// Value Definitions
// *****************************************************************************
public:
    virtual int valueCount() const;
    virtual const ValueDefinition& value(int index) const;
    virtual const ValueDefinition& value(VariantCRef valueId) const;
    virtual ValueDefinition& value(int index);
    virtual ValueDefinition& value(VariantCRef valueId);

    void getValueList(std::vector<const ValueDefinition*>& vList) const;

    bool hasKey() const;
    const ValueDefinition& keyValueDef() const;
    ValueDefinition& keyValueDef();
    int keyValueDefIndex() const { return m_keyValueDefIndex; }

    bool hasDerivedId() const;
    const ValueDefinition &derivedIdValueDef() const;
    ValueDefinition &derivedIdValueDef();
    int derivedIdValueDefIndex() const { return m_derivedIdValueDefIndex; }

protected:
    std::vector<ValueDefinitionUPtr> m_valueList;
    int m_keyValueDefIndex = -1;
    int m_derivedIdValueDefIndex = -1;
// *****************************************************************************


// *****************************************************************************
// Node Container Definitions
// *****************************************************************************
public:
    virtual int containerCount() const;
    virtual const ContainerDefinition& container(int index) const;
    template <typename T>
    const ContainerDefinition &container(const T& _primaryKey) const;

    virtual const ContainerDefinition& container(Node childNode) const;

    virtual void getContainerList(std::vector<const ContainerDefinition*> &list) const;

    virtual const ContainerGroupDefinition& containerGroup() const;

    virtual const NodeDefinition* childDefinition(int index) const;
    virtual const NodeDefinition* childDefinition(VariantCRef _primaryKey) const;
    virtual const NodeDefinition* childDefinition(Node childNode) const;

protected:
    std::vector<ContainerDefinitionUPtr> m_containerList;
    mutable ContainerGroupDefinitionUPtr m_containerGroup;
// *****************************************************************************


// *****************************************************************************
// Parent functions
// *****************************************************************************
public:
    virtual Node parentNode(Node node, const NodeDefinition** parentNodeDefinition = nullptr) const;

    virtual int parentContainerCount() const;
    virtual const ContainerDefinition* parentContainer(int index) const;
    virtual const ContainerDefinition* parentContainer(VariantCRef _primaryKey) const;
    virtual const ContainerDefinition* parentContainer(Node parentNode) const;
#ifdef XML_BACKEND
    virtual const ContainerDefinition* parentContainer(const std::string& tagName)const;
#endif // XML_BACKEND

    virtual bool hasParent(const NodeDefinition* nodeDefinition) const;

    virtual bool isParent(Node node, Node refNode, bool recursive = true) const;

protected:
    std::vector<const ContainerDefinition*> m_parentContainerDefinitions;
// *****************************************************************************


// *****************************************************************************
// Service functions
// *****************************************************************************
public:
    virtual void onNodeCreated(Node _node) const;
    virtual void onNodeInserted(Node _node) const;
// *****************************************************************************

    template<class... _Types> inline
    static typename NodeDefinitionSPtr MakeSPtr(_Types&&... _Args)
    {
        NodeDefinitionSPtr sPtr(new NodeDefinition(_STD forward<_Types>(_Args)...));
        sPtr->m_thisWPtr = sPtr;
        return sPtr;
    }

    friend class NodeDefinitionBuilder;
    friend class ContainerDefinitionBuilder;
};

// =============================================================================
// (public)
template <typename T>
const ContainerDefinition &NodeDefinition::container(const T& _primaryKey) const
{
    for (const auto& _child: m_containerList)
    {
        if (_child->containerDefinition()->primaryKey() == _primaryKey) {
            return *_child.get();
        }
    }

    if (hasDerivedBase()) {
        return m_derivedBase.lock()->container(_primaryKey);
    }

    assert(false);
    return ContainerDefinition::emptyChildNodeDefinition();
}

typedef NodeDefinition NodeDef;

} // namespace Model
} // namespace Oak
