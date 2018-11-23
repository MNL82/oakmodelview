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
#include <QObject>
#include <QString>

#include "QuickOakBaseTypes.h"

#include "../ServiceFunctions/PropertyHelper.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickValueSettings : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY_READONLY(bool, readOnly)
    AUTO_PROPERTY_READONLY(QString, unit)
    AUTO_PROPERTY_READONLY(QuickTriState::TriStateEnum, unique)
    AUTO_PROPERTY_READONLY(QuickTriState::TriStateEnum, required)

public:
    QuickValueSettings(QObject *parent = nullptr);

};
