import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../../elements"

BaseHeader {
    activeTab: "search"
    Row {
        anchors.fill: parent

        HeaderButton {
            imageNumber: 95
            inverse: true
            isActive: activeTab === "search"
            onClicked: activeTab = "search"
            width: Core.dp(66)
        }
        HeaderButton {
            imageNumber: 93
            inverse: true
            isActive: activeTab === "download"
            onClicked: activeTab = "download"
            width: Core.dp(66)
        }
        HeaderButton {
            imageNumber: 94
            inverse: true
            isActive: activeTab === "upload"
            onClicked: activeTab = "upload"
            width: Core.dp(66)
        }
    }
}
