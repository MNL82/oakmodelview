/**
 * oakmodelview - version 0.1.0
 * --------------------------------------------------------
 * Copyright (C) 2017, by Mikkel Nøhr Løvgreen (mikkel@oakmodelview.com)
 * Report bugs and download new versions at http://oakmodelview.com/
 *
 * This library is distributed under the MIT License.
 * See accompanying file LICENSE in the root folder.
 */

#include "ValueEditorHandler.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QIntValidator>
#include <QHBoxLayout>
#include <QCheckBox>

#include "ValueOptions.h"
#include "ServiceFunctions.h"

namespace Oak {
namespace View {

// =============================================================================
// (public)
ValueEditorHandler::ValueEditorHandler(QObject* parent, Model::ItemValue itemValue)
    : QObject(parent), m_itemValue(itemValue)
{
    assert(!itemValue.isDefNull());
}

// =============================================================================
// (public)
ValueEditorHandler::~ValueEditorHandler()
{
    clearWidgets();
}

// =============================================================================
// (public)
QWidget* ValueEditorHandler::getLabel()
{
    createLabel();
    return m_label;
}

// =============================================================================
// (public)
QWidget* ValueEditorHandler::getEditor()
{
    createEditor();
    return m_editor;
}

// =============================================================================
// (public)
void ValueEditorHandler::setNode(const Model::Node& node)
{
    m_itemValue = Model::ItemValue(m_itemValue.valueDef(), node, m_itemValue.item());

    updateLabelValue();
    updateEditorValue();
}

// =============================================================================
// (public)
void ValueEditorHandler::updateLabelValue()
{
    // Default behaivior is that the label is not changed when the node is changed
}

// =============================================================================
// (public)
void ValueEditorHandler::updateEditorValue()
{
    if (!m_editor) { return; }

    QCheckBox* checkBox = m_editor->findChild<QCheckBox*>("value");
    if (checkBox) {
        SignalBlocker blocker(checkBox);
        if (m_itemValue.isNull()) {
            checkBox->setChecked(false);
            return;
        }
        bool value;
        m_itemValue.getValue(value);
        checkBox->setChecked(value);
        return;
    }

    QLineEdit* lineEdit = m_editor->findChild<QLineEdit*>("value");
    if (lineEdit) {
        SignalBlocker blocker(lineEdit);
        if (m_itemValue.isNull()) {
            lineEdit->clear();
            return;
        }
        std::string str;
        m_itemValue.getValue(str);
        lineEdit->setText(QString::fromStdString(str));
        return;
    }

    QComboBox* comboBox = m_editor->findChild<QComboBox*>("value");
    if (comboBox) {
        SignalBlocker blocker(comboBox);
        if (m_itemValue.isNull()) {
            comboBox->clear();
            return;
        }

        comboBox->clear();
        std::vector<std::string> optionList;
        m_itemValue.getOptions(optionList);

        for (const std::string& option: optionList)
        {
            comboBox->addItem(QString::fromStdString(option));
        }
        if (m_itemValue.settings().optionsOnly()) {
            comboBox->setInsertPolicy(QComboBox::NoInsert);
        } else {
            comboBox->setInsertPolicy(QComboBox::InsertAtBottom);
        }

        std::string str;
        m_itemValue.getValue(str);
        comboBox->setCurrentText(QString::fromStdString(str));
        return;
    }
}

// =============================================================================
// (public)
void ValueEditorHandler::getEditorValue()
{
}

// =============================================================================
// (public)
void ValueEditorHandler::clearWidgets()
{
    clearLabel();
    clearEditor();
}

// =============================================================================
// (public)
void ValueEditorHandler::clearLabel()
{
    if (m_label) {
        if (m_label->parent() == nullptr) {
            // Delete the label if it has no parent
            delete m_label;
        } else {
            // Disconnect any signal from the m_label to this object
            disconnect(m_label, nullptr, this,  nullptr);
        }
        m_label = nullptr;
    }
}

// =============================================================================
// (public)
void ValueEditorHandler::clearEditor()
{
    if (m_editor) {
        if (m_editor->parent() == nullptr) {
            // Delete the editor if it has no parent
            delete m_editor;
        } else {
            // Disconnect any signal from the m_editor to this object
            disconnect(m_editor, nullptr, this, nullptr);
        }
        m_editor = nullptr;
    }
}

// =============================================================================
// (protected)
void ValueEditorHandler::createLabel()
{
    if (m_label != nullptr) { return; }

    m_label = new QLabel(QString::fromStdString(m_itemValue.displayName()));
    m_label->setToolTip(QString::fromStdString(m_itemValue.tooltip()));
    connect(m_label, SIGNAL(destroyed(QObject*)), this, SLOT(onLabelDestroyed()));

    updateLabelValue();
}

// =============================================================================
// (protected)
void ValueEditorHandler::createEditor()
{
    if (m_editor != nullptr) { return; }

    QValidator * validator = nullptr;

    auto valueDef = m_itemValue.valueDef();
    if (valueDef->valueType() == Model::UnionType::Integer) {
        validator = new QIntValidator();
    } else if (valueDef->valueType() == Model::UnionType::Double) {
        validator = new QDoubleValidator();
    }

    m_editor = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(m_editor);
    layout->setMargin(0);

    if (valueDef->valueType() == Model::UnionType::Bool) {
        QCheckBox* checkBox = new QCheckBox();
        checkBox->setObjectName("value");
        connect(checkBox, SIGNAL(stateChanged(int)), this, SLOT(onEditingFinished()));
        layout->addWidget(checkBox, 1);
    } else if (valueDef->options().isUsed()) {
        QComboBox* comboBox = new QComboBox();
        comboBox->setObjectName("value");
        connect(comboBox, SIGNAL(editTextChanged(QString)), this, SLOT(onValueEdited()));
        connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onEditingFinished()));
        if (validator) {
            comboBox->setValidator(validator);
            validator->setParent(comboBox);
            validator = nullptr;
        }
        layout->addWidget(comboBox, 1);
    } else {
        QLineEdit* lineEdit = new QLineEdit();
        lineEdit->setObjectName("value");
        connect(lineEdit, SIGNAL(textEdited(QString)), this, SLOT(onValueEdited()));
        connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(onEditingFinished()));
        if (validator) {
            lineEdit->setValidator(validator);
            validator->setParent(lineEdit);
            validator = nullptr;
        }
        if (valueDef->valueType() == Model::UnionType::Integer ||
            valueDef->valueType() == Model::UnionType::Double) {
            lineEdit->setAlignment(Qt::AlignRight);
        }
        layout->addWidget(lineEdit, 1);
    }

    if (m_itemValue.settings().hasUnit()) {
        QLabel* unitLabel = new QLabel();
        unitLabel->setText(QString::fromStdString(m_itemValue.settings().unit()));
        unitLabel->setObjectName("unit");
        layout->addWidget(unitLabel);
    }

    if (validator) { delete validator; }

    connect(m_editor, SIGNAL(destroyed(QObject*)), this, SLOT(onEditorDestroyed()));

    updateEditorValue();
}

