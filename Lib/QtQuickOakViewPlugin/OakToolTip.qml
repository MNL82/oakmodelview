import QtQuick 2.0
import QtQuick.Controls 2.4

Item {
    property alias text: toolTipId.text
    anchors.fill: parent
    MouseArea {
        id: mouseAreaId
        anchors.fill: parent
        hoverEnabled: true
    }
    ToolTip {
        id: toolTipId
        visible: mouseAreaId.containsMouse && toolTipId.text
    }
}
