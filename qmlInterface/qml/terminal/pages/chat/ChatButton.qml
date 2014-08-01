import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../../elements"

Rectangle {
    id: button
    property int imageNumber
    property bool checked: false
    property bool platformIndependentHoverEnabled: true
    property string imageFolder: "../../images/"
    property color hoverColor: "#f25d26"
    property color backgroundColor: "#848484"
    onCheckedChanged: updateButtonColor()
    signal clicked()
    clip: true
    color: backgroundColor
    Image {
        id: buttonImage
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        source: imageFolder + imageNumber + ".png"

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
            platformIndependentHoverEnabled: button.platformIndependentHoverEnabled
            onClicked: {
                button.clicked()
                buttonClickAnimation.restart()
            }
            onContainsMouseChanged: updateButtonColor()
        }
    }

    function updateButtonColor() {
        if (checked || ma.containsMouse) {
            button.color = button.hoverColor
            buttonImage.source = imageFolder + imageNumber + "a.png"
        } else {
            button.color = button.backgroundColor
            buttonImage.source = imageFolder + imageNumber + ".png"
        }
    }
}
