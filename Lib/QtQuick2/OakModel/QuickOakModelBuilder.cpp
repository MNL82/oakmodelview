/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QuickOakModelBuilder.h"
#include "NodeDef.h"

#include "../ServiceFunctions/Trace.h"

// =============================================================================
// (public)
QuickOakModelBuilder::QuickOakModelBuilder(QObject* parent)
    : QObject(parent)
{


}
// =============================================================================
// (public)
Oak::Model::NodeDefSPtr QuickOakModelBuilder::createModel()
{
    // TODO: Create node model

    set_status(a_errorMessages.empty() ? StatusEnum::SUCCESS : StatusEnum::FAILED);
    return Oak::Model::NodeDefSPtr();
}
