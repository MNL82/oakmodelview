/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "TreeViewListModel.h"
#include "TreeViewData.h"

#include "../ServiceFunctions/Trace.h"

// =============================================================================
// (public)
TreeViewListModel::TreeViewListModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

// =============================================================================
// (public)
TreeViewListModel::~TreeViewListModel()
{

}

// =============================================================================
// (public)
bool TreeViewListModel::isNull() const
{
    if (refModel() == nullptr) { return true; }
    if (m_parentModelIndex.isValid() && m_parentModelIndex.model() != refModel()) { return true; }
    return false;
}

// =============================================================================
// (public)
bool TreeViewListModel::shown() const
{
    return m_shown;
}

// =============================================================================
// (public)
int TreeViewListModel::delegateHeight() const
{
    return m_delegateHeight;
}

// =============================================================================
// (public)
QModelIndex TreeViewListModel::parentModelIndex() const
{
    return m_parentModelIndex;
}

// =============================================================================
// (public)
TreeViewData *TreeViewListModel::treeViewData() const
{
    return m_data;
}

// =============================================================================
// (public)
void TreeViewListModel::setShown(bool shown)
{
    if (m_shown == shown) { return; }
    m_shown = shown;
    updateDelegateHeight();
    emit shownChanged(m_shown);
}

// =============================================================================
// (public slots)
void TreeViewListModel::setDelegateHeight(int delegateHeight)
{
    if (m_delegateHeight == delegateHeight) { return; }
    m_delegateHeight = delegateHeight;
    //TRACE("Height: %i\n", delegateHeight);
    emit delegateHeightChanged(m_delegateHeight);
}

// =============================================================================
// (public slots)
void TreeViewListModel::setParentModelIndex(const QModelIndex &parentModelIndex)
{
    if (m_parentModelIndex == parentModelIndex) { return; }

    onModelAboutToBeReset();
    m_parentModelIndex = parentModelIndex;
    onModelReset();

    emit parentModelIndexChanged(m_parentModelIndex);
}

// =============================================================================
// (public slots)
void TreeViewListModel::setTreeViewData(TreeViewData *treeViewData)
{
    if (m_data == treeViewData) { return; }

    m_data = treeViewData;
    emit treeViewDataChanged(m_data);
}

// =============================================================================
// (public slots)
void TreeViewListModel::addChildModel(int row, TreeViewListModel *model)
{
    m_childModelMap[row] = model;
    connect(model, SIGNAL(delegateHeightChanged(int)), this, SLOT(updateDelegateHeight()));
    connect(m_data, SIGNAL(currentIndexChanged(const QModelIndex&)), model, SLOT(onCurrentIndexChanged(const QModelIndex&)));
    model->updateDelegateHeight();
}

// =============================================================================
// (protected)
int TreeViewListModel::modelHeight() const
{
    if (isNull() || !m_shown) { return false; }
    int height = rowCount(m_parentModelIndex) * m_delegateHeight;
    for (auto childModel: m_childModelMap.values()) {
        height += childModel->modelHeight();
    }
    return height;
}

// =============================================================================
// (protected)
TreeViewListModel *TreeViewListModel::childModel(int row) const
{
    if (m_childModelMap.contains(row)) {
        return m_childModelMap[row];
    }
    return nullptr;
}

// =============================================================================
// (protected)
QAbstractItemModel *TreeViewListModel::refModel() const
{
    return m_data ? m_data->model() : nullptr;
}

// =============================================================================
// (protected slots)
void TreeViewListModel::onModelAboutToBeReset()
{
    beginResetModel();
}

// =============================================================================
// (protected slots)
void TreeViewListModel::onModelReset()
{
    m_currentIndexRow = -1;
    for (TreeViewListModel *childModel: m_childModelMap.values())
    {
        childModel->QObject::setParent(nullptr);
        delete childModel;
    }
    m_childModelMap.clear();

    resetInternalData();
    endResetModel();
}

// =============================================================================
// (protected slots)
void TreeViewListModel::updateDelegateHeight(bool recursive)
{
    //TRACE("updateDelegateHeight(%s)\n", refModel()->data(m_parentModelIndex).toString().toStdString().c_str());
    if (recursive) {
        bool value = blockSignals(true);
        for(TreeViewListModel *cModel: m_childModelMap.values())
        {
            cModel->updateDelegateHeight(true);
        }
        blockSignals(value);
    }

    int height = (m_data->rootListModel() == this) ? 0 : m_data->itemHeight();
    if (m_shown) {
        int count = rowCount(m_parentModelIndex);
        for (int i = 0; i < count; i++)
        {
            TreeViewListModel * cModel = childModel(i);
            height += cModel ? cModel->delegateHeight() : m_data->itemHeight();
        }
        height += count * m_data->spacing();
    }

    setDelegateHeight(height);
}

