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
// Description: Node definition classes can access data from its underlieing
// node data in the tree data structure all the data nodes creates.
// =============================================================================


#include <string>
#include <vector>

#include "NodeData.h"
#include "NodeSettings.h"

#include "LeafDef.h"

#include "ContainerDef.h"
#include "ContainerGroupDef.h"

#include "Color.h"

#ifdef XML_BACKEND
#include "XMLElement.h"
#endif // XML_BACKEND


namespace Oak::Model {

class NodeDef;
typedef std::shared_ptr<NodeDef> NodeDefSPtr;
typedef std::weak_ptr<NodeDef> NodeDefWPtr;

// =============================================================================
// Enum definition
// =============================================================================
enum ValidationState {
    VALIDATION_STATE_INVALID = -1,
    VALIDATION_STATE_VALID = 0,
    VALIDATION_STATE_VALID_BASE = 1,
    VALIDATION_STATE_VALID_DERIVED = 2,
    VALIDATION_STATE_VALID_OTHER = 3
};

// =============================================================================
// Class definition
// =============================================================================
class NodeDef
{
protected:
    NodeDef(const std::string& _name);
    NodeDef(const std::string& _name, const UnionRef& _variantId);

public:
    virtual ~NodeDef();

    NodeDef(const NodeDef& copy);
    NodeDef(NodeDef&& move);

    NodeDef& operator=(const NodeDef &copy);
    NodeDef& operator=(NodeDef&& move);

    bool isNull() const;

    const std::string &name() const;
    virtual std::string displayName(bool basic = false) const;

    const std::string& tooltip() const;

#ifdef XML_BACKEND
    const std::string &tagName() const;
#endif // XML_BACKEND

    bool hasColor() const;
    const Color &color() const;

    bool hasImagePath() const;
    const std::string &imagePath() const;

    const NodeSettings& settings() const;

protected:
    NodeDefWPtr m_thisWPtr;

    /// The 'm_name' is a unike identifier for the node definition
    std::string m_name;
    std::string m_displayName;
    std::string m_tooltip;
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
    /// 'NodeDefs' can a variant of an inheritance hieraki with derived and base nodes in a tree like structure.
    /// All 'NodeDef' variants in the same hieraki will have the same 'name' but different 'variantId'.
    /// Derived 'NodeDef's will inherate all 'LeafDef's and "ContainerDef's of its base 'NodeDef'.
    /// The 'variantId' of a node is accessed with a 'LeafDef' belonging to the baseRoot so all 'NodeDef' variants
    /// of the inheritance hieraki will read the same value.
    const UnionRef variantId() const;

    std::vector<UnionRef> variantIdList(bool includeBase = false, bool includeDerived = false) const;
    void getVariantIdList(std::vector<UnionRef> &idList, bool includeBase = false, bool includeDerived = false) const;

    inline bool hasBase() const          { return !m_base.expired(); }
    inline bool hasDerived() const    { return !m_derivedList.empty(); }
    inline bool hasVariants() const           { return hasBase() || hasDerived(); }

    inline const NodeDef* base() const { return m_base.lock().get(); }
    const NodeDef* baseRoot() const;

    // Returns the valid NodeDef in the inheritance hierarki if any otherwise it returns nullptr.
    virtual const NodeDef* validVariant(const UnionRef& variantId, bool includeBase = false, bool includeDerived = true) const;
    virtual const NodeDef* validVariant(const NodeData &nodeData, bool includeBase = false, bool includeDerived = true) const;

    std::vector<const NodeDef *> variantList(bool includeBase = false, bool includeDerived = false) const;
    void getVariantList(std::vector<const NodeDef *> &vList, bool includeBase = false, bool includeDerived = false) const;

protected:
    UnionValue m_variantId;
    NodeDefWPtr m_base = NodeDefWPtr();
    std::vector<NodeDefSPtr> m_derivedList;
// *****************************************************************************


// *****************************************************************************
// Validation of data nodes
// *****************************************************************************
public:
    virtual bool validate(const UnionRef& variantId, bool includeBase = false, bool includeDerived = false) const;
    virtual bool validate(const NodeData &_nodeData, bool includeBase = false, bool includeDerived = false) const;

