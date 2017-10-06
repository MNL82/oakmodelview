/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#pragma once

#include <vector>
#include <string>

#include <QTreeWidgetItem>

#include "Item.h"

namespace Oak {
namespace View {

class NodeIndex : public std::vector<int>
{
public:
    NodeIndex(const Model::Item& item);
    NodeIndex(QTreeWidgetItem* qItem);

    QTreeWidgetItem* qItem(QTreeWidgetItem* rootQItem);
    Model::Item item(const Model::Item& rootItem);
};

} // namespace View
} // namespace Oak
