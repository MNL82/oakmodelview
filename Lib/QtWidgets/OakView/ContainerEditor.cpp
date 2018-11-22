/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ContainerEditor.h"

#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QContextMenuEvent>
#include <QMenu>

#include "NodeDef.h"
#include "ContainerEditorHandler.h"

#include "../ServiceFunctions/Assert.h"


namespace Oak::View::QtWidgets {

// =============================================================================
// (public)
ContainerEditor::ContainerEditor(ContainerEditorHandler* eHandler, QWidget *parent)
    : QWidget(parent), m_eHandler(eHandler)
{
    m_count = 0;

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin(0);

    m_groupBox = new QGroupBox();

    QVBoxLayout* groupLayout = new QVBoxLayout();

    m_listWidget = new QWidget();

    m_listLayout = new QGridLayout();
    m_listLayout->setMargin(0);
    m_listLayout->setVerticalSpacing(4);

    addEntry("empty");

    m_listWidget->setLayout(m_listLayout);
    groupLayout->addWidget(m_listWidget);

    m_addButton = new QPushButton("Add New");
    connect(m_addButton, SIGNAL(released()), this, SLOT(onEntryAdded()));
    groupLayout->addWidget(m_addButton);

    m_groupBox->setLayout(groupLayout);

    layout->addWidget(m_groupBox);

    setLayout(layout);

    updateEditor();
}

// =============================================================================
// (public)
void ContainerEditor::updateEditor()
{
    QString displayName = m_eHandler->listDisplayName();
    QString labelTitle = QString("%1 List:").arg(displayName);
    m_groupBox->setTitle(labelTitle);

    QStringList entryList = m_eHandler->itemIdList();
    int eCount = entryList.count();

    m_addButton->setHidden(!m_eHandler->canInsert(eCount));

    if (eCount == 0) {
        // A layout can not be empty.
        // So the widget containing the buttons are hidden instead.
        m_listWidget->hide();
        return;
    }

    // Update entry text of existing entries
    for (int i = 0; i < qMin(eCount, m_count); i++)
    {
        QLabel* label = dynamic_cast<QLabel*>(m_listLayout->itemAtPosition(i, 0)->widget());
        ASSERT(label);
        label->setText(entryList.at(i));
    }

    // Add missing buttons
    while (entryList.count() > m_count) {
        addEntry(entryList.at(m_count));

    }

    // Remove extra buttons
    while (entryList.count() < m_count) {
        removeLastEntry();
    }

    m_listWidget->show();
}

// =============================================================================
// (protected)
bool ContainerEditor::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::ContextMenu) {
        int index = entryIndex(obj);
        if (index >= 0) {
            QContextMenuEvent* cEvent = dynamic_cast<QContextMenuEvent*>(event);
            createContextMenu(cEvent->globalPos(), index);
            return true;
        }
    }
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent * mEvent = dynamic_cast<QMouseEvent*>(event);
        if (mEvent->button() == Qt::LeftButton) {
            int index = entryIndex(obj);
            if (index >= 0) {
                emit entryClicked(index);
                return true;
            }
        }
    }
    return false;
}

// =============================================================================
// (protected)
void ContainerEditor::addEntry(const QString& text)
{
    QLabel* label = new QLabel(text);
    //button->setStyleSheet("QPushButton { border:1px solid #444444 };");
    label->setAlignment(Qt::AlignHCenter);
    label->setFrameShape(QFrame::StyledPanel);

    m_listLayout->addWidget(label, m_count++, 0, 1, 1);
    label->installEventFilter(this);
}

// =============================================================================
// (protected)
void ContainerEditor::removeLastEntry()
{
    QLayoutItem* layoutItem = m_listLayout->itemAtPosition(--m_count, 0);
    m_listLayout->removeItem(layoutItem);
    QWidget * widget = layoutItem->widget();
    delete layoutItem;
    delete widget;
}

// =============================================================================
// (protected)
int ContainerEditor::entryIndex(QObject* obj) const
{
    for (int i = 0; i < m_count; i++)
    {
        if (obj == m_listLayout->itemAtPosition(i, 0)->widget()) {
            return i;
        }
    }
    return -1;
}

// =============================================================================
// (protected)
void ContainerEditor::createContextMenu(const QPoint& globalPos, int index)
{
    bool haveAction = false;
    QMenu menu(this);
    if (m_eHandler->canInsert(index)) {
        QAction* action = new QAction("Insert Before", this);
        connect(action, &QAction::triggered, [=] { emit entryInserted(index); });
        menu.addAction(action);
        haveAction = true;
    }
    if (m_eHandler->canInsert(index+1)) {
        QAction* action = new QAction("Insert After", this);
        connect(action, &QAction::triggered, [=] { emit entryInserted(index+1); });
        menu.addAction(action);
        haveAction = true;
    }
    if (m_eHandler->canRemove(index)) {
        QAction* action = new QAction("Remove", this);
        connect(action, &QAction::triggered, [=] { emit entryRemoved(index); });
        menu.addAction(action);
        haveAction = true;
    }
    if (haveAction) {
        menu.exec(globalPos);
    }
}

// =============================================================================
// (protected slots)
void ContainerEditor::onEntryAdded()
{
    emit entryInserted(m_eHandler->itemCount());
}

} // namespace Oak::View::QtWidgets

