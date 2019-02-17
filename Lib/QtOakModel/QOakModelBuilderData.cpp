/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QOakModelBuilderData.h"

#include <string>

#include "NodeDef.h"

#include "../ServiceFunctions/Trace.h"

// =============================================================================
// (public)
QOakModelBuilderData::QOakModelBuilderData(QObject* parent)
    : QObject(parent)
{
    m_builderOwner = std::make_unique<Oak::Model::ModelBuilderData>();
    m_builder = m_builderOwner.get();
}

// =============================================================================
// (public)
bool QOakModelBuilderData::validate()
{
    return m_builder->validateModel();
}

// =============================================================================
// (public)
Oak::Model::NodeDefSPtr QOakModelBuilderData::create()
{
    return m_builder->createModel();
}
