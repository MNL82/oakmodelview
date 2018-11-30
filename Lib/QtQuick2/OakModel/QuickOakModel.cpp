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
    //TRACE("Constructor: QuickOakModel");
    updateEnabledActions();
}

// =============================================================================
// (public)
QuickOakModelBuilder* QuickOakModel::builder() const
{
    return m_builder;
}

// =============================================================================
// (public slots)
QString QuickOakModel::name() const
{
    return m_name;
}

// =============================================================================
// (public slots)
void QuickOakModel::setName(QString name)
{
    m_name = name;
}

// =============================================================================
// (public slots)
void QuickOakModel::newModel()
{
    if (m_model.isDefNull()) { return; }
    m_model.createNewRootDocument(Oak::Model::Node::Type::XML);
    updateEnabledActions();
}

// =============================================================================
// (public slots)
bool QuickOakModel::loadModel(const QString &filePath)
{
    QString path;
    if (filePath.startsWith("file:///")) {
        path = filePath.mid(8);
    } else {
        path = filePath;
    }
    if (m_model.loadXMLRootNode(path.toStdString())) {
        emit dataLoaded();
        updateEnabledActions();
        return true;
    }
    return false;
}

// =============================================================================
// (public slots)
bool QuickOakModel::saveModel()
{
    if (m_model.xmlDocFilePath().empty()) { return false; }
    bool result = m_model.saveXMLRootNode();
    updateEnabledActions();
    return result;
}

// =============================================================================
// (public slots)
bool QuickOakModel::saveModelAs(const QString& filePath)
{
    QString path;
    if (filePath.startsWith("file:///")) {
        path = filePath.mid(8);
    } else {
        path = filePath;
    }
    bool result = m_model.saveXMLRootNode(path.toStdString());
    updateEnabledActions();
    return result;
}

// =============================================================================
// (private)
void QuickOakModel::setBuilder(QuickOakModelBuilder* rootNodeDef)
{
    TRACE("setRootNodeDef()");
    m_builder = rootNodeDef;

    m_model.setRootNodeDef(m_builder->create());

    updateEnabledActions();
    emit builderChanged();
}

// =============================================================================
// (private)
void QuickOakModel::updateEnabledActions()
{
    set_newActionEnabled(!m_model.isDefNull());
    set_loadActionEnabled(!m_model.isDefNull());
    set_saveActionEnabled(!m_model.isNull() && !m_model.xmlDocFilePath().empty());
    set_saveAsActionEnabled(!m_model.isNull());
}
