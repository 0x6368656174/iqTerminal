import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../../elements" as Elements

Image {
    id: button

    property bool widthAnimationEnabled: true
    property real baseWidth: Core.dp(28)
    property bool hide: false
    property bool hover: false

    function click() {
        clickAmitation.restart()
        button.clicked()
    }

    signal clicked()

    state: {
        if ((ma.containsMouse || hover) ) {
            return "hover"
        } else if (hide) {
            return "hiden"
        }
        return "normal"
    }

    Timer {
        id: stateTimer
        interval: 220
        onTriggered: {
            if ((ma.containsMouse || hover) ) {
                button.width = baseWidth * 1.2
            } else if (hide) {
                button.width = 0
            } else {
                button.width = baseWidth
            }
        }
    }

    SequentialAnimation {
        id: clickAmitation
        ScriptAction {
            script: stateTimer.start()
        }

        NumberAnimation {
            target: button
            property: "width"
            to: baseWidth
            duration: 100
        }
        NumberAnimation {
            target: button
            property: "width"
            to: baseWidth * 1.2
            duration: 100
        }
    }

    height: width

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: button
                width: baseWidth
            }
        },
        State {
            name: "hover"
            PropertyChanges {
                target: button
                width: baseWidth *1.2
            }
        },
        State {
            name: "hiden"
            PropertyChanges {
                target: button
                width: 0
            }
        }

    ]

    Behavior on width {
        enabled: widthAnimationEnabled && !clickAmitation.running
        NumberAnimation { duration: 200 }
    }

    Elements.TerminalMouseArea {
        id: ma
        anchors.fill: parent
        platformIndependentHoverEnabled: true
        onPressed: {
            click()
        }
    }
}
