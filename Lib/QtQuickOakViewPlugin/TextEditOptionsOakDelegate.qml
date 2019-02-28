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
        editable: optionsOnly

        onActiveFocusChanged: {
            console.log("ActiveFocus: " + comboBoxId.activeFocus ? "yes" : "no");
        }

        onCurrentTextChanged: {
            console.log("CurrentText: " + comboBoxId.currentText);
            edit = comboBoxId.currentText;
        }

        Connections {
            target: comboBoxId

            onAccepted: {
                edit = comboBoxId.editText;
                rootId.editingFinished();
            }
        }
    }

//    TextInput {
//        id: textEditId
//        anchors.fill: parent
//        verticalAlignment: Text.AlignVCenter

//        focus: true
//        selectByMouse: true

//        color: "#660000"
//        text: edit
//        validator: leafValidator
////        onTextChanged: {
////            console.log("Text changed: " +  textEditId.text)
////        }

//        Connections {
//            target: textEditId
//            onEditingFinished: {
//                //console.log("Editing Fished: " + textEditId.text);
//                edit = textEditId.text;
//                rootId.editingFinished();
//            }
//        }
//    }
}


