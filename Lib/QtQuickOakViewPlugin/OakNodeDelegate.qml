import QtQuick 2.0
import QtQuick.Layouts 1.3

Item {
    property bool isEditing : false

    property alias labelComponent : labelContentId.data
    property alias displayComponent : valueContentId.data
    property alias editComponent: editLoaderId.sourceComponent

    id: rootId
    anchors.left: parent.left
    anchors.right: parent.right
    implicitHeight: 30
    height: 30

    Rectangle {
        id: leafLabelId
        radius: 4
        color: "#dddddd"
        border.width: 2
        border.color: "#999999"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        implicitWidth: 90

        Item {
            id: labelContentId
            anchors.fill: parent
            anchors.leftMargin: 6
            anchors.rightMargin: 6
            anchors.topMargin: 4
            anchors.bottomMargin: 4
        }
    }

    Rectangle {
        id: leafValueId
        radius: 4
        color: "#eeeeee"
        border.width: 2
        border.color: "#999999"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: leafLabelId.right
        anchors.right: parent.right
        anchors.leftMargin: 4

        Item {
            id: valueContentId
            anchors.fill: parent
            anchors.leftMargin: 6
            anchors.rightMargin: 6
            anchors.topMargin: 4
            anchors.bottomMargin: 4
            visible: !isEditing
        }
        Loader {
            id: editLoaderId
            anchors.fill: parent
            anchors.leftMargin: 6
            anchors.rightMargin: 6
            anchors.topMargin: 4
            anchors.bottomMargin: 4
            active: isEditing
            focus: true

            onLoaded: {
                editLoaderId.item.loaded();
            }
        }
        MouseArea {
            id: mouseAreaId
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            enabled: !isEditing
            onClicked: {
                //console.log("Display value clicked");
                isEditing = true;
            }
        }
        Connections {
            target: editLoaderId.item
            onEditingFinished: {
                isEditing = false;
            }
        }
    }
//    OakToolTip {
//        text: toolTip
//    }
}
