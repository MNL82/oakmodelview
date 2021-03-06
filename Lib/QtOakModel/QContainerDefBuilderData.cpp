/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QContainerDefBuilderData.h"

#include "../ServiceFunctions/Trace.h"

#include "NodeDef.h"

// =============================================================================
// (public)
QContainerDefBuilderData::QContainerDefBuilderData(QObject *parent)
    : QObject(parent)
{
    //TRACE("Constructor: QuickContainerDef");
    m_builderOwner = std::make_unique<Oak::Model::ContainerDefBuilderData>();
    m_builder = m_builderOwner.get();
}
