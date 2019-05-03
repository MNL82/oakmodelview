import QtQuick 2.0
import Oak.Model 1.0

Item {
    id: treeListViewId
    property int depth: 0
    property alias model: listModelId
    property alias listView: listViewId

    TreeViewListModel {
        id: listModelId
        treeViewData: treeViewDataId
    }

    ListView {
        id: listViewId

        anchors.fill: parent

        model: listModelId
        spacing: treeViewId.spacing
        interactive: false

        delegate: TreeViewInternalDelegate {
        }
    }

}


