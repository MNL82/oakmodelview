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
    : QAbstractItemModel(parent)
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
    m_model.createNewRootDocument(Oak::Model::NodeData::Type::XML);
    updateEnabledActions();
    if (!m_model.isNull()) { this->resetInternalData(); }
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
bool QuickOakModel::saveModel()
{
    if (m_model.docFilePathXML().empty()) { return false; }
    bool result = m_model.saveRootNodeXML();
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
    bool result = m_model.saveRootNodeXML(path.toStdString());
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

    if (!m_model.isNull()) { this->resetInternalData(); }
}

// =============================================================================
// (private)
void QuickOakModel::updateEnabledActions()
{
    set_newActionEnabled(!m_model.isDefNull());
    set_loadActionEnabled(!m_model.isDefNull());
    set_saveActionEnabled(!m_model.isNull() && !m_model.docFilePathXML().empty());
    set_saveAsActionEnabled(!m_model.isNull());
}

// =============================================================================
// (public)
QModelIndex QuickOakModel::index(int row, int column, const QModelIndex &parent) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Item parentItem = itemFromIndex(parent);
    Oak::Model::Item item = parentItem.childAt(row);
    ASSERT(!item.isNull());
    return createModelIndex(row, column, item);
}

// =============================================================================
// (public)
QModelIndex QuickOakModel::parent(const QModelIndex &child) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Item childItem = itemFromIndex(child);
    Oak::Model::Item parentItem = childItem.parent();
    ASSERT(!parentItem.isNull());
    if (parentItem == m_model.rootItem()) {
        return QModelIndex();
    }
    return createModelIndex(child.row(), child.column(), parentItem);
}

// =============================================================================
// (public)
QModelIndex QuickOakModel::sibling(int row, int column, const QModelIndex &idx) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Item item= itemFromIndex(idx);
    if (row != idx.row()) {
        Oak::Model::Item parentItem = item.parent();
        item = parentItem.childAt(row);
    }
    return createModelIndex(row, column, item);
}

// =============================================================================
// (public)
int QuickOakModel::rowCount(const QModelIndex &parent) const
{
    if (m_model.isNull()) { return 0; }

    Oak::Model::Item item= itemFromIndex(parent);
    return item.childCount();
}

// =============================================================================
// (public)
int QuickOakModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (m_model.isNull()) { return 0; }
    return 2;
}

// =============================================================================
// (public)
bool QuickOakModel::hasChildren(const QModelIndex &parent) const
{
    if (m_model.isNull()) { return false; }

    Oak::Model::Item item= itemFromIndex(parent);
    return item.childCount() > 0;
}

// =============================================================================
// (public)
QVariant QuickOakModel::data(const QModelIndex &index, int role) const
{
    if (m_model.isNull()) { return QVariant(); }

    if (!index.isValid()) { return QVariant(); }

    Oak::Model::Item item= itemFromIndex(index);
    ASSERT(!item.isNull());

    switch (role) {
    case DisplayName:
        return QString::fromStdString(item.def()->displayName());
    case Name:
        return QString::fromStdString(item.def()->name());
    case KeyValue: {
        if (item.hasKey()) {
            return QString::fromStdString(item.keyLeaf().value<std::string>());
        }
        return QString();
    }
    case VariantValue:
        if (item.hasVariants()) {
            return QString::fromStdString(item.variantLeaf().value<std::string>());
        }
        return QString();
    default:
        break;
    }


    return QVariant();
}

// =============================================================================
// (public)
QHash<int, QByteArray> QuickOakModel::roleNames() const
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
QModelIndex QuickOakModel::createModelIndex(int row, int column, const Oak::Model::Item &item) const
{
    return createIndex(row, column, item.nodeData().internalPtr());
}

// =============================================================================
// (public)
Oak::Model::Item QuickOakModel::itemFromIndex(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return m_model.rootItem();
    }
    return m_model.itemFromDataPtr(index.internalPointer());
}
