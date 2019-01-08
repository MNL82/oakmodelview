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

#include <string>

#include "ObserverInterface.h"
#include "UnionValue.h"


namespace Oak::Model {

class NodeDef;
class LeafDef;
class ItemIndex;

class LeafQuery;
typedef std::shared_ptr<LeafQuery> LeafQuerySPtr;

// =============================================================================
// Class definition
// =============================================================================
class OptionsObserver : public ObserverInterface
{
public:
    OptionsObserver(OakModel * model, const NodeDef *optionsNodeDef, const LeafDef *optionsLeafDef);

    virtual void connect() override;
    virtual void disconnect() override;

protected:
    void onItemInserteAfter(const ItemIndex& itemIndex);
    void onItemMoveAfter(const ItemIndex& sourceItemIndex, const ItemIndex& targetItemIndex);
    void onItemMoveBefore(const ItemIndex& sourceItemIndex, const ItemIndex& targetItemIndex);
    void onItemCloneAfter(const ItemIndex& sourceItemIndex, const ItemIndex& targetItemIndex);

    void onItemRemoveBefore(const ItemIndex& itemIndex);

    void onLeafChangeBefore(const ItemIndex& itemIndex, const std::string &valueName);
    void onLeafChangeAfter(const ItemIndex& itemIndex, const std::string &valueName);

protected:
    const NodeDef *m_optionsNodeDef;
    const LeafDef *m_optionsLeafDef;
    LeafQuerySPtr m_inverseQuery;

    const NodeDef *m_sourceNodeDef;
    std::string m_sourceValueName;

    UnionValue m_valueBeforeChange;
};


typedef std::unique_ptr<OptionsObserver> OptionsObserverUPtr;

} // namespace Oak::Model

