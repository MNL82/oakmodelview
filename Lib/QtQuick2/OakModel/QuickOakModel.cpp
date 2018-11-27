/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QuickOakModel.h"

#include "QuickOakModelBuilder.h"

#include "../ServiceFunctions/Trace.h"

// =============================================================================
// (public)
QuickOakModel::QuickOakModel(QObject *parent)
    : QObject(parent)
{
    TRACE("Constructor: QuickOakModel");
}

// =============================================================================
// (public)
QuickOakModelBuilder* QuickOakModel::builder() const
{
    return m_builder;
}

// =============================================================================
// (public)
QString QuickOakModel::name() const
{
    return m_name;
}

// =============================================================================
// (public slots)
bool QuickOakModel::loadData(const QString &filePath)
{
    if (m_model.loadXMLRootNode(filePath.toStdString())) {
        emit dataLoaded();
        return true;
    }
    return false;
}

// =============================================================================
// (public slots)
void QuickOakModel::setName(QString name)
{
    m_name = name;
}

// =============================================================================
// (private)
void QuickOakModel::setBuilder(QuickOakModelBuilder* rootNodeDef)
{
    TRACE("setRootNodeDef()");
    m_builder = rootNodeDef;

    m_model.setRootNodeDef(m_builder->create());

    emit builderChanged();
}
