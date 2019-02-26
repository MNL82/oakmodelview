import QtQuick 2.0
import QtQuick.Controls 2.5

Rectangle {
    id: rootId
    signal editingFinished()
    signal loaded()

    anchors.fill: parent
    color: "#ffffff"

    onLoaded: {
        textEditId.forceActiveFocus()
        textEditId.selectAll();
    }

    TextInput {
        id: textEditId
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter

        focus: true
        selectByMouse: true

        color: "#660000"
        text: edit
        validator: leafValidator
//        onTextChanged: {
//            console.log("Text changed: " +  textEditId.text)
//        }

        Connections {
            target: textEditId
            onEditingFinished: {
                console.log("Editing Fished: " + textEditId.text);
                edit = textEditId.text;
                rootId.editingFinished();
            }
        }
    }
}


