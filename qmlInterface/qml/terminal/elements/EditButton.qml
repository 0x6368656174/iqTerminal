import QtQuick 2.0
import TerminalQmlPlugin 1.0

Rectangle {
    id: button
    property string type
    property bool enabled: true
    property bool mouseAreaActive: false

    visible: enabled

    signal clicked()

    width: Core.dp(22)
    height: Core.dp(22)

//    color: "#b0b5b4b9"
    color: "white"

    Item {
        anchors.fill: parent
        Image {
            id: buttonImage
            anchors.centerIn: parent
            width: Core.dp(22)
            height: width

            source: {
                if (type === "add") {
                    return "../images/43.png"
                } else if (type === "edit") {
                    return "../images/24.png"
                } else if (type === "remove") {
                    return "../images/45.png"
                } else if (type === "submit") {
                    return "../images/34b.png"
                } else if (type === "cancel") {
                    return "../images/22b.png"
                } else if (type === "selectAll") {
                    return "../images/34b.png"
                } else if (type === "deselectAll") {
                    return "../images/34.png"
                } else if (type === "addUser") {
                    return "../images/23.png"
                } else if (type === "removeUser") {
                    return "../images/36.png"
                }

                return ""
            }

            fillMode: Image.PreserveAspectFit
        }
    }

    TerminalMouseArea {
        id: ma
        anchors.fill: parent
        visible: button.mouseAreaActive
        hoverEnabled: true
        onContainsMouseChanged: {
            if (containsMouse) {
//                button.color = "#b0ff5d1b"
                button.color = "#ff5d1b"
            } else {
//                button.color = "#b0b5b4b9"
                button.color = "white"
            }
        }

        onPressed: {
            clickAnimation.restart()
            button.clicked()
        }
    }

    SequentialAnimation {
        id: clickAnimation
        PropertyAnimation {
            target: buttonImage
            property: "scale"
            from: 1.0
            to: 1.6
            duration: 150
        }
        ScriptAction {
            script: {
                buttonImage.scale = 1.0
            }
        }
    }
}
