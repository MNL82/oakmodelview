/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ObserverInterface.h"


#include "../ServiceFunctions/Trace.h"

#include "OakModel.h"


namespace Oak::Model {

// =============================================================================
// (public)
ObserverInterface::ObserverInterface(OakModel *model)
    : m_model { model }
{
    ASSERT(model);
}
// =============================================================================
// (public)
ObserverInterface::~ObserverInterface()
{
}

} // namespace Oak::Model

