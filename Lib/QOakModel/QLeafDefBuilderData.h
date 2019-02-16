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
#include <QVariant>

#include "QValueOptionsBuilderData.h"
#include "QLeafSettingsBuilderData.h"

#include "Union.h"
#include "QPropertyHelpers.h"

#include "LeafDefBuilderData.h"

// =============================================================================
// Class definition
// =============================================================================
class QLeafDefBuilderData : public QObject
{
    Q_OBJECT
public:
    enum dataTypeEnum {
        Undefined = -1,
        Char = 0,
        Boolean = 1,
        Integer = 2,
        Double = 3,
        String = 4,
        DateTime = 5
    };
    Q_ENUM(dataTypeEnum)

private:
    BUILDER_PROPERTY_STRING(name)
    BUILDER_PROPERTY_STRING(displayName)
    BUILDER_PROPERTY_STRING(tooltip)
    BUILDER_PROPERTY_VARIANT(defaultValue)
    BUILDER_PROPERTY_ENUM(dataTypeEnum, Oak::Model::UnionType, dataType)
    BUILDER_PROPERTY_OBJECT(QLeafSettingsBuilderData, settings)
    BUILDER_PROPERTY_OBJECT(QValueOptionsBuilderData, options)

public:
    QLeafDefBuilderData(QObject* parent = nullptr);
    Oak::Model::LeafDefBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::LeafDefBuilderData * m_builder;
    Oak::Model::LeafDefBuilderDataUPtr m_builderOwner;
};
