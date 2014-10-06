import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../../elements"
import "." as Welcome

Item {
    id: startViewRowItem
    property url iconSource
    property alias text: buttonText.text

    signal clicked();

    anchors.left: parent.left
    anchors.right: parent.right
    height: (28 * applicationModel.settings.zoomFactor)

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: buttonImage
                width: (28 * applicationModel.settings.zoomFactor)
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
        spacing: (8 * applicationModel.settings.zoomFactor)

        Item {
            width: height
            height: parent.height
            Welcome.Button {
                id: buttonImage
                source: startViewRowItem.iconSource
                anchors.centerIn: parent
            }
        }

        Text {
            id: buttonText
            color: "white"
            font.pixelSize: (8 * applicationModel.settings.zoomFactor)
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

