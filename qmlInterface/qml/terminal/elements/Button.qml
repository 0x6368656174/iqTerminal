import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    id: button
    property alias source: buttonImage.source
    signal clicked()

    onClicked: buttonClickAnimation.restart()

    Image {
        id: buttonImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit

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
            onClicked: button.clicked()
        }
    }
}
