import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    id: createNewButton
    property bool enabled: true

    signal clicked()

    anchors.left: parent.left
    anchors.right: parent.right
    height: Core.dp(24)

    Rectangle {
        anchors.fill: parent
        anchors.bottomMargin: Core.dp(2)
        color: createNewButtonMA.containsMouse?"#da4504":"#c6c1c7"

        Item {
            anchors.leftMargin: Core.dp(12)
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: createNewButtonText.width
            Text {
                id: createNewButtonText
                anchors.centerIn: parent
                font.pixelSize: Core.dp(8)
                text: qsTr("Создать новую")
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
            enabled: enabled && !enableButtonTimer.running

            onClicked: {
                createButtonClickAnimation.restart()
                createNewButton.clicked()
            }
        }
    }

    Rectangle {
        id: spacer
        anchors.bottom: createNewButton.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Core.dp(2)
    }
}
