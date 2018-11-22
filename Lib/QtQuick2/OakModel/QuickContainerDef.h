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

class QuickNodeDef;

#include "../ServiceFunctions/PropertyHelper.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickContainerDef : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY(QString, nodeDefName)
    AUTO_PROPERTY(int, minCount)
    AUTO_PROPERTY(int, maxCount)

public:
    QuickContainerDef(QObject *parent = nullptr);
};
