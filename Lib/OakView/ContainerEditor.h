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

#include <QPushButton>
#include <QGridLayout>
#include <QGroupBox>

#include "ContainerDefinition.h"

namespace Oak {
namespace View {

class ContainerEditorHandler;

// =============================================================================
// Class definition
// =============================================================================
class ContainerEditor : public QWidget
{
    Q_OBJECT
public:
    ContainerEditor(ContainerEditorHandler* eHandler, QWidget *parent = 0);

    void updateEditor();

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;

    void addEntry(const QString& text);
    void removeLastEntry();
    int entryIndex(QObject* obj) const;
    void createContextMenu(const QPoint& globalPos, int index);

protected slots:
    void onItemClicked();
    void onEntryAdded();

signals:
    void entryClicked(int index);
    void entryInserted(int index);
    void entryRemoved(int index);
    void entryAdded();

protected:
    ContainerEditorHandler* m_eHandler;
    QWidget* m_listWidget;
    QPushButton* m_addButton;
    QGridLayout* m_listLayout;
    QGroupBox* m_groupBox;

    int m_count;
};

} // namespace View
} // namespace Oak
