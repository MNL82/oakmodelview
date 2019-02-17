/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QNodeDefBuilderData.h"

#include "../ServiceFunctions/Trace.h"

#include "NodeDefBuilderData.h"
#include "NodeDefBuilder.h"

// =============================================================================
// (public)
QNodeDefBuilderData::QNodeDefBuilderData(QObject *parent)
    : QObject(parent)
{
    //TRACE("Constructor: QuickNodeDef");
    m_builderOwner = std::make_unique<Oak::Model::NodeDefBuilderData>();
    m_builder = m_builderOwner.get();
}
