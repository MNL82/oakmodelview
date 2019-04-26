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
    m_model.notifier_keyLeafChangeAfter.add(this, &QOakModel::onKeyLeafChanged);
    m_model.notifier_variantLeafChangeAfter.add(this, &QOakModel::onVariantLeafChanged);
    m_model.notifier_nodeInserteBefore.add(this,&QOakModel::onNodeInserteBefore);
    m_model.notifier_nodeInserteAfter.add(this, &QOakModel::onNodeInserteAfter);
    m_model.notifier_nodeRemoveBefore.add(this, &QOakModel::onNodeRemoveBefore);
    m_model.notifier_nodeRemoveAfter.add(this, &QOakModel::onNodeRemoveAfter);
    m_model.notifier_nodeMoveBefore.add(this, &QOakModel::onNodeMoveBefore);
    m_model.notifier_nodeMoveAfter.add(this, &QOakModel::onNodeMoveAfter);
    m_model.notifier_nodeCloneBefore.add(this, &QOakModel::onNodeCloneBefore);
    m_model.notifier_nodeCloneAfter.add(this, &QOakModel::onNodeCloneAfter);
    updateEnabledActions();
}

// =============================================================================
// (public)
QOakModelBuilderData* QOakModel::builder() const
{
    return m_builder;
}

// =============================================================================
// (public)
QString QOakModel::name() const
{
    return m_name;
}

// =============================================================================
// (public)
Oak::Model::OakModel *QOakModel::oakModel()
{
    return &m_model;
}

// =============================================================================
// (public)
const Oak::Model::OakModel *QOakModel::oakModel() const
{
    return &m_model;
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
    beginResetModel();
    m_model.createNewRootDocument(Oak::Model::NodeData::Type::XML);
    if (!m_model.isNull()) {
        this->resetInternalData();
    }
    endResetModel();
    updateEnabledActions();
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
    //TRACE("setRootNodeDef()");
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
void QOakModel::onVariantLeafChanged(const Oak::Model::NodeIndex& nIndex)
{
    Oak::Model::Node node = nIndex.node(m_model.rootNode());
    if (node.isNull()) { return; }
    QModelIndex index = createModelIndex(node);
    dataChanged(index, index, QVector<int>() << Qt::DisplayRole << QOakModel::VariantValue);
}

// =============================================================================
// (public)
void QOakModel::onKeyLeafChanged(const Oak::Model::NodeIndex& nIndex)
{
    Oak::Model::Node node = nIndex.node(m_model.rootNode());
    if (node.isNull()) { return; }
    QModelIndex index = createModelIndex(node);
    dataChanged(index, index, QVector<int>() <<  QOakModel::KeyValue);
}

// =============================================================================
// (public)
void QOakModel::onNodeInserteBefore(const Oak::Model::NodeIndex &nIndex)
{
    TRACE("onNodeInserteBefore(%s)\n",nIndex.toString().c_str());
    Oak::Model::Node pNode = nIndex.nodeParent(m_model.rootNode());
    if (pNode.isNull()) { return; }
    QModelIndex pIndex = createModelIndex(pNode);
    int i = nIndex.lastNodeIndex().index();
    beginInsertRows(pIndex, i, i);
}

// =============================================================================
// (public)
void QOakModel::onNodeInserteAfter(const Oak::Model::NodeIndex &nIndex)
{
    TRACE("onNodeInserteAfter(%s)\n",nIndex.toString().c_str());
    endInsertRows();
}

// =============================================================================
// (public)
void QOakModel::onNodeRemoveBefore(const Oak::Model::NodeIndex &nIndex)
{
    TRACE("onNodeRemoveBefore(%s)\n",nIndex.toString().c_str());
}

// =============================================================================
// (public)
void QOakModel::onNodeRemoveAfter(const Oak::Model::NodeIndex &nIndex)
{

}

// =============================================================================
// (public)
void QOakModel::onNodeMoveBefore(const Oak::Model::NodeIndex &sourceNodeIndex, const Oak::Model::NodeIndex &targetNodeIndex)
{

}

// =============================================================================
// (public)
void QOakModel::onNodeMoveAfter(const Oak::Model::NodeIndex &sourceNodeIndex, const Oak::Model::NodeIndex &targetNodeIndex)
{

}

// =============================================================================
// (public)
void QOakModel::onNodeCloneBefore(const Oak::Model::NodeIndex &sourceNodeIndex, const Oak::Model::NodeIndex &targetNodeIndex)
{

}

// =============================================================================
// (public)
void QOakModel::onNodeCloneAfter(const Oak::Model::NodeIndex &sourceNodeIndex, const Oak::Model::NodeIndex &targetNodeIndex)
{

}

// =============================================================================
// (public)
QModelIndex QOakModel::index(int row, int column, const QModelIndex &parent) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Node newParentNode = toNode(parent);
    ASSERT(!newParentNode.isNull());
    return createModelIndex(row, column, newParentNode);
}

// =============================================================================
// (public)
QModelIndex QOakModel::parent(const QModelIndex &child) const
{
    if (m_model.isNull()) { return QModelIndex(); }

    Oak::Model::Node newNode = toParentNode(child);
    if (newNode == m_model.rootNode()) {
        return QModelIndex();
    }
    Oak::Model::Node newParentNode = newNode.parent();
    ASSERT(!newParentNode.isNull());
    int index = newParentNode.childIndex(newNode);
    return createModelIndex(index, child.column(), newParentNode);
}

// =============================================================================
// (public)
QModelIndex QOakModel::sibling(int row, int column, const QModelIndex &idx) const
{
    if (m_model.isNull()) { return QModelIndex(); }
    return createIndex(row, column, idx.internalPointer());
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
    return result;
}

// =============================================================================
// (public)
QModelIndex QOakModel::createModelIndex(const Oak::Model::Node &node) const
{
    Oak::Model::Node pNode = node.parent();
    ASSERT(!pNode.isNull());
    int index = pNode.childIndex(node);
    ASSERT(index >= 0);
    return createModelIndex(index, 0, pNode);
}

// =============================================================================
// (public)
QModelIndex QOakModel::createModelIndex(int row, int column, const Oak::Model::Node& parentNode) const
{
    return createIndex(row, column, parentNode.nodeData().internalPtr());
}

// =============================================================================
// (public)
Oak::Model::Node QOakModel::toParentNode(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return m_model.rootNode();
    }
    return m_model.nodeFromDataPtr(index.internalPointer());
}

// =============================================================================
// (public)
Oak::Model::Node QOakModel::toNode(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return m_model.rootNode();
    }
    Oak::Model::Node pNode = m_model.nodeFromDataPtr(index.internalPointer());
    if (pNode.isNull()) { return Oak::Model::Node(); }
    return pNode.childAt(index.row());
}
