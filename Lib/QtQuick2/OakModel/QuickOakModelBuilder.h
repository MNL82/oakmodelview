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
#include <memory>

#include "QuickNodeDef.h"

#include "../ServiceFunctions/PropertyHelper.h"

namespace Oak::Model {
    class NodeDef;
    typedef std::shared_ptr<NodeDef> NodeDefSPtr;
}

// =============================================================================
// Class definition
// =============================================================================
class QuickOakModelBuilder : public QObject
{
    Q_OBJECT

public:
    enum StatusEnum {
        INITIAL = 0,
        SUCCESS = 1,
        FAILED = 2
    };
    Q_ENUM(StatusEnum)

private:
    AUTO_PROPERTY_OBJECT_LIST(QuickOakModelBuilder, QuickNodeDef, nodeDefs, nodeDef)

    AUTO_PROPERTY(StatusEnum, status)
    AUTO_PROPERTY_LIST(QuickOakModelBuilder, QString, errorMessages, errorMessage)

public:
    QuickOakModelBuilder(QObject *parent = nullptr);

    Oak::Model::NodeDefSPtr createModel();
};
