/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ListView.h"

#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QResizeEvent>
#include <QScrollBar>

#include <QDebug>

#include "OakModel.h"
#include "ListViewItem.h"


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
ListView::ListView(QWidget *parent)
    : QFrame(parent)
{
    m_scrollArea = new QScrollArea();
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Update stylesheet
    QString styleSheet = ".QFrame {"
                         "  border: 1px solid rgba(0, 0, 0, 255);"
                         "  border-radius: 4px;"
                         "}"
                         ".QFrame:focus {"
                         "  border: 1px solid rgba(140, 0, 0, 255);"
                         "}"
                         ".QPushButton {"
                         "  background: transparent;"
                         "}"
                         ".QPushButton:hover { "
                         "  border-radius: 3px;"
                         "  border: 1px solid rgba(120, 120, 120, 255);"
                         "}";

    m_scrollArea->setStyleSheet(styleSheet);

    // Horizontal line between scrollable existing items and draggable new items
    auto lineA = new QFrame;
    lineA->setFrameShape(QFrame::HLine);
    lineA->setFrameShadow(QFrame::Sunken);
    lineA->setHidden(true);

    // Available new items that can be dragged into place
    auto dragItems = new QWidget();
    dragItems->setHidden(true);
    m_dragLayout = new QGridLayout();
    m_dragLayout->setMargin(0);
    dragItems->setLayout(m_dragLayout);

    // TEST Buttons - Begin
    auto dragButton1 = new QPushButton("Drag 1");
    auto dragButton2 = new QPushButton("Drag 2");
    auto dragButton3 = new QPushButton("Drag 3");
    auto dragButton4 = new QPushButton("Drag 4");
    auto dragButton5 = new QPushButton("Drag 5");
    auto dragButton6 = new QPushButton("Drag 6");
    m_dragLayout->addWidget(dragButton1, 0, 0);
    m_dragLayout->addWidget(dragButton2, 0, 1);
    m_dragLayout->addWidget(dragButton3, 0, 2);
    m_dragLayout->addWidget(dragButton4, 1, 0);
    m_dragLayout->addWidget(dragButton5, 1, 1);
    m_dragLayout->addWidget(dragButton6, 1, 2);
    // TEST Buttons - End

    auto mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_scrollArea, 1);
    mainLayout->addWidget(lineA);
    mainLayout->addWidget(dragItems, 0);
    setLayout(mainLayout);
}

// =============================================================================
// (public)
ListView::~ListView()
{

}

// =============================================================================
// (public)
void ListView::setOakModel(Model::OakModel *model)
{
    if (m_model == model) { return; }

    if (m_model) {
        // Disconnect the old model
        m_model->notifier_currentItemChanged.remove(this);

        m_model->notifier_itemInserteAfter.remove(this);
        m_model->notifier_itemMoveBefore.remove(this);
        m_model->notifier_itemMoveAfter.remove(this);
        m_model->notifier_itemCloneAfter.remove(this);
        m_model->notifier_itemRemoveBefore.remove(this);
        m_model->notifier_variantLeafChangeAfter.remove(this);
        m_model->notifier_keyLeafChangeAfter.remove(this);
    }

    // Change the model
    m_model = model;

    if (m_model) {
        // connect the new mobel
        m_model->notifier_currentItemChanged.add(this, &ListView::currentItemChanged);

        m_model->notifier_itemInserteAfter.add(this, &ListView::onItemInserteAfter);
        m_model->notifier_itemMoveBefore.add(this, &ListView::onItemMoveBefore);
        m_model->notifier_itemMoveAfter.add(this, &ListView::onItemMoveAfter);
        m_model->notifier_itemCloneAfter.add(this, &ListView::onItemCloneAfter);
        m_model->notifier_itemRemoveBefore.add(this, &ListView::onItemRemoveBefore);
        m_model->notifier_variantLeafChangeAfter.add(this, &ListView::onVariantLeafChangeAfter);
        m_model->notifier_keyLeafChangeAfter.add(this, &ListView::onKeyLeafChangeAfter);
    }
}

// =============================================================================
// (public)
void ListView::setRootItem(const Model::Item &item)
{
    if (m_rootItem != nullptr) {
        // Do nothing if the root item is the same
        if (m_rootItem->item() == item) { return; }

        disconnect(m_rootItem, SIGNAL(heightChanged()), this, SLOT(adjustItemWidth()));
        //Clear existing item before creating a new one
    }
    m_rootItem = new ListViewItem(this, item, 0);
    m_rootItemIndex = Model::ItemIndex::create(item);

    m_scrollArea->setWidget(m_rootItem);
    m_rootItem->setFixedWidth(m_scrollArea->viewport()->width());

    connect(m_rootItem, SIGNAL(heightChanged(int)), this, SLOT(adjustItemWidth()), Qt::QueuedConnection);
}

