/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QLeafSettingsBuilderData.h"

// =============================================================================
// (public)
QLeafSettingsBuilderData::QLeafSettingsBuilderData(QObject *parent)
    : QObject(parent)
{
    m_builderOwner = std::make_unique<Oak::Model::LeafSettingsBuilderData>();
    m_builder = m_builderOwner.get();
}
