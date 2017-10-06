/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "NodeIndex.h"
#include "TreeDataModel.h"

#include <assert.h>

namespace Oak {
namespace View {

// =============================================================================
// (public)
NodeIndex::NodeIndex(const Model::Item& item)
{
    assert(!item.isNull());

    // Create a list of items up to the rootItem
    std::vector<Model::Item> itemList;
    Model::Item currentItem = item;
    while (!currentItem.isNull() && currentItem != item.model()->rootItem()) {
        itemList.push_back(currentItem);
        currentItem = currentItem.parent();
    }
    if (currentItem == item.model()->rootItem()) {
        itemList.push_back(item.model()->rootItem());
    }

    if (itemList.size() <= 1) { return; }

    // Run though the list backwards and
    // adds the index of all child items to the list of indexes
    auto it = itemList.rbegin();
    currentItem = *it;
    while (++it != itemList.rend()) {
        Model::Item childItem = *it;
        push_back(currentItem.childIndex(childItem));
        currentItem = childItem;
    }
}

// =============================================================================
// (public)
NodeIndex::NodeIndex(QTreeWidgetItem* qItem)
{
    assert(qItem);
    if (!qItem) { return; }
    QTreeWidgetItem * cQItem = qItem;
    QTreeWidgetItem * pQItem = cQItem->parent();
    while (pQItem) {
        this->insert(this->begin(), pQItem->indexOfChild(cQItem));
        cQItem = pQItem;
        pQItem = cQItem->parent();
    }
}

// =============================================================================
// (public)
QTreeWidgetItem* NodeIndex::qItem(QTreeWidgetItem* rootQItem)
{
    assert(rootQItem);
    QTreeWidgetItem * currentQItem = rootQItem;
    for (const int& index: *this) {
        currentQItem = currentQItem->child(index);
        assert(currentQItem);
    }
    return currentQItem;
}

// =============================================================================
// (public)
Model::Item NodeIndex::item(const Model::Item &rootItem)
{
    assert(!rootItem.isNull());
    Model::Item currentItem = rootItem;
    for (const int& index: *this)
    {
        currentItem = currentItem.childAt(index);
        assert(!currentItem.isNull());
    }
    return currentItem;
}

} // namespace View
} // namespace Oak
