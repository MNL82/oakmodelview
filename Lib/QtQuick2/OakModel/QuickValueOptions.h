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
#include <QQmlListProperty>
#include <QVariant>
#include <QVariantList>

#include "ValueOptionsBuilderData.h"

#include "QuickPropertyHelpers.h"

// =============================================================================
// Class definition
// =============================================================================
class QValueOptionsBuilderData : public QObject
{
    Q_OBJECT

    BUILDER_PROPERTY_VARIANT_LIST(values)
    BUILDER_PROPERTY_VARIANT_LIST(excludedValues)
    BUILDER_PROPERTY_STRING(valueQuery)
    BUILDER_PROPERTY_STRING(excludedValueQuery)

public:
    QValueOptionsBuilderData(QObject *parent = nullptr);
    Oak::Model::ValueOptionsBuilderDataUPtr takeBuilder() { return std::move(m_builderOwner); }

private:
    Oak::Model::ValueOptionsBuilderData * m_builder;
    Oak::Model::ValueOptionsBuilderDataUPtr m_builderOwner;
};
