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

#include "NodeData.h"
#include "Leaf.h"
#include "LeafDef.h"

#include <QObject>


namespace Oak::View::QtWidgets {

// =============================================================================
// Class definition
// =============================================================================
class ValueEditorHandler : public QObject
{
    Q_OBJECT
public:
    ValueEditorHandler(QObject *parent, Model::Leaf leaf);
    virtual ~ValueEditorHandler();

    ValueEditorHandler& operator=(const ValueEditorHandler& copy) = delete;

    QWidget* getLabel();
    QWidget* getEditor();

    void setNode(const Model::NodeData& nodeData);

    void updateLabelValue();
    void updateEditorValue();
    void getEditorValue();

    void clearWidgets();
    void clearLabel();
    void clearEditor();

protected:
    virtual void createLabel();
    virtual void createEditor();
    void setValue(const Model::UnionRef& value);

protected slots:
    void onValueEdited();
    void onEditingFinished();

    void onLabelDestroyed();
    void onEditorDestroyed();

signals:
    void valueEditedFinished();

protected:
    Model::Leaf m_leaf;
    QWidget* m_label = nullptr;
    QWidget* m_editor = nullptr;
};

} // namespace Oak::View::QtWidgets

