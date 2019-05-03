/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "TreeViewData.h"

// =============================================================================
// (public)
TreeViewData::TreeViewData(QObject *parent)
    : QObject(parent)
{
}

// =============================================================================
// (public)
bool TreeViewData::isValid() const
{
    if (m_rootListModel == nullptr) { return false; }
    if (m_model == nullptr) { return false; }
    if (m_rootModelIndex.isValid() && m_rootModelIndex.model() != m_model) { return false; }
    return true;
}

// =============================================================================
// (public)
int TreeViewData::itemHeight() const
{
    return m_itemHeight;
}

// =============================================================================
// (public)
int TreeViewData::spacing() const
{
    return m_spacing;
}

// =============================================================================
// (public)
QModelIndex TreeViewData::rootModelIndex() const
{
    return m_rootModelIndex;
}

// =============================================================================
// (public)
QAbstractItemModel *TreeViewData::model() const
{
    return m_model;
}

// =============================================================================
// (public)
TreeViewListModel *TreeViewData::rootListModel() const
{
    return m_rootListModel;
}

// =============================================================================
// (public)
QModelIndex TreeViewData::currentIndex() const
{
    return m_currentIndex;
}

// =============================================================================
// (public slots)
void TreeViewData::setItemHeight(int itemHeight)
{
    if (m_itemHeight == itemHeight) { return; }

    m_itemHeight = itemHeight;
    if (m_rootListModel != nullptr) {
        m_rootListModel->updateDelegateHeight(true);
    }

    emit itemHeightChanged(m_itemHeight);
}

// =============================================================================
// (public slots)
void TreeViewData::setSpacing(int spacing)
{
    if (m_spacing == spacing) { return; }

    m_spacing = spacing;
    emit spacingChanged(m_spacing);
}

// =============================================================================
// (public slots)
void TreeViewData::setRootModelIndex(QModelIndex rootModelIndex)
{
    if (m_rootModelIndex == rootModelIndex) { return; }

    m_rootModelIndex = rootModelIndex;
    m_rootListModel->setParentModelIndex(m_rootModelIndex);

    emit rootModelIndexChanged(m_rootModelIndex);
}

// =============================================================================
// (public slots)
void TreeViewData::setModel(QAbstractItemModel *model)
{
    if (m_model == model) { return; }

    modelDisconnect();

    onModelAboutToBeReset();
    m_model = model;
    onModelReset();

    modelConnect();

    emit modelChanged(m_model);
}

// =============================================================================
// (public slots)
void TreeViewData::setRootListModel(TreeViewListModel *rootListModel)
{
    if (m_rootListModel == rootListModel) { return; }

    if (m_rootListModel != nullptr) {
        disconnect(this, SIGNAL(currentIndexChanged(const QModelIndex&)), m_rootListModel, SLOT(onCurrentIndexChanged(const QModelIndex&)));
    }

    m_rootListModel = rootListModel;
    m_rootListModel->setTreeViewData(this);

    if (m_rootListModel != nullptr) {
        m_rootListModel->setTreeViewData(this);
        m_rootListModel->setParentModelIndex(m_rootModelIndex);
        connect(this, SIGNAL(currentIndexChanged(const QModelIndex&)), m_rootListModel, SLOT(onCurrentIndexChanged(const QModelIndex&)));
    }

    emit rootListModelChanged(m_rootListModel);
}

// =============================================================================
// (public slots)
void TreeViewData::setCurrentIndex(QModelIndex currentIndex)
{
    if (m_currentIndex == currentIndex) { return; }

    m_currentIndex = currentIndex;
    emit currentIndexChanged(m_currentIndex);
}

// =============================================================================
// (protected)
void TreeViewData::modelConnect()
{
    if (m_model == nullptr) { return; }
    connect(m_model, SIGNAL(modelAboutToBeReset()), this, SLOT(onModelAboutToBeReset()));
    connect(m_model, SIGNAL(modelReset()), this, SLOT(onModelReset()));

    connect(m_model, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)), this, SLOT(onRowsAboutToBeInserted(const QModelIndex &, int, int)));
    connect(m_model, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(onRowsInserted(const QModelIndex &, int, int)));
}

// =============================================================================
// (protected)
void TreeViewData::modelDisconnect()
{
    if (m_model == nullptr) { return; }
    disconnect(m_model, SIGNAL(modelAboutToBeReset()), this, SLOT(onModelAboutToBeReset()));
    disconnect(m_model, SIGNAL(modelReset()), this, SLOT(onModelReset()));

    disconnect(m_model, SIGNAL(rowsAboutToBeInserted(const QModelIndex &, int, int)), this, SLOT(onRowsAboutToBeInserted(const QModelIndex &, int, int)));
    disconnect(m_model, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(onRowsInserted(const QModelIndex &, int, int)));
}

// =============================================================================
// (protected)
TreeViewListModel *TreeViewData::modelFromIndex(const QModelIndex &index)
{
    if (m_rootListModel->isNull()) { return nullptr; }

    // Make sure the index is of column 0 (To avoid false negative)
    QModelIndex compareIndex = index.sibling(index.row(), 0);

    // Define a lamda function for recursive search fo a model that match
    std::function<TreeViewListModel *(TreeViewListModel *)> findModel;
    findModel = [compareIndex, &findModel](TreeViewListModel *model)->TreeViewListModel* {
        if (model->parentModelIndex() == compareIndex) { return model; }
        TreeViewListModel *modelMatch;
        for (TreeViewListModel *childModel: model->m_childModelMap.values())
        {
            modelMatch = findModel(childModel);
            if (modelMatch != nullptr) { return modelMatch; }
        }
        return nullptr;
    };

    // Execute the search
    return findModel(m_rootListModel);
}

// =============================================================================
// (protected slots)
void TreeViewData::onModelAboutToBeReset()
{
    if (m_rootListModel != nullptr) {
        m_rootListModel->onModelAboutToBeReset();
    }
}

// =============================================================================
// (protected slots)
void TreeViewData::onModelReset()
{
    if (m_rootListModel != nullptr) {
        m_rootListModel->onModelReset();
    }
}

// =============================================================================
// (protected slots)
void TreeViewData::onRowsAboutToBeInserted(const QModelIndex &parent, int start, int end)
{
    TreeViewListModel *modelMatch = modelFromIndex(parent);
    if (modelMatch == nullptr) { return; }
    modelMatch->beginInsertRows(parent, start, end);
}

//// =============================================================================
//// (protected slots)
//void TreeViewData::onRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
//{

//}

//// =============================================================================
//// (protected slots)
//void TreeViewData::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
//{

//}

// =============================================================================
// (protected slots)
void TreeViewData::onRowsInserted(const QModelIndex &parent, int first, int last)
{
    TreeViewListModel *modelMatch = modelFromIndex(parent);
    if (modelMatch) {
        // Update existing model
        modelMatch->endInsertRows();
        modelMatch->layoutChanged();
        modelMatch->updateDelegateHeight();
    } else {
        modelMatch = modelFromIndex(parent.parent());
        if (modelMatch) {
            if (m_model->rowCount(parent) == last - first + 1) {
                // Item of parent model now has children
                QModelIndex index = modelMatch->index(parent.row(), parent.column(), QModelIndex());
                modelMatch->dataChanged(index, index,  QVector<int>() << TreeViewListModel::HasChildren);
            }
        }
    }

}

//// =============================================================================
//// (protected slots)
//void TreeViewData::onRowsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
//{

//}

//// =============================================================================
//// (protected slots)
//void TreeViewData::onRowsRemoved(const QModelIndex &parent, int first, int last)
//{

//}
