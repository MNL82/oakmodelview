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

Window {
    id: windowId
    visible: true
    width: 800
    height: 600
    title: qsTr("Testing Qt Quick 2")


    ContinuousChromatographyModel {
        id: oakModelId
    }

    OakNodeProxyModel {
        id: oakNodeModelId
        sourceModelItem: treeViewId.currentIndex
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
                OakToolButton {
                    enabled: true
                    imageSource: "images/folder_64.png"
                    toolTip: "Load test file"
                    onClicked: {
                        oakModelId.loadModel("C:/dev/oakmodelview/QtQuickTest/Test.xml");
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true

            RowLayout {
                anchors.fill: parent
                spacing: 0

                QQC1.TreeView {

                    id: treeViewId
                    Layout.fillHeight: true
                    implicitWidth: 250
                    model: oakModelId
                    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
                    verticalScrollBarPolicy: Qt.ScrollBarAlwaysOn


                    rowDelegate: Rectangle {
                        color: (styleData.selected) ? "#c0c0ff" : (styleData.row % 2 == 0) ? "#f8f8f8" : "#f0f0f0"
                        height: 30
                    }

                    onWidthChanged: {
                        console.log("Width: " + width);
                    }


                    style: TreeViewStyle {
//                        scrollBarBackground: Rectangle {
//                            color: "blue"
//                        }

                        branchDelegate: Item {
//                            id: branchId
//                            width: 10
//                            height: 20
//                            MouseArea {
//                                anchors.fill: parent
//                                acceptedButtons: Qt.LeftButton
//                                onClicked: {
//                                    if (styleData.isExpanded) {
//                                        treeViewId.collapse(styleData.index);
//                                    } else {
//                                        treeViewId.expand(styleData.index);
//                                    }
//                                }
//                            }
//                            Rectangle {
//                                anchors.fill: parent
//                                color: "red"
//                            }
                        }
                    }

                    QQC1.TableViewColumn {
                        title: "Name"
                        role: "display"
                        resizable: false
                    }
//                    QQC1.TableViewColumn {
//                        title: "Key"
//                        role: "keyValue"
//                        resizable: true
//                        //width: 150
//                    }

                    itemDelegate: OakTreeViewDelegate { view: treeViewId }

                }

                Rectangle {
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
