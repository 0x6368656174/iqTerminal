import QtQuick 2.0
import TerminalQmlPlugin 1.0

Rectangle {
    id: searchBar
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    height: Core.dp(22)
    color: "white"

    signal search(var text)

    Text {
        anchors.centerIn: parent
        font.pixelSize: Core.dp(8)
        color: "#c6c1c7"
        text: qsTr("input text field")
        opacity: textInput.text === "" && !textInput.focus?1:0.5
    }

    TextInput {
        id: textInput
        clip: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: searchButton.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: Core.dp(8)
        anchors.rightMargin: Core.dp(8)
        font.pixelSize: Core.dp(8)
        verticalAlignment: Text.AlignVCenter
        selectByMouse: true
        selectionColor: "#c00d0d"
        onAccepted: searchBar.search(textInput.text)
    }

    Button {
        id: searchButton
        source: "../images/21.png"
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: Core.dp(8)
        width: Core.dp(14)
        height: Core.dp(14)
        onClicked: searchBar.search(textInput.text)
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 1
        color: "#c6c1c7"
    }
}
