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

#include <memory>


namespace Oak::Model {

class OakModel;

// =============================================================================
// Class definition
// =============================================================================
class ObserverInterface
{
public:
    ObserverInterface(OakModel * model);
    virtual ~ObserverInterface();

    virtual void connect() = 0;
    virtual void disconnect() = 0;

protected:
    OakModel * m_model;
};


typedef std::unique_ptr<ObserverInterface> ObserverInterfaceUPtr;

} // namespace Oak::Model