    virtual ValidationState validationState(const UnionRef& _variantId) const;
    virtual ValidationState validationState(const NodeData &nodeData) const;
// *****************************************************************************


// *****************************************************************************
// Value Definitions
// *****************************************************************************
public:
    virtual int valueCount(bool includeBase = true, bool includeDerived = false) const;
    virtual bool hasValue(const std::string &valueName, bool includeBase = true, bool includeDerived = false) const;
    virtual int valueIndex(const LeafDef *leafDef, bool includeBase = true, bool includeDerived = false) const;
    virtual const LeafDef& value(int index, bool includeBase = true, bool includeDerived = false) const;
    virtual const LeafDef& value(const std::string &valueName, bool includeBase = true, bool includeDerived = false) const;
    virtual LeafDef& value(int index, bool includeBase = true, bool includeDerived = false);
    virtual LeafDef& value(const std::string &valueName, bool includeBase = true, bool includeDerived = false);

    std::vector<const LeafDef*> valueList(bool includeBase = true, bool includeDerived = false) const;
    void getValueList(std::vector<const LeafDef*>& vList, bool includeBase = true, bool includeDerived = false) const;

    bool hasKey() const;
    const LeafDef& keyLeafDef() const;
    LeafDef& keyLeafDef();
    int indexOfKeyLeafDef() const { return m_indexOfKeyLeafDef; }

    const LeafDef &variantLeafDef() const;
    LeafDef &variantLeafDef();
    int indexOfVariantLeafDef() const { return m_indexOfVariantLeafDef; }

protected:
    std::vector<LeafDefUPtr> m_valueList;
    int m_indexOfKeyLeafDef = -1;
    int m_indexOfVariantLeafDef = -1;
// *****************************************************************************


// *****************************************************************************
// Node Container Definitions
// *****************************************************************************
public:
    virtual int containerCount(bool includeBase = true, bool includeDerived = false) const;
    virtual bool hasContainer(const std::string &_name, bool includeBase = true, bool includeDerived = false) const;
    virtual const ContainerDef& container(int index, bool includeBase = true, bool includeDerived = false) const;
    virtual const ContainerDef &container(const std::string &_name, bool includeBase = true, bool includeDerived = false) const;

    virtual int containerIndex(const std::string &_name, bool includeBase = true) const;

    virtual const ContainerDef& container(const NodeData &childNodeData, bool includeBase = true, bool includeDerived = false) const;

    virtual std::vector<const ContainerDef*> containerList(bool includeBase = true, bool includeDerived = false) const;
    virtual void getContainerList(std::vector<const ContainerDef*> &list, bool includeBase = true, bool includeDerived = false) const;


    virtual const NodeDef* childDef(int index, bool includeBase = true) const;
    virtual const NodeDef* childDef(const std::string &_name, bool includeBase = true, bool includeDerived = false) const;
    virtual const NodeDef* childDef(const NodeData &childNodeData, bool includeBase = true, bool includeDerived = false) const;

    std::vector<const NodeDef *> childDefList(bool includeBase = true, bool includeDerived = false) const;
    void getChildDefList(std::vector<const NodeDef *> &cList, bool includeBase = true, bool includeDerived = false) const;

    virtual const ContainerGroupDef& containerGroup() const;
protected:
    std::vector<ContainerDefUPtr> m_containerList;
    mutable ContainerGroupDefUPtr m_containerGroup;
// *****************************************************************************


// *****************************************************************************
// Parent functions
// *****************************************************************************
public:
    virtual NodeData parentNode(const NodeData &nodeData, const NodeDef** parentNodeDef = nullptr, bool includeBase = true, bool includeDerived = false) const;

    virtual int parentContainerCount(bool includeBase = true, bool includeDerived = false) const;
    virtual const ContainerDef* parentContainer(int index, bool includeBase = true, bool includeDerived = false) const;
    virtual const ContainerDef* parentContainer(const std::string& _name, bool includeBase = true, bool includeDerived = false) const;
    virtual const ContainerDef* parentContainer(const NodeData &parentNodeData, bool includeBase = true, bool includeDerived = false) const;

    virtual bool isParent(const NodeData &nodeData, const NodeData &refNodeData, bool recursive = true) const;

protected:
    std::vector<const ContainerDef*> m_parentContainerDefs;
// *****************************************************************************


// *****************************************************************************
// Service functions
// *****************************************************************************
public:
    virtual void onNodeInserted(const NodeData &_nodeData) const;
    virtual void onNodeMoved(const NodeData &_nodeData) const;
    virtual void onNodeCloned(const NodeData &_nodeData) const;
// *****************************************************************************

    template<class... _Types> inline
    static NodeDefSPtr MakeSPtr(_Types&&... _Args)
    {
        NodeDefSPtr sPtr(new NodeDef(_STD forward<_Types>(_Args)...));
        sPtr->m_thisWPtr = sPtr;
        return sPtr;
    }

    NodeDefSPtr sPtr() { return m_thisWPtr.lock(); }

    friend class NodeDefBuilder;
    friend class ContainerDefBuilder;
};

} // namespace Oak::Model

