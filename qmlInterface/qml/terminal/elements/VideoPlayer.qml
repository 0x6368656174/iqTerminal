import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import QtMultimedia 5.2
import "../pages"

Page {
    id: videoPlayer

    name: "video_player"

    property real position:  video.position / video.duration
    property alias text: playerNameText.text
    property bool isPlay: false
    property bool isPause: false
    property alias file: video.source

    signal hiden()

    onHiden: {
        stop()
    }

    function play() {
        video.play()
    }

    function pause() {
        video.pause()
    }

    function stop() {
        video.stop()
    }

    Rectangle {
        anchors.fill: parent
        color: "#6c676e"
    }

    Video {
        id: video
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        fillMode: VideoOutput.PreserveAspectFit

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

    TerminalMouseArea {
        anchors.fill: parent
        onClicked: {
            if (controlsRect.anchors.bottomMargin < 0) {
                controlsRect.anchors.bottomMargin = 0
                hideTimer.restart()
            } else {
                videoPlayer.hiden()
            }
            onPressAndHold: {}
        }

        Rectangle {
            id: controlsRect
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: Core.dp(55)
            anchors.bottomMargin: -height
            Behavior on anchors.bottomMargin {NumberAnimation {duration: 200} }
            color: "#80c6c1c7"

            Timer {
                id: hideTimer
                interval: 5000
                onTriggered: controlsRect.anchors.bottomMargin = - controlsRect.height
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {}
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
                position: video.position
                duration: video.duration
                isPlay: videoPlayer.isPlay
                isPause: videoPlayer.isPause

                onPlayClicked: {
                    hideTimer.restart()
                    video.play()
                }

                onPauseClicked: {
                    hideTimer.restart()
                    video.pause()
                }

                onStopClicked: {
                    hideTimer.restart()
                    video.stop()
                }

                onPositionSliderClicked: {
                    hideTimer.restart()
                    video.seek(newPosition)
                }
            }
        }
    }
}
