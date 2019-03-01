import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: rootId
    signal editingFinished()
    signal loaded()

    anchors.fill: parent
    color: "#00ffff"

    onLoaded: {
        comboBoxId.forceActiveFocus();
        // Expand options
        comboBoxId.popup.open();
        if (!optionsOnly) {
            // Select all text
            comboBoxId.selectAll();
        }
    }

    ComboBox {
        id: comboBoxId
        anchors.fill: parent
        focus: true
        model: options
        editText: edit
        validator: leafValidator
        editable: !optionsOnly

        onActiveFocusChanged: {
            if (!comboBoxId.activeFocus) {
                edit = comboBoxId.currentText;
                rootId.editingFinished();
            }
        }
    }
}


