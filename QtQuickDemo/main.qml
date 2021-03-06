import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import Qt.labs.qmlmodels 1.0

import QtQuick.Controls 1.4 as QQC1
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Oak.Model 1.0
import Oak.View 1.0
import Oak.TreeView 1.0

Window {
    id: windowId
    visible: true
    width: 800
    height: 600
    title: qsTr("OakModelView Qt Quick Demo")

    TaskTrackerModel {
        id: oakModelId
    }

    OakNodeProxyModel {
        id: oakNodeModelId
        sourceItem: treeViewId.currentItem
    }

    FileDialog {
        id: loadModelDialogId
        selectFolder: false
        selectExisting: true
        nameFilters: [ "Xml file (*.xml)" ]
        title: "Load Model"
        modality: Qt.WindowModal
        onAccepted: oakModelId.loadModel(fileUrl)
    }

    FileDialog {
        id: saveAsModelDialogId
        selectExisting: false
        selectFolder: false
        nameFilters: [ "Xml file (*.xml)" ]
        title: "Load Model"
        modality: Qt.WindowModal
        onAccepted: oakModelId.saveModelAs(fileUrl)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            id: toolBarId
            Layout.fillWidth: true
            RowLayout {
                OakToolButton {
                    enabled: oakModelId.newActionEnabled
                    imageSource: "images/doc_64.png"
                    toolTip: "New"
                    onClicked: oakModelId.newModel()
                }
                OakToolButton {
                    enabled: oakModelId.loadActionEnabled
                    imageSource: "images/folder_64.png"
                    toolTip: "Load file"
                    onClicked: loadModelDialogId.open()
                }
                OakToolButton {
                    enabled: oakModelId.saveActionEnabled
                    imageSource: "images/save_64.png"
                    toolTip: "Save file"
                    onClicked: oakModelId.saveModel()
                }
                OakToolButton {
                    enabled: oakModelId.saveAsActionEnabled
                    imageSource: "images/save-as-64.png"
                    toolTip: "Save As file"
                    onClicked: saveAsModelDialogId.open()
                }
//                OakToolButton {
//                    enabled: true
//                    imageSource: "images/folder_64.png"
//                    toolTip: "Load test file"
//                    onClicked: {
//                        oakModelId.loadModel("C:/dev/oakmodelview/QtQuickDemo/Test.xml");
//                    }
//                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                spacing: 0


                Item { // Tree View and ToolBar
                    Layout.fillHeight: true
                    implicitWidth: 250

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        Rectangle {
                            color: "#aaaaaa"
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            OakTreeView {
                                id: treeViewId
                                anchors.fill: parent
                                model: oakModelId
                                spacing: 0
                                //indent: 10
//                                itemHeight: 30
//                                maxDepth: -1
                            }

                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.margins: 4
                            implicitHeight: 30
                            //visible: selectedToolBarModelId.rowCount() !== 0

                            ToolBarSelection {
                                height: 22
                                sourceItem: treeViewId.currentItem
//                                buttons: [
//                                    OakToolButtonAddChild {

//                                    }
//                                ]
                            }

//                            ListView {
//                                id: selectionToolBarId
//                                anchors.fill: parent
//                                orientation: ListView.Horizontal
//                                spacing: 4

//                                model: OakToolBarModel {
//                                    id: selectedToolBarModelId

//                                    sourceItem: treeViewId.currentIndex

//                                    buttons: [
//                                        OakToolButtonAddChild {

//                                        }

//                                    ]

//                                }
//                                delegate: ToolButton {
//                                    implicitWidth: 30
//                                    implicitHeight: 30
//                                    onClicked: selectedToolBarModelId.trigger(index)
//                                    Text {
//                                        anchors.fill: parent
//                                        text: name
//                                    }
//                                    Image {
//                                        id: imageId
//                                        anchors.fill: parent
//                                        anchors.margins: parent.height/7
//                                        source: imagePath
//                                        fillMode: Image.PreserveAspectFit
//                                        layer.enabled: !parent.enabled
//                                        layer.effect: Desaturate {
//                                            desaturation: 1
//                                            Behavior on desaturation { NumberAnimation { easing.type: Easing.InOutQuad } }
//                                        }
//                                    }
//                                }
//                            }
                        }
                    }
                }



                Rectangle { // Leaf editors
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 4

                    ListView {
                        anchors.fill: parent
                        model: oakNodeModelId
                        spacing: 4

                        delegate: DelegateChooser {
                            role: "hasOptions"
                            DelegateChoice {
                                roleValue: true
                                delegate: OakNodeDelegate {
                                    labelComponent: LabelOakDelegate { }
                                    displayComponent: DisplayOakDelegate { }
                                    editComponent: OptionsOakDelegate { }
                                }
                            }
                            DelegateChoice {
                                delegate: OakNodeDelegate {
                                    labelComponent: LabelOakDelegate { }
                                    displayComponent: DisplayOakDelegate { }
                                    editComponent: EditOakDelegate { }
                                }
                            }
                        }
                    }

                }


            }
        }
    }

//    MessageDialog {
//        id: messageDialogId
//        title: "Warning"
//        text: "Undefined warning"
//        icon: StandardIcon.Warning
//        Component.onCompleted: visible = true
//    }

//    Item {
//        anchors.centerIn: parent
//        Rectangle {
//            id: rect1Id
//            width: 100
//            height: 200
//            color: "#33aa00"
//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    console.log("clicked")
//                }
//            }
//            ToolTip.text: "Hallo :)"
//        }
//        Rectangle {
//            id: rect2Id
//            width: 100
//            height: 200
//            color: "#24a628"
//            anchors.left: rect1Id.right
//            gradient: Gradient {
//                GradientStop {
//                    position: 0.00;
//                    color: "#24be58";
//                }
//                GradientStop {
//                    position: 1.00;
//                    color: "#c03535";
//                }
//            }
//        }
//    }
}
