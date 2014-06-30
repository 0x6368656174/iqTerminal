import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    id: buttons
    property int position: 0
    property int duration: 0
    property bool isPlay: false
    property bool isPause: false

    height: Core.dp(38)

    signal playClicked()
    signal stopClicked()
    signal pauseClicked()
    signal positionSliderClicked(var newPosition)

    Button {
        id: playButton
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: Core.dp(6)
        anchors.leftMargin: playerNameText.anchors.leftMargin
        width: Core.dp(10)
        height: width
        source: buttons.isPlay||buttons.isPause?"../../images/50a.png":"../../images/50.png"

        onClicked: playClicked()
    }

    Button {
        id: pauseButton
        anchors.horizontalCenter: barImage.horizontalCenter
        anchors.verticalCenter: playButton.verticalCenter
        width: Core.dp(10)
        height: width
        source: "../../images/51.png"

        onClicked: pauseClicked()
    }

    Button {
        id: stopButton
        anchors.right: endImage.right
        anchors.verticalCenter: playButton.verticalCenter
        width: Core.dp(10)
        height: width
        source: "../../images/52.png"

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
        source: "../../images/49d.png"
    }

    Image {
        id: positionImage
        anchors.verticalCenter: startImage.verticalCenter
        anchors.left: startImage.right
        anchors.leftMargin: -Core.dp(5)
        width: duration > 0?(endImage.x - startImage.x) * position / duration:0
        Behavior on width {NumberAnimation {duration: 200; easing.type: Easing.OutQuad;} }
        height: Core.dp(4)
        source: "../../images/49b.png"
    }

    Image {
        id: startImage
        anchors.left: parent.left
        anchors.top: playButton.bottom
        anchors.topMargin: Core.dp(6)
        anchors.leftMargin: playerNameText.anchors.leftMargin
        width: Core.dp(10)
        height: width
        source: buttons.isPlay||buttons.isPause||positionImage.width>barImage.width-Core.dp(6.5)?"../../images/49.png":"../../images/49c.png"
    }

    Image {
        id: endImage
        anchors.verticalCenter: startImage.verticalCenter
        anchors.right: positionText.left
        anchors.rightMargin: Core.dp(11)
        width: Core.dp(10)
        height: width
        rotation: 180
        source: positionImage.width>barImage.width-Core.dp(6.5)?"../../images/49.png":"../../images/49c.png"
    }

    Image {
        id: positionEnd
        anchors.verticalCenter: startImage.verticalCenter
        anchors.horizontalCenter: positionImage.right
        visible: buttons.isPlay||buttons.isPause
        width: Core.dp(8)
        height: width
        source: "../../images/49.png"
    }

    Text {
        id: positionText
        anchors.right: parent.right
        anchors.rightMargin: playerNameText.anchors.leftMargin
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