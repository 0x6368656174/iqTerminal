import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    id: buttons
    property int position: 0
    property int duration: 0
    property bool isPlay: false
    property bool isPause: false
    property bool isStop: false
    property string style: "orange"

    height: Core.dp(38)

    signal playClicked()
    signal stopClicked()
    signal pauseClicked()
    signal positionSliderClicked(var newPosition)
    signal finished()

    onIsPlayChanged: {
        if (!isPlay && position >= duration)
            finished()
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {}
    }

    Button {
        id: playButton
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: Core.dp(6)
        anchors.leftMargin: Core.dp(19)
        width: Core.dp(10)
        height: width
        source: {
            if(buttons.isPlay||buttons.isPause||position>0) {
                if(style === "orange"){
                    return "../images/50a.png"
                } else {
                    return "../images/50b.png"
                }
            } else return "../images/50.png"
        }

        onClicked: playClicked()
    }

    Button {
        id: pauseButton
        anchors.horizontalCenter: barImage.horizontalCenter
        anchors.verticalCenter: playButton.verticalCenter
        width: Core.dp(10)
        height: width
        source: "../images/51.png"

        onClicked: pauseClicked()
    }

    Button {
        id: stopButton
        anchors.right: endImage.right
        anchors.verticalCenter: playButton.verticalCenter
        width: Core.dp(10)
        height: width
        source: "../images/52.png"

        onClicked: stopClicked()
    }


    Image {
        id: barImage
        anchors.verticalCenter: startImage.verticalCenter
        anchors.left: startImage.right
        anchors.right: endImage.left
        anchors.leftMargin: -Core.dp(5)
        anchors.rightMargin: anchors.leftMargin
        height: Core.dp(10)
        source: "../images/49d.png"
    }

    Image {
        id: positionImage
        anchors.verticalCenter: startImage.verticalCenter
        anchors.left: startImage.right
        anchors.leftMargin: -Core.dp(5)
        width: duration > 0?(endImage.x - startImage.x) * position / duration:0
        Behavior on width {NumberAnimation {duration: 200; easing.type: Easing.OutQuad;} }
        height: Core.dp(4)
        source: {
            if (style === "orange") {
                return "../images/49b.png"
            } else {
                return "../images/49bbb.png"
            }
        }
    }

    Image {
        id: startImage
        anchors.left: parent.left
        anchors.top: playButton.bottom
        anchors.topMargin: Core.dp(6)
        anchors.leftMargin: playButton.anchors.leftMargin
        width: Core.dp(10)
        height: width
        source: {
            if (position > 0) {
                if (style === "orange") {
                    return "../images/49.png"
                } else {
                    return "../images/49bb.png"
                }
            }else {
                return "../images/49c.png"
            }
        }
    }

    Image {
        id: endImage
        anchors.verticalCenter: startImage.verticalCenter
        anchors.right: positionText.left
        anchors.rightMargin: Core.dp(11)
        width: Core.dp(10)
        height: width
        rotation: 180
        source: {
            if (positionImage.width>barImage.width-Core.dp(6.5)) {
                if (style === "orange") {
                    return "../images/49.png"
                } else {
                    return "../images/49bb.png"
                }
            }else {
                return "../images/49c.png"
            }
        }
    }

    Image {
        id: positionEnd
        anchors.verticalCenter: startImage.verticalCenter
        anchors.horizontalCenter: positionImage.right
        visible: position > 0
        width: Core.dp(8)
        height: width
        source: {
            if (style === "orange") {
                return "../images/49.png"
            } else {
                return "../images/49bb.png"
            }
        }
    }

    Text {
        id: positionText
        anchors.right: parent.right
        anchors.rightMargin: playButton.anchors.leftMargin
        anchors.verticalCenter: endImage.verticalCenter
        font.pixelSize: Core.dp(8)
        text: {
            var minute = parseInt(position/60000)
            var minuteStr = minute
            var seconds = parseInt((position - parseInt(position/60000)*60000)/1000)
            var secondsStr = ""
            if (seconds < 10) {
                secondsStr = "0" + seconds
            } else {
                secondsStr = seconds
            }
            return minuteStr + ":" + secondsStr
        }
    }
    TerminalMouseArea {
        anchors.left: startImage.left
        anchors.right: endImage.right
        anchors.margins: startImage.width/2
        anchors.verticalCenter: startImage.verticalCenter
        height: startImage.height

        onClicked: {
            var newPosition = (mouse.x)/width
            positionSliderClicked(duration * newPosition)
        }
    }
}
