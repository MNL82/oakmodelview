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

#include <QuickItemQuery.h>

// =============================================================================
// Class definition
// =============================================================================
class QuickSiblingsQuery : public QuickItemQuery
{
    Q_OBJECT
public:
    QuickSiblingsQuery(QObject *parent = nullptr);
    ~QuickSiblingsQuery() {}

};

