/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QuickValueSettings.h"

// =============================================================================
// (public)
QuickValueSettings::QuickValueSettings(QObject *parent)
    : QObject(parent), a_unique { QuickTriState::Undefined }, a_required { QuickTriState::Undefined }
{

}
