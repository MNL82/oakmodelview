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

#include "NodeDef.h"
#include "Item.h"
#include "CallbackFunctions.h"

#ifdef XML_BACKEND
#include "XMLDocument.h"
#endif // XML_BACKEND

namespace Oak {
namespace Model {

// =============================================================================
// Class definition
// =============================================================================
class OakModel
{
public:
    OakModel();
    virtual ~OakModel();

    bool isNull() const;
    bool isDefNull() const;
    bool isNodeNull() const;

    bool createNewRootDocument(Node::Type backendType, bool setAsCurrent = true);
    void clearRoot();

    const Item& rootItem() const;

    const std::string &rootDefName() const;

    const NodeDef* rootNodeDef() const;
    void setRootNodeDef(NodeDefSPtr def);
    void setRootNodeDef(const NodeDef* def);

    void setRootNode(Node node);

    void setRootItem(const Item& item);

    const Item& currentItem() const;
    void setCurrentItem(Item item) const;

#ifdef XML_BACKEND
    const std::string& xmlDocFilePath() { return m_xmlDocFilePath; }
    void setXMLDocFilePath(const std::string xmlDocFilePath) { m_xmlDocFilePath = xmlDocFilePath; }
    void setXMLRootNode(Node rootNode, bool setAsCurrent = true);
    bool loadXMLRootNode(const std::string& filePath, bool setAsCurrent = true);
    bool saveXMLRootNode(const std::string& filePath = "");
#endif // XML_BACKEND

    // Starting with the root NodeDef and look through all NodeDefs recursive
    //  untill a match is found
    const NodeDef *findNodeDef(Node node) const;

protected:
    void onItemInserted(const Item& parentItem, int index) const;
    void onItemMoved(const Item& sourceParentItem, int sourceIndex, const Item& targetParentItem, int targetIndex) const;
    void onItemCloned(const Item& sourceParentItem, int sourceIndex, const Item& targetParentItem, int targetIndex) const;
    void onItemRemoved(const Item& parentItem, int index) const;
    void onItemBeforeRemoving(const Item &item) const;

    void onEntryChanged(const Item& item, int valueIndex) const;

public:
    Callback notifier_currentItemChanged;
    Callback notifier_rootNodeChanged;
    Callback notifier_rootNodeDefChanged;

    Callback notifier_destroyed;

    Callback_ItemInt notifier_itemInserted;
    Callback_ItemIntItemInt notifier_itemMoved;
    Callback_ItemIntItemInt notifier_itemCloned;
    Callback_ItemInt notifier_itemRemoved;
    Callback_Item notifier_itemBeforeRemoving;

    Callback_ItemInt notifier_entryChanged;

protected:
    Item m_rootItem;
    mutable Item m_currentItem;

    // Used only to keep the definition alive (Smart Pointer)
    NodeDefSPtr m_def;

#ifdef XML_BACKEND
    Node m_XMLRootNode;
    std::string m_xmlDocFilePath;
    XML::Document m_xmlDoc;
#endif // XML_BACKEND

    friend class Item;
    friend class Entry;
};

} // namespace Model
} // namespace Oak
