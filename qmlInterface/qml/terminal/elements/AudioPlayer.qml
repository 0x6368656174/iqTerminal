import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtMultimedia 5.2

Rectangle {
    id: playerRect
    property real position:  audio.position / audio.duration
    property alias text: playerNameText.text
    property bool isPlay: false
    property bool isPause: false
    property alias file: audio.source

    function play() {
        audio.play()
    }

    function pause() {
        audio.pause()
    }

    function stop() {
        audio.stop()
    }

    Audio {
        id: audio

        onPlaying: {
            isPlay = true
            isPause = false
        }
        onPaused:  {
            isPlay = false
            isPause = true
        }
        onStopped: {
            isPlay = false
            isPause = false
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        height: 1
        color: "#c6c1c7"
    }

    Text {
        id: playerNameText
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        elide: Text.ElideRight
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: Core.dp(19)
        anchors.rightMargin: Core.dp(8)
        anchors.topMargin: Core.dp(5)
        font.pixelSize: Core.dp(8)
    }

    MediaButtons {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        position: audio.position
        duration: audio.duration
        isPlay: playerRect.isPlay
        isPause: playerRect.isPause

        onPlayClicked: {
            audio.play()
        }

        onPauseClicked: {
            audio.pause()
        }

        onStopClicked: {
            audio.stop()
        }

        onPositionSliderClicked: {
            audio.seek(newPosition)
        }
    }
}