// =============================================================================
// (public)
void TreeViewListModel::onCurrentIndexChanged(const QModelIndex &currentIndex)
{
    if (m_currentIndexRow >= 0) {
        QModelIndex i = index(m_currentIndexRow, 0, QModelIndex());
        if (i.isValid()) {
            //TRACE("Clear currentIndex %s:%i\n", data(i).toString().toStdString().c_str(), i.row());
            dataChanged(i, i, QVector<int>() << IsCurrent);
        } else {
            TRACE("Invalid row: %1", m_currentIndexRow);
        }
        m_currentIndexRow = -1;
    }
    QModelIndex parent = currentIndex.parent();
    if (parent != m_parentModelIndex) { return; }

    QModelIndex i = index(currentIndex.row(), 0, QModelIndex());
    if (i.isValid()) {
        m_currentIndexRow = i.row();
        //TRACE("Set currentIndex %s:%i\n", data(i).toString().toStdString().c_str(), i.row());
        dataChanged(i, i, QVector<int>() << IsCurrent);
    } else {
        TRACE("Invalid row: %1", currentIndex.row());
    }
}

// =============================================================================
// (public)
QModelIndex TreeViewListModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (isNull()) { return QModelIndex(); }
    return createIndex(row, column);
}

// =============================================================================
// (public)
QModelIndex TreeViewListModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

// =============================================================================
// (public)
int TreeViewListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (isNull()) { return 0; }
    int count = refModel()->rowCount(m_parentModelIndex);
    //TRACE("Count: %i\n", count);
    return count;
}

// =============================================================================
// (public)
int TreeViewListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if (isNull()) { return 0; }
    return refModel()->columnCount(m_parentModelIndex);
}

// =============================================================================
// (public)
QVariant TreeViewListModel::data(const QModelIndex &index, int role) const
{
    if (isNull()) { return QVariant(); }

    switch (role) {
    case HasChildren: {
        QModelIndex refIndex = refModel()->index(index.row(), index.column(), m_parentModelIndex);
        return refModel()->hasChildren(refIndex);
    }
    case Expanded: {
        bool expanded = m_childModelMap.contains(index.row()) && m_childModelMap[index.row()]->shown();
        TRACE("Expanded: %s\n", expanded ? "true" : "false");
        return expanded;
    }
    case IsCurrent: {
        bool isCurrent = m_data->currentIndex() == refModel()->index(index.row(), index.column(), m_parentModelIndex);
        //TRACE("CurrentIndex %s:%i value:%i\n", data(index).toString().toStdString().c_str(), index.row(), isCurrent ? 1 : 0);
        return isCurrent;
    }
    case RefModelIndex:
        return refModel()->index(index.row(), index.column(), m_parentModelIndex);
    }

    QModelIndex refIndex = refModel()->index(index.row(), index.column(), m_parentModelIndex);
    return refModel()->data(refIndex, role);
}

// =============================================================================
// (public)
bool TreeViewListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (isNull()) { return false; }

    switch (role) {
    case HasChildren:
        return false;
    case Expanded: {
        if (data(index, role) == value) { return true; }
        bool expanded = value.toBool();
        ASSERT(m_childModelMap.contains(index.row()));
        m_childModelMap[index.row()]->setShown(expanded);
        TRACE("SetExpanded: %s\n", expanded ? "true" : "false");
        dataChanged(index, index, QVector<int>() <<  Expanded);
        return true;
    }
    case IsCurrent: {
        bool isCurrent = value.toBool();
        if (isCurrent) {
            m_data->setCurrentIndex(refModel()->index(index.row(), index.column(), m_parentModelIndex));
        } else {
            m_data->setCurrentIndex(QModelIndex());
        }
        return true;
    }
    case RefModelIndex:
        return false;
    }

    QModelIndex refIndex = refModel()->index(index.row(), index.column(), m_parentModelIndex);
    return refModel()->setData(refIndex, value, role);
}

// =============================================================================
// (public)
QHash<int, QByteArray> TreeViewListModel::roleNames() const
{
    if (isNull()) { return QHash<int, QByteArray>(); }
    QHash<int, QByteArray> result = refModel()->roleNames();
    result.insert(HasChildren, QByteArrayLiteral("hasChildren"));
    result.insert(Expanded, QByteArrayLiteral("expanded"));
    result.insert(IsCurrent, QByteArrayLiteral("isCurrent"));
    result.insert(RefModelIndex, QByteArrayLiteral("refModelIndex"));
    return result;
}
