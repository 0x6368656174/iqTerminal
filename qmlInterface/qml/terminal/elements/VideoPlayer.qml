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
    property var playlist: []

    signal hiden()

    width: rotation === 0?parent.width:parent.height
    height: rotation === 0?parent.height:parent.width

    onHiden: {
        video.stop()
    }

    onIsPlayChanged: {
        if (isPlay)
            audioPlayer.stop()
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

        onStatusChanged: {
            if (status === MediaPlayer.EndOfMedia) {
                //Включим следующий файл из списка
                if (playlist.length === 0)
                    return
                var currentIndex = -1
                for (var i = 0; i < playlist.length; ++i) {
                    if (playlist[i][1].toString() === file.toString()) {
                        currentIndex = i
                        break
                    }
                }
                ++currentIndex
                if (currentIndex >= playlist.length)
                    currentIndex = 0
                text = playlist[currentIndex][0]
                file = playlist[currentIndex][1]
                play()

            }
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
            height: (55 * applicationModel.settings.zoomFactor)
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
                anchors.leftMargin: (19 * applicationModel.settings.zoomFactor)
                anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                anchors.topMargin: (5 * applicationModel.settings.zoomFactor)
                font.pixelSize: (8 * applicationModel.settings.zoomFactor)
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
                    videoPlayer.hiden()
                }

                onPositionSliderClicked: {
                    hideTimer.restart()
                    video.seek(newPosition)
                }
            }
        }
    }

    Button {
        id: rotateButton
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: (26 * applicationModel.settings.zoomFactor)
        width: (48 * applicationModel.settings.zoomFactor)
        source: "../images/203.png"

        onClicked: {
            if(videoPlayer.rotation === 0)
                videoPlayer.rotation = 90
            else
                videoPlayer.rotation = 0
        }
    }
}
