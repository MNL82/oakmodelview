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

#include "ContainerEditor.h"
#include "Item.h"

#include <QObject>

#include <memory>

namespace Oak {
namespace View {

// =============================================================================
// Class definition
// =============================================================================
class ContainerEditorHandler : public QObject
{
    Q_OBJECT
public:
    ContainerEditorHandler(Model::Item item, Model::VariantCRef name, QObject *parent = 0);

    QString listDisplayName() const;
    int entryCount() const;
    QStringList entryList() const;

    bool canInsert(int index) const;
    bool canRemove(int index) const;

    QWidget* getEditor();
    bool setNode(const Model::Node& node);
    void updateEditor();
    void clearEditor();

protected:
    void createEditor();

protected slots:
    void onEditorDestroyed();
    void onEntryClicked(int index);
    void onEntryInsert(int index);
    void onEntryRemove(int index);

protected:

    Model::Item m_item;
    Model::VariantCRef m_name;
    QWidget* m_editor = nullptr;
};

} // namespace View
} // namespace Oak
