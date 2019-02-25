/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include <QImage>

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
// (public slots)
void QOakModel::testClick(const QVariant &nodeDataId)
{
    Oak::Model::Node node = toNode(nodeDataId);
    TRACE("testClick(%s)", node.def()->displayName().c_str());
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

    Oak::Model::Node parentNode = toNode(parent);
    Oak::Model::Node node = parentNode.childAt(row);
    ASSERT(!node.isNull());
    return createModelIndex(row, column, node);
}

// =============================================================================
// (public)
QModelIndex QOakModel::parent(const QModelIndex &child) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Node childNode = toNode(child);
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

    Oak::Model::Node node= toNode(idx);
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

    Oak::Model::Node node= toNode(parent);
    return node.childCount();
}

// =============================================================================
// (public)
int QOakModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (m_model.isNull()) { return 0; }
    return 1;
}

// =============================================================================
// (public)
bool QOakModel::hasChildren(const QModelIndex &parent) const
{
    if (m_model.isNull()) { return false; }

    Oak::Model::Node node = toNode(parent);
    return node.childCount() > 0;
}

// =============================================================================
// (public)
QVariant QOakModel::data(const QModelIndex &index, int role) const
{
    if (m_model.isNull()) { return QVariant(); }

    if (!index.isValid()) { return QVariant(); }

    Oak::Model::Node node = toNode(index);
    ASSERT(!node.isNull());

    switch (role) {
    case Qt::DisplayRole: // QML: display
         return QString::fromStdString(node.def()->displayName());
    case Qt::DecorationRole: { // QML: decoration
        if (node.def()->hasImagePath()) {
            return QImage(QString::fromStdString(node.def()->imagePath()));
        }
        return QVariant();
    }
    case Qt::ToolTipRole: // QML: toolTip
        return QString::fromStdString(node.def()->tooltip());
    case Qt::StatusTipRole: // QML: statusTip
        return QVariant();
    case Qt::WhatsThisRole: // QML: whatsThis
        return QVariant();
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
    case NodeDataId:
        return toNodeDataId(index);
    default:
        break;
    }

    return QVariant();
}

// =============================================================================
// (public)
Qt::ItemFlags QOakModel::flags(const QModelIndex& index) const
{
    if (m_model.isNull()) { return Qt::NoItemFlags; }

    if (!index.isValid()) { return Qt::NoItemFlags; }

    Oak::Model::Node node = toNode(index);
    ASSERT(!node.isNull());

    Qt::ItemFlags flagList;

    // Currently all nodes can be selected
    flagList |= Qt::ItemIsSelectable;

//    // No node can be edited from this model
//    flagList |= Qt::ItemIsEditable;

//    // Drag and drop will be enabled in the future
//    flagList |= Qt::ItemIsDragEnabled;
//    flagList |= Qt::ItemIsDropEnabled;

//    // Currently no nodes can be checked
//    flagList |= Qt::ItemIsUserCheckable;

    // Currently all nodes are enabled
    flagList |= Qt::ItemIsEnabled;

    if (node.def()->containerCount() == 0) {
        flagList |= Qt::ItemNeverHasChildren;
    }

    return flagList;
}

// =============================================================================
// (public)
QHash<int, QByteArray> QOakModel::roleNames() const
{
    QHash<int, QByteArray> result = QAbstractItemModel::roleNames();
    result.insert(Name, QByteArrayLiteral("name"));
    result.insert(KeyValue, QByteArrayLiteral("keyValue"));
    result.insert(VariantValue, QByteArrayLiteral("variantValue"));
    result.insert(NodeDataId, QByteArrayLiteral("nodeDataId"));
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
Oak::Model::Node QOakModel::toNode(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return m_model.rootNode();
    }
    return m_model.nodeFromDataPtr(index.internalPointer());
}

// =============================================================================
// (public)
Oak::Model::Node QOakModel::toNode(const QVariant& nodeDataId) const
{
    void *ptr = reinterpret_cast<void*>(nodeDataId.value<intptr_t>());
    if (ptr == nullptr) {
        return m_model.rootNode();
    }
    return m_model.nodeFromDataPtr(ptr);
}

// =============================================================================
// (public)
QVariant QOakModel::toNodeDataId(const QModelIndex& index) const
{
    return QVariant::fromValue(reinterpret_cast<intptr_t>(index.internalPointer()));
}
