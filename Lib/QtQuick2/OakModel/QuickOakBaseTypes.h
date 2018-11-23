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

class QuickTriState : public QObject {
    Q_OBJECT
public:
    enum TriStateEnum {
        Undefined = -1,
        False = 0,
        True = 1
    };
    Q_ENUM(TriStateEnum)

    QuickTriState() : QObject() {}
};

