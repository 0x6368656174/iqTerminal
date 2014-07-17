import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../../elements"

Rectangle {
    id: button
    property int imageNumber
    property bool checked: false
    onCheckedChanged: updateButtonColor()
    signal clicked()
    clip: true
    color: "#848484"
    Image {
        id: buttonImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: "../../images/" + imageNumber + ".png"

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
            id: ma
            anchors.fill: parent
            platformIndependentHoverEnabled: true
            onClicked: {
                button.clicked()
                buttonClickAnimation.restart()
            }
            onContainsMouseChanged: updateButtonColor()
        }
    }

    function updateButtonColor() {
        if (checked || ma.containsMouse) {
            button.color = "#f25d26"
            buttonImage.source = "../../images/" + imageNumber + "a.png"
        } else {
            button.color = "#848484"
            buttonImage.source = "../../images/" + imageNumber + ".png"
        }
    }
}
