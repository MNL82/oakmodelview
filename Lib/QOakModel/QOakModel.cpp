/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "QOakModel.h"

#include "QOakModelBuilderData.h"

#include "../ServiceFunctions/Trace.h"

// =============================================================================
// (public)
QOakModel::QOakModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    //TRACE("Constructor: QuickOakModel");
    updateEnabledActions();
}

// =============================================================================
// (public)
QOakModelBuilderData* QOakModel::builder() const
{
    return m_builder;
}

// =============================================================================
// (public slots)
QString QOakModel::name() const
{
    return m_name;
}

// =============================================================================
// (public slots)
void QOakModel::setName(QString name)
{
    m_name = name;
}

// =============================================================================
// (public slots)
void QOakModel::newModel()
{
    if (m_model.isDefNull()) { return; }
    m_model.createNewRootDocument(Oak::Model::NodeData::Type::XML);
    updateEnabledActions();
    if (!m_model.isNull()) { this->resetInternalData(); }
}

// =============================================================================
// (public slots)
bool QOakModel::loadModel(const QString &filePath)
{
    QString path;
    if (filePath.startsWith("file:///")) {
        path = filePath.mid(8);
    } else {
        path = filePath;
    }
    beginResetModel();
    if (m_model.loadRootNodeXML(path.toStdString())) {
        emit dataLoaded();
        updateEnabledActions();
        if (!m_model.isNull()) {
            this->resetInternalData();
        }
        endResetModel();

//        TRACE("Column Count: %i", columnCount(QModelIndex()));
//        TRACE("Row Count: %i", rowCount(QModelIndex()));
        return true;
    }
    endResetModel();
    return false;
}

// =============================================================================
// (public slots)
bool QOakModel::saveModel()
{
    if (m_model.docFilePathXML().empty()) { return false; }
    bool result = m_model.saveRootNodeXML();
    updateEnabledActions();
    return result;
}

// =============================================================================
// (public slots)
bool QOakModel::saveModelAs(const QString& filePath)
{
    QString path;
    if (filePath.startsWith("file:///")) {
        path = filePath.mid(8);
    } else {
        path = filePath;
    }
    bool result = m_model.saveRootNodeXML(path.toStdString());
    updateEnabledActions();
    return result;
}

// =============================================================================
// (private)
void QOakModel::setBuilder(QOakModelBuilderData* rootNodeDef)
{
    TRACE("setRootNodeDef()");
    m_builder = rootNodeDef;

    m_model.setRootNodeDef(m_builder->create());

    updateEnabledActions();
    emit builderChanged();

    if (!m_model.isNull()) { this->resetInternalData(); }
}

// =============================================================================
// (private)
void QOakModel::updateEnabledActions()
{
    set_newActionEnabled(!m_model.isDefNull());
    set_loadActionEnabled(!m_model.isDefNull());
    set_saveActionEnabled(!m_model.isNull() && !m_model.docFilePathXML().empty());
    set_saveAsActionEnabled(!m_model.isNull());
}

// =============================================================================
// (public)
QModelIndex QOakModel::index(int row, int column, const QModelIndex &parent) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Node parentNode = nodeFromIndex(parent);
    Oak::Model::Node node = parentNode.childAt(row);
    ASSERT(!node.isNull());
    return createModelIndex(row, column, node);
}

// =============================================================================
// (public)
QModelIndex QOakModel::parent(const QModelIndex &child) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Node childNode = nodeFromIndex(child);
    Oak::Model::Node parentNode = childNode.parent();
    ASSERT(!parentNode.isNull());
    if (parentNode == m_model.rootNode()) {
        return QModelIndex();
    }
    return createModelIndex(child.row(), child.column(), parentNode);
}

// =============================================================================
// (public)
QModelIndex QOakModel::sibling(int row, int column, const QModelIndex &idx) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Node node= nodeFromIndex(idx);
    if (row != idx.row()) {
        Oak::Model::Node parentNode = node.parent();
        node = parentNode.childAt(row);
    }
    return createModelIndex(row, column, node);
}

// =============================================================================
// (public)
int QOakModel::rowCount(const QModelIndex &parent) const
{
    if (m_model.isNull()) { return 0; }

    Oak::Model::Node node= nodeFromIndex(parent);
    return node.childCount();
}

// =============================================================================
// (public)
int QOakModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (m_model.isNull()) { return 0; }
    return 2;
}

// =============================================================================
// (public)
bool QOakModel::hasChildren(const QModelIndex &parent) const
{
    if (m_model.isNull()) { return false; }

    Oak::Model::Node node = nodeFromIndex(parent);
    return node.childCount() > 0;
}

// =============================================================================
// (public)
QVariant QOakModel::data(const QModelIndex &index, int role) const
{
    if (m_model.isNull()) { return QVariant(); }

    if (!index.isValid()) { return QVariant(); }

    Oak::Model::Node node = nodeFromIndex(index);
    ASSERT(!node.isNull());

    switch (role) {
    case DisplayName:
        return QString::fromStdString(node.def()->displayName());
    case Name:
        return QString::fromStdString(node.def()->name());
    case KeyValue: {
        if (node.hasKey()) {
            return QString::fromStdString(node.keyLeaf().value<std::string>());
        }
        return QString();
    }
    case VariantValue:
        if (node.hasVariants()) {
            return QString::fromStdString(node.variantLeaf().value<std::string>());
        }
        return QString();
    default:
        break;
    }


    return QVariant();
}

// =============================================================================
// (public)
QHash<int, QByteArray> QOakModel::roleNames() const
{
    QHash<int, QByteArray> result = QAbstractItemModel::roleNames();
    result.insert(DisplayName, QByteArrayLiteral("displayName"));
    result.insert(Name, QByteArrayLiteral("name"));
    result.insert(KeyValue, QByteArrayLiteral("keyValue"));
    result.insert(VariantValue, QByteArrayLiteral("variantValue"));
    return result;
}

// =============================================================================
// (public)
QModelIndex QOakModel::createModelIndex(int row, int column, const Oak::Model::Node& node) const
{
    return createIndex(row, column, node.nodeData().internalPtr());
}

// =============================================================================
// (public)
Oak::Model::Node QOakModel::nodeFromIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return m_model.rootNode();
    }
    return m_model.nodeFromDataPtr(index.internalPointer());
}
