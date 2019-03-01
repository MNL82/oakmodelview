import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: rootId
    signal editingFinished()
    signal loaded()
    QtObject {
        id: internal
        property bool isLoaded: false
    }

    anchors.fill: parent
    color: "#00ffff"

    onLoaded: {
        internal.isLoaded = false;
        comboBoxId.forceActiveFocus();
        // Expand options
        comboBoxId.popup.open();
        if (comboBoxId.editable) {
            comboBoxId.model = options;
            comboBoxId.editText = edit;
            // Select all text
            comboBoxId.selectAll();
        } else {
            var list = options;
            comboBoxId.model = list;
            comboBoxId.currentIndex = list.indexOf(edit);
        }
        internal.isLoaded = true;
    }

    ComboBox {
        id: comboBoxId
        anchors.fill: parent
        focus: true
        validator: leafValidator
        editable: !optionsOnly

        onActiveFocusChanged: {
            if (!activeFocus) {
                rootId.editingFinished();
            }
        }

        onCurrentTextChanged: {
            if (!internal.isLoaded) { return; }
            console.log("CurrentText: " + currentText)
            if (edit != currentText) {
                edit = currentText;
            }
        }
    }
}


