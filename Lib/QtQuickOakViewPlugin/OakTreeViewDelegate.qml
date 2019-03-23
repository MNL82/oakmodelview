import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 1.4 as QQC1

Item {
    property QQC1.TreeView view: null
    property bool arrowStyle: true
    id:delegateId
    height: 30
    implicitHeight: 30

    Text {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        text: styleData.value
    }
    Button {
        id: expandButtonId
        visible: styleData.hasChildren && styleData.column === 0 && view != null
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: 4
        width: height
        //highlighted: mouseAreaId.containsMouse
        flat: true

        Image {
            id: imageId
            visible: arrowStyle
            width: Math.min(parent.width, 12)
            height: Math.min(parent.height, 12)
            anchors.centerIn: parent
            rotation: -90
            source: "expandArrow_32.png"
        }

        text: (arrowStyle) ? "" : ( styleData.isExpanded ) ? "-" : "+"

        MouseArea {
            id: mouseAreaId
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            //hoverEnabled: true
            onClicked: {
                if (styleData.isExpanded) {
                    view.collapse(styleData.index);
                    if (arrowStyle) { imageId.rotation = -90; }
                } else {
                    view.expand(styleData.index);
                    if (arrowStyle) { imageId.rotation = 0; }
                }
                mouse.accepted = true;
            }
        }
    }
}
