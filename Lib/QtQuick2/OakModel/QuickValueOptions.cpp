/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QuickValueOptions.h"

// =============================================================================
// (public)
QuickValueOptions::QuickValueOptions(QObject *parent)
    : QObject(parent)
{
    m_builderOwner = std::make_unique<Oak::Model::ValueOptionsBuilderData>();
    m_builder = m_builderOwner.get();
}
