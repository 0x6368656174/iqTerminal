import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    id: bar
    height: Core.dp(22)

    anchors.left: parent.left
    anchors.right: parent.right

    signal buttonClicked(var buttonType)

    Row {
        id: buttons
        anchors.fill: parent
        EditButton {
            type: "submit"
            width: parent.width / 2
            onClicked: bar.buttonClicked("submit")
        }

        EditButton {
            type: "back"
            width: parent.width / 2
            onClicked: bar.buttonClicked("cancel")
        }
    }
}
