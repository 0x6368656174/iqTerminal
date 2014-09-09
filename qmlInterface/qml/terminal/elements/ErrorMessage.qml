import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    property alias text: errorTextText.text

    id: errorMessage

    opacity: parent.visible?1:0
    height: errorTextText.lineCount * Core.dp(16)
    Behavior on opacity {NumberAnimation {duration: 200} }

    onVisibleChanged: {
        if (!visible) {
            text = ""
        }
    }

    PropertyAnimation {
        id: showErrorAmination
        target: errorBox
        property: "anchors.horizontalCenterOffset"
        from: Core.dp(12)
        to: 0
        easing.type: Easing.OutElastic
        easing.amplitude: Core.dp(2)
        duration: 800
        running: errorMessage.visible
    }

    Rectangle {
        id: errorBox
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
        border.color: "#FF0000"
        color: "#FBEFEF"
        width: Core.dp(125)

        Text {
            id: errorTextText
            color: "#FF0000"
            font.pixelSize: Core.dp(8)
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: Core.dp(8)
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }
    }
}
