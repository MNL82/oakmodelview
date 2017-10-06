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

#include "Node.h"
#include "ItemValue.h"
#include "ValueDefinition.h"

#include <QObject>

namespace Oak {
namespace View {

// =============================================================================
// Class definition
// =============================================================================
class ValueEditorHandler : public QObject
{
    Q_OBJECT
public:
    ValueEditorHandler(QObject *parent, Model::ItemValue itemValue);
    virtual ~ValueEditorHandler();

    ValueEditorHandler& operator=(const ValueEditorHandler& copy) = delete;

    QWidget* getLabel();
    QWidget* getEditor();

    void setNode(const Model::Node& node);

    void updateLabelValue();
    void updateEditorValue();
    void getEditorValue();

    void clearWidgets();
    void clearLabel();
    void clearEditor();

protected:
    virtual void createLabel();
    virtual void createEditor();
    void setItemValue(const Model::VariantCRef& value);

protected slots:
    void onValueEdited();
    void onEditingFinished();

    void onLabelDestroyed();
    void onEditorDestroyed();

signals:
    void valueEditedFinished();

protected:
    Model::ItemValue m_itemValue;
    QWidget* m_label = nullptr;
    QWidget* m_editor = nullptr;
};

} // namespace View
} // namespace Oak