// =============================================================================
// (public)
void ListView::currentItemChanged()
{
    ListViewItem * currentViewItem = getViewItem(m_model->currentItemIndex());

    if (currentViewItem == m_currentViewItem) { return; }

    if (m_currentViewItem) {
        m_currentViewItem->clearCurrent();
        disconnect(m_currentViewItem, SIGNAL(destroyed()), this, SLOT(onCurrentItemViewDestoyed()));
    }

    m_currentViewItem = currentViewItem;

    if (m_currentViewItem) {
        m_currentViewItem->setCurrent();
        connect(m_currentViewItem, SIGNAL(destroyed()), this, SLOT(onCurrentItemViewDestoyed()));

        // Make current items visible
        ListViewItem * pItem = m_currentViewItem->parent();
        if (pItem != nullptr) {
            while (pItem != m_rootItem) {
                pItem->setExspanded(true);
                pItem = pItem->parent();
            }
        }
    }
}

// =============================================================================
// (public)
int ListView::maxDepth() const
{
    return m_maxDepth;
}

// =============================================================================
// (public)
void ListView::setMaxDepth(int depth)
{
    if (m_maxDepth == depth) { return; }

    m_maxDepth = depth;

    // TODO: Update view and drag items when depth changes
}

// =============================================================================
// (public)
void ListView::resizeEvent(QResizeEvent *event)
{
    if (m_rootItem != nullptr) {
        m_rootItem->setFixedWidth(m_scrollArea->viewport()->width());
    }
    QWidget::resizeEvent(event);
}

// =============================================================================
// (protected)
ListViewItem *ListView::getViewItem(const Model::ItemIndex &itemIndex)
{
    // Return if change is outside root item
    if (!itemIndex.contains(*m_rootItemIndex.get())) { return nullptr; }

    // Get the item index relative to the root item
    const Model::ItemIndex &relItemIndex = itemIndex.childItemIndex(m_rootItemIndex->depth());

    return m_rootItem->child(relItemIndex);
}

// =============================================================================
// (protected)
void ListView::createDragItems() const
{

}

// =============================================================================
// (protected)
void ListView::clearDragItems() const
{

}

// =============================================================================
// (protected)
void ListView::onItemInserteAfter(const Model::ItemIndex &itemIndex)
{
    // Return if change is outside root item
    if (!itemIndex.contains(*m_rootItemIndex.get())) { return; }

    // Get the item index relative to the root item
    const Model::ItemIndex &relItemIndex = itemIndex.childItemIndex(m_rootItemIndex->depth());

    m_rootItem->onItemInserteAfter(relItemIndex);
}

// =============================================================================
// (protected)
void ListView::onItemMoveAfter(const Model::ItemIndex &sourceItemIndex, const Model::ItemIndex &targetItemIndex)
{
    Q_UNUSED(sourceItemIndex);
    onItemInserteAfter(targetItemIndex);
}

// =============================================================================
// (protected)
void ListView::onItemMoveBefore(const Model::ItemIndex &sourceItemIndex, const Model::ItemIndex &targetItemIndex)
{
    Q_UNUSED(targetItemIndex);
    onItemRemoveBefore(sourceItemIndex);
}

// =============================================================================
// (protected)
void ListView::onItemCloneAfter(const Model::ItemIndex &sourceItemIndex, const Model::ItemIndex &targetItemIndex)
{
    Q_UNUSED(sourceItemIndex);
    onItemInserteAfter(targetItemIndex);
}

// =============================================================================
// (protected)
void ListView::onItemRemoveBefore(const Model::ItemIndex &itemIndex)
{
    // Return if change is outside root item
    if (!itemIndex.contains(*m_rootItemIndex.get())) { return; }

    // Get the item index relative to the root item
    const Model::ItemIndex &relItemIndex = itemIndex.childItemIndex(m_rootItemIndex->depth());

    m_rootItem->onItemRemoveBefore(relItemIndex);
}

// =============================================================================
// (protected)
void ListView::onVariantLeafChangeAfter(const Model::ItemIndex &itemIndex)
{
    // Child items can change when the variant definition change
    onItemRemoveBefore(itemIndex);
    onItemInserteAfter(itemIndex);

    currentItemChanged();
}

// =============================================================================
// (protected)
void ListView::onKeyLeafChangeAfter(const Model::ItemIndex &itemIndex)
{
    // Child items can change when the variant definition change
    ListViewItem * viewItem = getViewItem(itemIndex);
    if (viewItem != nullptr) {
        viewItem->updateLabel();
    }
}

// =============================================================================
// (protected slots)
void ListView::adjustItemWidth()
{
    if (m_rootItem != nullptr) {
        m_rootItem->setFixedWidth(m_scrollArea->viewport()->width());
    }
}

// =============================================================================
// (protected slots)
void ListView::onCurrentItemViewDestoyed()
{
    m_currentViewItem = nullptr;
}

} // namespace Oak::View::QtWidgets

