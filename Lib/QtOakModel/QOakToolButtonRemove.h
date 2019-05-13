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

#include "QOakToolButton.h"
#include "NodeIndex.h"

// =============================================================================
// Class definition
// =============================================================================
class QOakToolButtonRemove : public QOakToolButton
{
    Q_OBJECT
public:
    QOakToolButtonRemove(QObject *parent = nullptr);

    virtual int count() const override;

    virtual QVariant data(int index, int role) const override;

    virtual void sourceModelConnect() override;
    virtual void sourceModelDisconnect() override;

    virtual void trigger(int index) override;

protected:
    void onNodeInserteAfter(const Oak::Model::NodeIndex& nodeIndex);
    void onNodeRemoveAfter(const Oak::Model::NodeIndex& nodeIndex);
    void onNodeMoveAfter(const Oak::Model::NodeIndex& sourceNodeIndex, const Oak::Model::NodeIndex& targetNodeIndex);
    void onNodeCloneAfter(const Oak::Model::NodeIndex& sourceNodeIndex, const Oak::Model::NodeIndex& targetNodeIndex);

};

