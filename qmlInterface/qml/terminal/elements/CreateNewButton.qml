import QtQuick 2.0
import TerminalQmlPlugin 1.0

Rectangle {
    id: createNewButton
    property bool enabled: true
    property string text: qsTr("Create new") + applicationModel.settings.translatorStringEnd
    property bool spacerVisible: true

    signal clicked()

    anchors.left: parent.left
    anchors.right: parent.right
    height: 24 * applicationModel.settings.zoomFactor
    color: "white"

    Rectangle {
        anchors.fill: parent
        anchors.bottomMargin: createNewButton.spacerVisible?2 * applicationModel.settings.zoomFactor:0
        color: createNewButtonMA.containsMouse?"#da4504":"#c6c1c7"

        Item {
            anchors.leftMargin: 12 * applicationModel.settings.zoomFactor
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: createNewButtonText.width
            Text {
                id: createNewButtonText
                anchors.centerIn: parent
                font.pixelSize: 8 * applicationModel.settings.zoomFactor
                text: createNewButton.text
                color: createNewButtonMA.containsMouse?"white":"black"
            }
        }

        SequentialAnimation {
            id: createButtonClickAnimation
            PropertyAnimation {
                target: createNewButtonText
                property: "scale"
                from: 1.0
                to: 1.2
                duration: 200
            }
            ScriptAction {
                script: {
                    createNewButtonText.scale = 1.0
                }
            }
        }

        Timer {
            id: enableButtonTimer
            interval: 1000
        }

        TerminalMouseArea {
            id: createNewButtonMA
            anchors.fill: parent
            platformIndependentHoverEnabled: true
            enabled: createNewButton.enabled

            onClicked: {
                if (!enableButtonTimer.running) {
                    enableButtonTimer.restart()
                    createButtonClickAnimation.restart()
                    createNewButton.clicked()
                }
            }
        }
    }
}
