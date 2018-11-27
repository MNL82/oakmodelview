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

#include "QuickValueOptions.h"
#include "QuickValueSettings.h"

#include "Union.h"
#include "QuickPropertyHelpers.h"

#include "ValueDefBuilderData.h"

// =============================================================================
// Class definition
// =============================================================================
class QuickValueDef : public QObject
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
    BUILDER_PROPERTY_VARIANT(defaultValue)
    BUILDER_PROPERTY_ENUM(dataTypeEnum, Oak::Model::UnionType, dataType)
    BUILDER_PROPERTY_OBJECT(QuickValueSettings, settings)
    BUILDER_PROPERTY_OBJECT(QuickValueOptions, options)

public:
    QuickValueDef(QObject* parent = nullptr);
    Oak::Model::ValueDefBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::ValueDefBuilderData * m_builder;
    Oak::Model::ValueDefBuilderDataUPtr m_builderOwner;
};
