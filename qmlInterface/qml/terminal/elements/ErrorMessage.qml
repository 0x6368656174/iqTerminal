import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    property alias text: errorTextText.text

    id: errorMessage

    opacity: parent.visible?1:0
    height: errorTextText.lineCount * 16 * applicationModel.settings.zoomFactor
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
        from: 12 * applicationModel.settings.zoomFactor
        to: 0
        easing.type: Easing.OutElastic
        easing.amplitude: 2 * applicationModel.settings.zoomFactor
        duration: 800
        running: errorMessage.visible
    }

    Rectangle {
        id: errorBox
        anchors.horizontalCenter: parent.horizontalCenter
        height: parent.height
        border.color: "#FF0000"
        color: "#FBEFEF"
        width: 125 * applicationModel.settings.zoomFactor

        Text {
            id: errorTextText
            color: "#FF0000"
            font.pixelSize: 8 * applicationModel.settings.zoomFactor
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 8 * applicationModel.settings.zoomFactor
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }
    }
}
