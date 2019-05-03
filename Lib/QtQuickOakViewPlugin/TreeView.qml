import QtQuick 2.0
import Oak.Model 1.0

Item {
    id: treeViewId

    property alias model: treeViewDataId.model; // Needed at startup
    property alias rootModelIndex: treeViewDataId.rootModelIndex; // Default is root model index

    property alias spacing: treeViewDataId.spacing; // Used both in qml and C++
    property alias itemHeight: treeViewDataId.itemHeight; // Used both in qml and C++
    property int indent: 10;    // Used only in the qml
    property alias currentIndex: treeViewDataId.currentIndex; // Used from both sides

    clip: true
    height: treeListViewId.model.delegateHeight;

    TreeListView {
        anchors.fill: parent
        id: treeListViewId
        listView.interactive: true
    }

    TreeViewData
    {
        id: treeViewDataId
        rootListModel: treeListViewId.model
    }
}
