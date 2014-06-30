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
    height: Core.dp(20)
    width: Core.dp(125)

    onFocusChanged: if (focus) textInput.forceActiveFocus()

    Text {
        id: placeholder
        color: "white"
        font.pixelSize: Core.dp(8)
        anchors.centerIn: parent
//        visible: textInput.text === "" && !textInput.focus
        opacity: textInput.text === "" && !textInput.focus?1:0.1
    }

    TextInput {
        id: textInput
        clip: true
        anchors.fill: parent
        anchors.leftMargin: Core.dp(8)
        anchors.rightMargin: anchors.leftMargin
        verticalAlignment: Text.AlignVCenter
        color: "white"
        font.pixelSize: Core.dp(8)
        echoMode: lineEdit.password?TextInput.Password:TextInput.Normal
        passwordCharacter: "*"
        selectByMouse: true
        selectionColor: "#c00d0d"
        onAccepted: lineEdit.submit()
    }
}
