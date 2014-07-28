import QtQuick 2.0
import TerminalQmlPlugin 1.0

Rectangle {
    id: button
    property int imageNumber
    property bool isActive: false
    property bool inverse: false

    onIsActiveChanged: {
        if (!clickAnimation.running)
            button.state = getState()
    }

    signal clicked()

    width: Core.dp(39)
    height: Core.dp(24)
    color: "transparent"

    Item {
        anchors.fill: parent
        clip: true
        Image {
            id: activeImage
            visible: false
            anchors.fill: parent
            source: !inverse?"../images/" + imageNumber + ".png":"../images/" + imageNumber + "a.png"
        }

        Image {
            id: unactiveImage
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            source: !inverse?"../images/" + imageNumber + "a.png":"../images/" + imageNumber + ".png"
        }
    }

    TerminalMouseArea {
        id: ma
        anchors.fill: parent
        platformIndependentHoverEnabled: true
//        enabled: !isActive
        onEnabledChanged: {
            button.color = "transparent"
        }
        onContainsMouseChanged: {
            if (isActive) {
                button.color = "transparent"
                return
            }
            if (containsMouse) {
                button.color = "#ff5d1b"
            } else {
                button.color = "transparent"
            }
        }

        onClicked: {
            if (isActive)
                return
            clickAnimation.restart()
            button.clicked()
        }
    }

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: activeImage
                visible: false
            }
            PropertyChanges {
                target: unactiveImage
                visible: true
            }
        },

        State {
            name: "active"
            PropertyChanges {
                target: activeImage
                visible: true
            }
            PropertyChanges {
                target: unactiveImage
                visible: false
            }
        }
    ]

    function getState() {
        if (button.isActive) {
            return "active"
        }
        return "normal"
    }

    SequentialAnimation {
        id: clickAnimation
        ScriptAction {
            script: {
                unactiveImage.visible = true
                stateTimer.start()
            }
        }

        PropertyAnimation {
            target: unactiveImage
            property: "scale"
            from: 1.0
            to: 1.6
            duration: 150
        }
        ScriptAction {
            script: {
                unactiveImage.scale = 1.0
            }
        }
    }

    Timer {
        id: stateTimer
        interval: 170
        onTriggered: {
            button.state = getState()
        }
    }
}
