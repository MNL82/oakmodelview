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

#include "QServiceFunctions.h"
#include "QNodeDefBuilderData.h"

#include "ModelBuilderData.h"
#include "QPropertyHelpers.h"

namespace Oak::Model {
    class NodeDef;
    typedef std::shared_ptr<NodeDef> NodeDefSPtr;
}

// =============================================================================
// Class definition
// =============================================================================
class QOakModelBuilderData : public QObject
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
    BUILDER_PROPERTY_OBJECT_LIST(QOakModelBuilderData, QNodeDefBuilderData, nodeDefs, nodeDef)
    BUILDER_PROPERTY_STRING_LIST(errorMessages)

public:
    QOakModelBuilderData(QObject *parent = nullptr);
    Oak::Model::ModelBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

    bool validate();
    Oak::Model::NodeDefSPtr create();

private:
    Oak::Model::ModelBuilderData * m_builder;
    Oak::Model::ModelBuilderDataUPtr m_builderOwner;
};
