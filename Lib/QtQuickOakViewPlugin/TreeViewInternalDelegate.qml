import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
    id: delegateId

    property int depth: 0
    property bool arrowStyle: true

    anchors.left: parent.left
    anchors.right: parent.right
    implicitHeight: treeViewDataId.itemHeight

    Rectangle { // Idem
        id: rectId
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        implicitHeight: treeViewDataId.itemHeight
        color: isCurrent ? "#aaaaee" : "#ffffff"

        Item {
            anchors.left: parent.left
            anchors.right: buttonItemId.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 6 + treeListViewId.depth * treeViewId.indent
                anchors.verticalCenter: parent.verticalCenter
                text: display
            }
            MouseArea {
                id: mouseAreaTextId
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                onClicked: {
                    isCurrent = true;

                    mouse.accepted = true;
                }
            }
        }

        Item {
            id: buttonItemId
            visible: hasChildren
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: hasChildren ? height : 0

            Button {
                id: expandButtonId
                anchors.fill: parent
                anchors.margins: 4

                //highlighted: mouseAreaButtonId.containsMouse
                flat: true

                Image {
                    id: imageId
                    visible: arrowStyle
                    width: Math.min(parent.width, 12)
                    height: Math.min(parent.height, 12)
                    anchors.centerIn: parent
                    rotation: expanded ? 0 : -90;
                    source: "expandArrow_32.png"
                }

                text: (arrowStyle) ? "" : ( expanded ) ? "-" : "+"
            }
            MouseArea {
                id: mouseAreaButtonId
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                //hoverEnabled: true
                onClicked: {
                    // Load the child list before the first time the item is expanded
                    if (expanded === false && childListLoaderId.sourceComponent === null) {
                        childListLoaderId.sourceComponent = Qt.createComponent("TreeListView.qml");
                        childListLoaderId.active = true;
                    }
                    expanded = !expanded;
                    mouse.accepted = true;
                }
            }
        }
    }

    Loader {
        id: childListLoaderId
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: rectId.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: treeViewId.spacing
        visible: expanded

        active: false

        onLoaded: {
            item.depth = treeListViewId.depth + 1; // No databinding
            item.model.treeViewData = treeListViewId.model.treeViewData;
            item.model.parentModelIndex = refModelIndex;
            treeListViewId.model.addChildModel(index, item.model)

            delegateId.height = Qt.binding(function() { return oakModel.delegateHeight });
        }
    }
}
