import QtQuick 2.0
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0
import Oak.Model 1.0

ListView {
    property alias buttons: selectedToolBarModelId.buttons
    property alias sourceItem: selectedToolBarModelId.sourceItem

    id: selectionToolBarId
    anchors.fill: parent
    orientation: ListView.Horizontal
    spacing: 4

    height: 32

    model: OakToolBarModel {
        id: selectedToolBarModelId

        //sourceItem: treeViewId.currentIndex

        buttons: [
            OakToolButtonAddChild {

            },
            OakToolButtonRemove {

            }

        ]

    }
//    delegate: ToolButton {
//        implicitWidth: 30
//        implicitHeight: 30
//        onClicked: selectedToolBarModelId.trigger(index)
//        Text {
//            anchors.fill: parent
//            text: name
//        }
//        Image {
//            id: imageId
//            anchors.fill: parent
//            anchors.margins: parent.height/7
//            source: imagePath
//            fillMode: Image.PreserveAspectFit
//            layer.enabled: !parent.enabled
//            layer.effect: Desaturate {
//                desaturation: 1
//                Behavior on desaturation { NumberAnimation { easing.type: Easing.InOutQuad } }
//            }
//        }
//    }
    delegate: ToolButton {
        height: height = parent.height
        width: height
        onClicked: selectedToolBarModelId.trigger(index)

        Image {
            id: imageId
            anchors.fill: parent
            anchors.margins: parent.height/7
            source: imagePath
            fillMode: Image.PreserveAspectFit
            layer.enabled: !parent.enabled
            layer.effect: Desaturate {
                desaturation: 1
                Behavior on desaturation { NumberAnimation { easing.type: Easing.InOutQuad } }
            }
        }
        hoverEnabled: true
        highlighted: hovered
        ToolTip {
            id: toolTipId
            text: toolTip
            visible: parent.hovered
        }
    }
}
