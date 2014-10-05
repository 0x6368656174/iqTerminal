import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../../elements"

BaseHeader {
    activeTab: "search"
    Row {
        anchors.fill: parent

        HeaderButton {
            imageNumber: 95
            width: parent.width/3
            inverse: true
            isActive: activeTab === "search"
            onClicked: activeTab = "search"
        }
        HeaderButton {
            imageNumber: 93
            width: parent.width/3
            inverse: true
            isActive: activeTab === "download"
            onClicked: activeTab = "download"
        }
        HeaderButton {
            imageNumber: 94
            width: parent.width/3
            inverse: true
            isActive: activeTab === "upload"
            onClicked: activeTab = "upload"
        }
    }
}
