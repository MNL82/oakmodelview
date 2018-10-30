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

namespace Oak {
namespace Model {

class NodeDef;
class ValueDef;
class ItemIndex;

// =============================================================================
// Class definition
// =============================================================================
class OptionsObserver : public ObserverInterface
{
public:
    OptionsObserver(OakModel * model, const NodeDef *optionsNodeDef, const ValueDef *optionsValueDef);

    virtual void connect() override;
    virtual void disconnect() override;

protected:
    void onEntryChangeAfter(const ItemIndex& itemIndex, const std::string &valueName);

protected:
    const NodeDef *m_optionsNodeDef;
    const ValueDef *m_optionsValueDef;

    const NodeDef *m_sourceNodeDef;
    std::string m_sourceValueName;
};


typedef std::unique_ptr<OptionsObserver> OptionsObserverUPtr;

} // namespace Model
} // namespace Oak
