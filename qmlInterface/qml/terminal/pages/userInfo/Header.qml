import QtQuick 2.0
import "../../elements"

BaseHeader {
    Row {
        anchors.fill: parent

        HeaderButton {
            imageNumber: 25
            width: parent.width/4
            isActive: activeTab === "info"
            onClicked: activeTab = "info"
        }
        HeaderButton {
            imageNumber: 26
            width: parent.width/4
            isActive: activeTab === "photo"
            onClicked: activeTab = "photo"
        }
        HeaderButton {
            imageNumber: 27
            width: parent.width/4
            isActive: activeTab === "video"
            onClicked: activeTab = "video"
        }
        HeaderButton {
            imageNumber: 28
            width: parent.width/4
            isActive: activeTab === "music"
            onClicked: activeTab = "music"
        }
    }
}
