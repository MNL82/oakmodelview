/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ActionToolBar.h"

#include <QMessageBox>
#include <QAction>
#include <QIcon>
#include <QImage>

namespace Oak {
namespace View {

// =============================================================================
// (public)
ActionToolBar::ActionToolBar(QWidget *parent)
    : QToolBar("Model Actions", parent)
{
    m_actionRemove = new QAction(QPixmap(":/OakView/Resources/delete_32.png"), "Delete");
    m_actionRemove->setEnabled(false);
    m_actionRemove->setShortcut(QKeySequence::Delete);
    connect(m_actionRemove, SIGNAL(triggered()), this, SLOT(onActionDelete()));

    addAction(m_actionRemove);
}

// =============================================================================
// (public)
void ActionToolBar::setModel(Model::OakModel *model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentItemChanged.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentItemChanged.add(this, &ActionToolBar::currentItemChanged);
    }
}

// =============================================================================
// (public)
void ActionToolBar::currentItemChanged()
{
    Model::Item item = m_model->currentItem();
    if (item.isNull()) {
        disableAllActions();
        return;
    }

    Model::Item pItem = item.parent();
    if (!pItem.isNodeNull()) {
        int index = pItem.childIndex(item);
        m_actionRemove->setEnabled(pItem.canRemoveChild(index));
    } else{
        disableAllActions();
    }

}

// =============================================================================
// (protected)
void ActionToolBar::disableAllActions()
{
    m_actionRemove->setEnabled(false);
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionDelete()
{
    if (QMessageBox::question(this, "Delete", "Are you sure you want to delete current item?") == QMessageBox::Yes) {
        Model::Item item = m_model->currentItem();
        Model::Item pItem = item.parent();
        int index = pItem.childIndex(item);
        pItem.removeChild(index);
    }
}

} // namespace View
} // namespace Oak
