import QtQuick 2.0
import TerminalQmlPlugin 1.0

Rectangle {
    property string activeTab: "info"

    anchors.left: parent.left
    anchors.right: parent.right
    height: Core.dp(24)

    Row {
        anchors.fill: parent

        HeaderButton {
            imageNumber: 25
            isActive: activeTab === "info"
            onClicked: activeTab = "info"
        }
        HeaderButton {
            imageNumber: 26
            isActive: activeTab === "photo"
            onClicked: activeTab = "photo"
        }
        HeaderButton {
            imageNumber: 27
            isActive: activeTab === "video"
            onClicked: activeTab = "video"
        }
        HeaderButton {
            imageNumber: 28
            isActive: activeTab === "music"
            onClicked: activeTab = "music"
        }
    }
}
