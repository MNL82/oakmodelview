import QtQuick 2.0

Rectangle {
    radius: 4
    color: "#dddddd"
    border.width: 2
    border.color: "#999999"

    Text {
        anchors.fill: parent
        anchors.leftMargin: 6
        verticalAlignment: Text.AlignVCenter
        text: displayName + ":"
    }
}
