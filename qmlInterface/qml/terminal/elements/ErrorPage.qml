import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property alias text: errorMessage.text
    property Item blurItem
    property int blurLeftMargin

    anchors.fill: parent
    id: errorBox
    visible: false

    Rectangle {
        anchors.fill: parent
        color: "#da4504"
    }

    FastBlur {
        anchors.fill: parent
        anchors.leftMargin: blurLeftMargin
        source: blurItem
        radius: 24
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: errorBox.visible?0.4:0
        Behavior on opacity {NumberAnimation { duration: 200; } }
    }


    ErrorMessage {
        id: errorMessage
        width: parent.width / 3 * 2
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: errorBox.visible = false
    }
}
