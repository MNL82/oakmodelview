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

#include "Node.h"
#include "NodeSettings.h"

#include "ValueDef.h"

#include "ContainerDef.h"
#include "ContainerGroupDef.h"

#include "Color.h"

#ifdef XML_BACKEND
#include "XMLElement.h"
#endif // XML_BACKEND

namespace Oak {
namespace Model {

class NodeDef;
typedef std::shared_ptr<NodeDef> NodeDefSPtr;
typedef std::shared_ptr<NodeDef> NodeDefSPtr;
typedef std::weak_ptr<NodeDef> NodeDefWPtr;
typedef std::weak_ptr<NodeDef> NodeDefWPtr;

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
class NodeDef
{
protected:
    NodeDef(const std::string& _name);
    NodeDef(const std::string& _name, const UnionRef& _derivedId);

public:
    virtual ~NodeDef();

    NodeDef(const NodeDef& copy);
    NodeDef(NodeDef&& move);

    NodeDef& operator=(const NodeDef &copy);
    NodeDef& operator=(NodeDef&& move);

    bool isNull() const;

    const std::string &name() const;

    virtual std::string displayName(bool basic = false) const;

    bool hasColor() const;
    const Color &color() const;

    bool hasImagePath() const;
    const std::string &imagePath() const;

    const NodeSettings& settings() const;

    NodeDefSPtr sPtr() const;

    void setWPtr(NodeDefSPtr sPtr);

#ifdef XML_BACKEND
    const std::string &tagName() const;
#endif // XML_BACKEND

protected:
    NodeDefWPtr m_thisWPtr;

    /// The 'm_name' is a unike identifier for the node definition
    std::string m_name;

    std::string m_displayName;

    Color m_color;

    std::string m_imagePath;

    NodeSettings m_settings;

#ifdef XML_BACKEND
    /// The 'm_tagName' is the name of the XMLElement tag that the definition controls
    std::string m_tagName;
#endif // XML_BACKEND

// *****************************************************************************
// Node definition inheritance functionality
// *****************************************************************************
public:
    /// 'NodeDefs' can be part of an inheritance hieraki with derived and base nodes in a tree like structure.
    /// All 'NodeDef's in the same hieraki will have the same 'name' but different 'derivedId'.
    /// Derived 'NodeDef's will inherate all 'ValueDef's and "ContainerDef's of its base 'NodeDef'.
    /// The 'derivedId' of a node is stored as a 'ValueDef' belonging to the derivedRoot so all 'NodeDef's
    /// of the inheritance hieraki will have the value.
    const UnionRef derivedId() const;

    void derivedIdListAll(std::vector<UnionRef> &idList) const;
    void derivedIdListFromDerived(std::vector<UnionRef> &idList) const;
    void derivedIdListFromThisAndDerived(std::vector<UnionRef> &idList) const;

    inline bool hasDerivedBase() const    { return !m_derivedBase.expired(); }
    inline bool hasDerivedDiviations() const { return !m_derivedDirectList.empty(); }
    inline bool hasDiviations() const      { return hasDerivedBase() || hasDerivedDiviations(); }

    inline const NodeDef* derivedBase() const { return m_derivedBase.lock().get(); }
    const NodeDef* derivedRoot() const;

    // Returns the valid NodeDef in the inheritance hierarki if any otherwise it returns 0.
    virtual const NodeDef* getDerivedAny(const UnionRef& derivedId) const;
    virtual const NodeDef* getDerivedAny(Node node) const;

    // Returns the valid NodeDef in the inheritance hierarki if any otherwise it returns 0.
    // Only test derived definitions.
    virtual const NodeDef* getDerived(const UnionRef& derivedId, const NodeDef* excluding = nullptr) const;
    virtual const NodeDef* getDerived(Node node, const NodeDef* excluding = nullptr) const;

    // Returns the valid NodeDef in the inheritance hierarki if any otherwise it returns 0.
    // Only test this definition and its derived definitions.
    virtual const NodeDef* getDerivedOrThis(const UnionRef& derivedId, const NodeDef* excluding = nullptr) const;
    virtual const NodeDef* getDerivedOrThis(Node node, const NodeDef* excluding = nullptr) const;

protected:
    UnionValue m_derivedId;
    NodeDefWPtr m_derivedBase = NodeDefWPtr();
    std::vector<NodeDefSPtr> m_derivedDirectList;
// *****************************************************************************


// *****************************************************************************
// Validation of data nodes
// *****************************************************************************
public:
    virtual bool validateForThis(const UnionRef& derivedId) const;
    virtual bool validateForThis(Node _node) const;

