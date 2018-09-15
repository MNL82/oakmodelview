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
    m_actionInsert = new QAction(QPixmap(":/OakView/Resources/add_32.png"), "Insert item before");
    m_actionInsert->setEnabled(false);
    connect(m_actionInsert, SIGNAL(triggered()), this, SLOT(onActionInsert()));
    addAction(m_actionInsert);

    m_actionDelete = new QAction(QPixmap(":/OakView/Resources/delete_32.png"), "Delete item");
    m_actionDelete->setEnabled(false);
    m_actionDelete->setShortcut(QKeySequence::Delete);
    connect(m_actionDelete, SIGNAL(triggered()), this, SLOT(onActionDelete()));
    addAction(m_actionDelete);

    addSeparator();

    m_actionCut = new QAction(QPixmap(":/OakView/Resources/cut_32.png"), "Cut item");
    m_actionCut->setEnabled(false);
    m_actionCut->setShortcut(QKeySequence::Cut);
    connect(m_actionCut, SIGNAL(triggered()), this, SLOT(onActionCut()));
    addAction(m_actionCut);

    m_actionCopy = new QAction(QPixmap(":/OakView/Resources/copy_32.png"), "Copy item");
    m_actionCopy->setEnabled(false);
    m_actionCopy->setShortcut(QKeySequence::Copy);
    connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(onActionCopy()));
    addAction(m_actionCopy);

    m_actionPaste = new QAction(QPixmap(":/OakView/Resources/paste_32.png"), "Paste item before");
    m_actionPaste->setEnabled(false);
    m_actionPaste->setShortcut(QKeySequence::Paste);
    connect(m_actionPaste, SIGNAL(triggered()), this, SLOT(onActionPaste()));
    addAction(m_actionPaste);

    addSeparator();

    m_actionUp = new QAction(QPixmap(":/OakView/Resources/up_32.png"), "Move item up");
    m_actionUp->setEnabled(false);
    connect(m_actionUp, SIGNAL(triggered()), this, SLOT(onActionUp()));
    addAction(m_actionUp);

    m_actionDown = new QAction(QPixmap(":/OakView/Resources/down_32.png"), "Move item down");
    m_actionDown->setEnabled(false);
    connect(m_actionDown, SIGNAL(triggered()), this, SLOT(onActionDown()));
    addAction(m_actionDown);
}

// =============================================================================
// (public)
void ActionToolBar::setModel(Model::OakModel *model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentItemChanged.remove(this);
        m_model->notifier_itemBeforeRemoving.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentItemChanged.add(this, &ActionToolBar::currentItemChanged);
        m_model->notifier_itemBeforeRemoving.add(this, &ActionToolBar::itemBeforeRemoving);
    }
}

// =============================================================================
// (protected)
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
        m_actionInsert->setEnabled(pItem.canInsertChild(item.def()->name(), index));
        m_actionDelete->setEnabled(pItem.canRemoveChild(index));
        int index2 = index-1;
        m_actionUp->setEnabled(index2 >= 0 && pItem.canMoveChild(index2, item));
        index2 = index+1;
        m_actionDown->setEnabled(pItem.canMoveChild(index2, item));
        m_actionCut->setEnabled(true);
        m_actionCopy->setEnabled(true);

        if (!m_copyItem.isNull()) {
            index2 = index;
            if (pItem.canCloneChild(index2, m_copyItem)) {
                m_actionPaste->setEnabled(true);
            } else {
                index2 = -1;
                m_actionPaste->setEnabled(item.canCloneChild(index2, m_copyItem));
            }
        } else if (!m_cutItem.isNull()) {
            index2 = index;
            if (pItem.canMoveChild(index2, m_cutItem)) {
                m_actionPaste->setEnabled(true);
            } else {
                index2 = -1;
                m_actionPaste->setEnabled(item.canMoveChild(index2, m_cutItem));
            }
        } else {
            m_actionPaste->setEnabled(false);
        }
    } else{
        disableAllActions();
    }

}

// =============================================================================
// (protected)
void ActionToolBar::itemBeforeRemoving(const Model::Item &item)
{
    Model::Item cItem = m_cutItem;
    while (!cItem.isNull()) {
        if (cItem == item) {
            m_cutItem = Model::Item();
            m_actionPaste->setEnabled(false);
            return;
        }
        cItem = cItem.parent();
    }

    cItem = m_copyItem;
    while (!cItem.isNull()) {
        if (cItem == item) {
            m_copyItem = Model::Item();
            m_actionPaste->setEnabled(false);
            return;
        }
        cItem = cItem.parent();
    }
}

// =============================================================================
// (protected)
void ActionToolBar::disableAllActions()
{
    m_actionInsert->setEnabled(false);
    m_actionDelete->setEnabled(false);
    m_actionUp->setEnabled(false);
    m_actionDown->setEnabled(false);
    m_actionCut->setEnabled(false);
    m_actionCopy->setEnabled(false);
    m_actionPaste->setEnabled(false);
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionInsert()
{
    Model::Item item = m_model->currentItem();
    Model::Item pItem = item.parent();
    int index = pItem.childIndex(item);
    if (pItem.canInsertChild(item.def()->name(), index)) {
        Model::Item newItem = pItem.insertChild(item.def()->name(), index);
        m_model->setCurrentItem(newItem);
        setFocus(); // Return focus to the action tool bar from whatever view inserted the item
    }
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
        setFocus(); // Return focus to the action tool bar from whatever view deleted the item
    }
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionUp()
{
    Model::Item item = m_model->currentItem();
    Model::Item pItem = item.parent();
    int index = pItem.childIndex(item)-1;
    pItem.moveChild(index, item);
    setFocus(); // Return focus to the action tool bar from whatever view moved the item
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionDown()
{
    Model::Item item = m_model->currentItem();
    Model::Item pItem = item.parent();
    int index = pItem.childIndex(item)+1;
    pItem.moveChild(index, item);
    setFocus(); // Return focus to the action tool bar from whatever view moved the item
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionCut()
{
    m_cutItem = m_model->currentItem();
    m_copyItem = Model::Item();
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionCopy()
{
    m_cutItem = Model::Item();
    m_copyItem = m_model->currentItem();
}

// =============================================================================
// (protected slots)
void ActionToolBar::onActionPaste()
{
    if (!m_copyItem.isNull()) {
        Model::Item item = m_model->currentItem();
        Model::Item pItem = item.parent();
        int index = pItem.childIndex(item);
        if (pItem.canCloneChild(index, m_copyItem)) {
            Model::Item newItem = pItem.cloneChild(index, m_copyItem);
            m_model->setCurrentItem(newItem);
        } else {
            index = -1;
            Model::Item newItem = item.cloneChild(index, m_copyItem);
            m_model->setCurrentItem(newItem);
        }
        setFocus(); // Return focus to the action tool bar from whatever view copied the item
    } else if (!m_cutItem.isNull()) {
        Model::Item item = m_model->currentItem();
        Model::Item pItem = item.parent();
        int index = pItem.childIndex(item);
        if (pItem.canMoveChild(index, m_cutItem)) {
            Model::Item newItem = pItem.moveChild(index, m_cutItem);
            m_model->setCurrentItem(newItem);
        } else {
            index = -1;
            Model::Item newItem = item.moveChild(index, m_cutItem);
            m_model->setCurrentItem(newItem);
        }
        m_cutItem = Model::Item();
        m_actionPaste->setEnabled(false);
        setFocus(); // Return focus to the action tool bar from whatever view moved the item
    }
}

} // namespace View
} // namespace Oak
