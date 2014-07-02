import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../../elements"

Item {
    id: startViewRowItem
    property url iconSource
    property alias text: buttonText.text

    signal clicked();

    anchors.left: parent.left
    anchors.right: parent.right
    height: Core.dp(28)

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: buttonImage
                width: Core.dp(28)
            }
            PropertyChanges {
                target: buttonText
                opacity: 1
            }
        },
        State {
            name: "hiden"
            PropertyChanges {
                target: buttonImage
                state: "hiden"
            }
            PropertyChanges {
                target: buttonText
                opacity: 0
            }
        }

    ]

    Row {
        anchors.left: parent.left
        anchors.right: parent.right
        height: parent.height
        spacing: Core.dp(8)

        Item {
            width: height
            height: parent.height
            Button {
                id: buttonImage
                source: startViewRowItem.iconSource
                anchors.centerIn: parent
            }
        }

        Text {
            id: buttonText
            color: "white"
            font.pixelSize: Core.dp(8)
            anchors.verticalCenter: parent.verticalCenter

            Behavior on opacity {
                NumberAnimation { duration: 200 }
            }
        }
    }

    TerminalMouseArea {
        platformIndependentHoverEnabled: true
        onContainsMouseChanged: {
            if (containsMouse) {
                buttonImage.hover = true
            } else {
                buttonImage.hover = false
            }
        }

        anchors.fill: parent
        onPressed: startViewRowItem.clicked();
    }
}

