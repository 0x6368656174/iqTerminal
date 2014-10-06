import QtQuick 2.0
import TerminalQmlPlugin 1.0

Rectangle {
    id: searchBar
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    height: (22 * applicationModel.settings.zoomFactor)
    color: "white"

    signal search(var text)

    Text {
        anchors.centerIn: parent
        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
        color: "#c6c1c7"
        text: qsTr("input text field") + applicationModel.settings.translatorStringEnd
        opacity: textInput.text === "" && !textInput.focus?1:0.5
    }

    TextInput {
        id: textInput
        clip: true
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: searchButton.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
        anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
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
        anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
        width: (14 * applicationModel.settings.zoomFactor)
        height: (14 * applicationModel.settings.zoomFactor)
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
