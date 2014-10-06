import QtQuick 2.0
import TerminalQmlPlugin 1.0

Rectangle {
    property string activeTab: ""

    anchors.left: parent.left
    anchors.right: parent.right
    height: 24 * applicationModel.settings.zoomFactor
}
