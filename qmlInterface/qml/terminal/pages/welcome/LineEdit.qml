import QtQuick 2.0
import TerminalQmlPlugin 1.0

Rectangle {
    id: lineEdit

    property alias text: textInput.text
    property alias placeholderText: placeholder.text
    property bool password: false

    signal submit()

    border.color: "#c00d0d"
    border.width: 1
    color: "transparent"
    height: (20 * applicationModel.settings.zoomFactor)
    width: (125 * applicationModel.settings.zoomFactor)

    onFocusChanged: if (focus) textInput.forceActiveFocus()

    Text {
        id: placeholder
        color: "#b1b1b1"
        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
        anchors.centerIn: parent
        visible: textInput.text === "" && !textInput.focus
    }

    TextInput {
        id: textInput
        clip: true
        anchors.fill: parent
        anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
        anchors.rightMargin: anchors.leftMargin
        verticalAlignment: Text.AlignVCenter
        color: "white"
        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
        echoMode: lineEdit.password?TextInput.Password:TextInput.Normal
        passwordCharacter: "*"
        selectByMouse: true
        selectionColor: "#c00d0d"
        onAccepted: lineEdit.submit()
        horizontalAlignment: Text.AlignHCenter
    }
}