    virtual bool validateForDerived(const UnionRef& derivedId, const NodeDef* excluding = nullptr) const;
    virtual bool validateForDerived(Node node, const NodeDef* excluding = nullptr) const;

    virtual bool validateForThisOrDerived(const UnionRef& derivedId, const NodeDef* excluding = nullptr) const;
    virtual bool validateForThisOrDerived(Node node, const NodeDef* excluding = nullptr) const;

    virtual bool validateForAny(const UnionRef& derivedId) const;
    virtual bool validateForAny(Node node) const;

    virtual ValidationState validationState(const UnionRef& _derivedId) const;
    virtual ValidationState validationState(Node node) const;
// *****************************************************************************


// *****************************************************************************
// Value Definitions
// *****************************************************************************
public:
    virtual int valueCount() const;
    virtual bool hasValue(const std::string &valueName) const;
    virtual const ValueDef& value(int index) const;
    virtual const ValueDef& value(const std::string &valueName) const;
    virtual ValueDef& value(int index);
    virtual ValueDef& value(const std::string &valueName);

    std::vector<const ValueDef*> valueList() const;
    void getValueList(std::vector<const ValueDef*>& vList) const;

    bool hasKey() const;
    const ValueDef& keyValueDef() const;
    ValueDef& keyValueDef();
    int keyValueDefIndex() const { return m_keyValueDefIndex; }

    bool hasDerivedId() const;
    const ValueDef &derivedIdValueDef() const;
    ValueDef &derivedIdValueDef();
    int derivedIdValueDefIndex() const { return m_derivedIdValueDefIndex; }

protected:
    std::vector<ValueDefUPtr> m_valueList;
    int m_keyValueDefIndex = -1;
    int m_derivedIdValueDefIndex = -1;
// *****************************************************************************


// *****************************************************************************
// Node Container Definitions
// *****************************************************************************
public:
    virtual int containerCount() const;
    virtual const ContainerDef& container(int index) const;
    const ContainerDef &container(const std::string &_name) const;

    virtual const ContainerDef& container(Node childNode) const;

    virtual std::vector<const ContainerDef*> containerList() const;
    virtual void getContainerList(std::vector<const ContainerDef*> &list) const;

    virtual const ContainerGroupDef& containerGroup() const;

    virtual const NodeDef* childDef(int index) const;
    virtual const NodeDef* childDef(const std::string &_name) const;
    virtual const NodeDef* childDef(Node childNode) const;

protected:
    std::vector<ContainerDefUPtr> m_containerList;
    mutable ContainerGroupDefUPtr m_containerGroup;
// *****************************************************************************


// *****************************************************************************
// Parent functions
// *****************************************************************************
public:
    virtual Node parentNode(Node node, const NodeDef** parentNodeDef = nullptr) const;

    virtual int parentContainerCount() const;
    virtual const ContainerDef* parentContainer(int index) const;
    virtual const ContainerDef* parentContainer(const std::string& _name) const;
    virtual const ContainerDef* parentContainer(Node parentNode) const;
#ifdef XML_BACKEND
    virtual const ContainerDef* parentContainerTagName(const std::string& tagName)const;
#endif // XML_BACKEND

    virtual bool hasParent(const NodeDef* nodeDef) const;

    virtual bool isParent(Node node, Node refNode, bool recursive = true) const;

protected:
    std::vector<const ContainerDef*> m_parentContainerDefs;
// *****************************************************************************


// *****************************************************************************
// Service functions
// *****************************************************************************
public:
    virtual void onNodeInserted(Node _node) const;
    virtual void onNodeMoved(Node _node) const;
    virtual void onNodeCloned(Node _node) const;
// *****************************************************************************

    template<class... _Types> inline
    static NodeDefSPtr MakeSPtr(_Types&&... _Args)
    {
        NodeDefSPtr sPtr(new NodeDef(_STD forward<_Types>(_Args)...));
        sPtr->m_thisWPtr = sPtr;
        return sPtr;
    }

    friend class NodeDefBuilder;
    friend class ContainerDefBuilder;
};

typedef NodeDef NodeDef;

} // namespace Model
} // namespace Oak
