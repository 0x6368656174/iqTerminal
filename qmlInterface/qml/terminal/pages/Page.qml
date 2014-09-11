import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    id: page
    property string name
    property bool showHideAnimationRunning: {
        return hideLeftAnimations.running || showLeftAnimations.running || hideRightAnimations.running || showRightAnimations.running
    }

    clip: true

    width: parent.width
    height: parent.height

    Image {
        anchors.fill: parent
        fillMode: Image.Tile
        height: width
        source: "../images/1a.png"
    }

    function hideLeft() {
        hideLeftAnimations.start()
    }

    function showLeft() {
        showLeftAnimations.start()
    }

    function showRight() {
        showRightAnimations.start()
    }

    function hideRight() {
        hideRightAnimations.start()
    }

    SequentialAnimation {
        id: hideLeftAnimations
        ScriptAction {
            script: {
                page.anchors.left = undefined
                page.anchors.horizontalCenter = parent.horizontalCenter
                page.z = 1
            }
        }
        PropertyAnimation {
            target: page
            properties: "scale"
            from: 1
            to: 0.8
            duration: 200
        }
        ScriptAction {
            script: {
                hideRect.visible = true
            }
        }

        PauseAnimation {
            duration: 400
        }
        ScriptAction {
            script: {
                page.z = 0
                page.visible = false
            }
        }
    }

    SequentialAnimation {
        id: showLeftAnimations
        ScriptAction {
            script: {
                page.anchors.left = undefined
                page.anchors.horizontalCenter = parent.horizontalCenter
                page.z = 1
                page.scale = 0.8
                hideRect.visible = true
                page.visible = true
            }
        }
        PauseAnimation {
            duration: 400
        }
        ScriptAction {
            script: {
                hideRect.visible = false
            }
        }
        PropertyAnimation {
            target: page
            properties: "scale"
            from: 0.8
            to: 1
            duration: 200
        }
        ScriptAction {
            script: {
                page.z = 2
            }
        }
    }

    SequentialAnimation {
        id: showRightAnimations
        ScriptAction {
            script: {
                page.anchors.horizontalCenter = undefined
                page.anchors.left = parent.left
                page.anchors.leftMargin = page.width
                page.z = 2
                scale = 1
                hideRect.visible = false
                page.visible = true
            }
        }
        PauseAnimation {
            duration: 200
        }
        PropertyAnimation {
            target: page
            properties: "anchors.leftMargin"
            from: page.width
            to: 0
            duration: 400
        }
    }

    SequentialAnimation {
        id: hideRightAnimations
        ScriptAction {
            script: {
                page.anchors.horizontalCenter = undefined
                page.anchors.left = parent.left
                page.anchors.leftMargin = 0
            }
        }
        PropertyAnimation {
            target: page
            properties: "anchors.leftMargin"
            from: 0
            to: page.width
            duration: 400
        }
        ScriptAction {
            script: {
                page.visible = false
            }
        }
    }

    Rectangle {
        id: hideRect
        anchors.fill: parent
        z: 2
        color: "black"
        opacity: 0.75
        visible: false
    }
}
