import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../../elements"

Item {
    id: button
    property alias source: buttonImage.source
    signal clicked()
    Image {
        id: buttonImage
        anchors.fill: parent

        SequentialAnimation {
            id: buttonClickAnimation
            PropertyAnimation {
                target: buttonImage
                property: "scale"
                from: 1.0
                to: 1.4
                duration: 200
            }
            ScriptAction {
                script: {
                    buttonImage.scale = 1.0
                }
            }
        }

        TerminalMouseArea {
            anchors.fill: parent
            onClicked: {
                button.clicked()
                buttonClickAnimation.restart()
            }
        }
    }
}
