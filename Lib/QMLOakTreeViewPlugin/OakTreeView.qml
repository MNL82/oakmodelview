import QtQuick 2.0
import QtQuick.Controls 2.5
import Oak.TreeView 1.0

Rectangle {
    id: treeViewId

    property alias model: internalModelId.treeModel; // Needed at startup
    property alias rootModelIndex: internalModelId.rootModelIndex; // Default is root model index

    property alias spacing: listViewId.spacing;
    property alias currentItem: internalModelId.currentTreeModelIndex;

    property bool arrowStyle: true

    property int indent: 10;    // Used only in the qml

    color: "transparent"
    clip: true

    ListView {
        id: listViewId

        anchors.fill: parent

        model: TreeViewInternalModel {
            id: internalModelId
            currentGlobalRow: listViewId.currentIndex
            onCurrentGlobalRowForceUpdate: {
                listViewId.currentIndex = internalModelId.currentGlobalRow;
            }
        }

        delegate: Item {
            id: delegateId

            anchors.left: parent.left
            anchors.right: parent.right
            implicitHeight: 30

            Rectangle { // Idem
                id: rectId
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                implicitHeight: 30
                color: listViewId.currentIndex === index ? "#aaaaee" : "#ffffff"

                Item {
                    anchors.left: parent.left
                    anchors.right: buttonItemId.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom

                    Text {
                        anchors.left: parent.left
                        anchors.leftMargin: 6 + depth * treeViewId.indent // depth
                        anchors.verticalCenter: parent.verticalCenter
                        text: display
                    }
                    MouseArea {
                        id: mouseAreaTextId
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        onClicked: {
                            listViewId.currentIndex = index;
                            mouse.accepted = true;
                        }
                    }
                }

                Item {
                    id: buttonItemId
                    visible: hasChildren;
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width: buttonItemId.visible ? height : 0

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
                            source: "expandArrow.png"
                        }

                        text: (arrowStyle) ? "" : ( expanded ) ? "-" : "+"
                    }
                    MouseArea {
                        id: mouseAreaButtonId
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton
                        //hoverEnabled: true
                        onClicked: {
                            expanded = !expanded;
                            mouse.accepted = true;
                        }
                    }
                }
            }
        }
    }
}


