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
#include "Node.h"

#include <QObject>

#include <memory>


namespace Oak::View::QtWidgets {

// =============================================================================
// Class definition
// =============================================================================
class ContainerEditorHandler : public QObject
{
    Q_OBJECT
public:
    ContainerEditorHandler(const Model::Node &node, const std::string &name, QObject *parent = nullptr);

    QString listDisplayName() const;
    int nodeCount() const;
    QStringList nodeIdList() const;

    bool canInsert(int index) const;
    bool canRemove(int index) const;

    QWidget* getEditor();
    bool setNode(const Model::NodeData& nodeData);
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

    Model::Node m_node;
    std::string m_name;
    QWidget* m_editor = nullptr;
};

} // namespace Oak::View::QtWidgets