// =============================================================================
// (protected)
void ValueEditorHandler::setItemValue(const Model::UnionRef &value)
{
    // TODO: Add undo/redo stuff here...

    if (m_itemValue.isNull() || !m_itemValue.setValue(value)) {
        // ItemValue is not changed
        updateEditorValue();
    } else {
        // ItemValue has been changed
        updateLabelValue();
    }
}

// =============================================================================
// (protected slots)
void ValueEditorHandler::onValueEdited()
{
    // Update error state
}

// =============================================================================
// (protected slots)
void ValueEditorHandler::onEditingFinished()
{
    QCheckBox* checkBox = m_editor->findChild<QCheckBox*>("value");
    if (checkBox) {
        setItemValue(checkBox->isChecked());
        emit valueEditedFinished();
        return;
    }

    QLineEdit* lineEdit = m_editor->findChild<QLineEdit*>("value");
    if (lineEdit) {
        setItemValue(lineEdit->text().toStdString());
        emit valueEditedFinished();
        return;
    }

    QComboBox* comboBox = m_editor->findChild<QComboBox*>("value");
    if (comboBox) {
        setItemValue(comboBox->currentText().toStdString());
        emit valueEditedFinished();
        return;
    }
}

// =============================================================================
// (protected slots)
void ValueEditorHandler::onLabelDestroyed()
{
    m_label = nullptr;
}

// =============================================================================
// (protected slots)
void ValueEditorHandler::onEditorDestroyed()
{
    m_editor = nullptr;
}

} // namespace View
} // namespace Oak
