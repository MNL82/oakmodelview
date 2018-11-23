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

#include "QuickItemQuery.h"

#include "../ServiceFunctions/PropertyHelper.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickValueQuery : public QObject
{
    Q_OBJECT
    AUTO_PROPERTY_READONLY(QString, valueName)
    AUTO_PROPERTY_OBJECT_READONLY(QuickItemQuery, itemQuery)

public:
    QuickValueQuery(QObject *parent = nullptr);

};

