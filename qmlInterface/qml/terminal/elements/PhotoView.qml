import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../pages"

Page {
    id: photoView
    property url source
    property string text

    signal hiden()

    onHiden: opacity = 0

    function show() {
        opacity = 1
    }

    width: rotation === 0?parent.width:parent.height
    height: rotation === 0?parent.height:parent.width

    name: "photo_view"

    Behavior on opacity {NumberAnimation {duration: 200} }
    visible: opacity !== 0
    opacity: 0

    Rectangle {
        anchors.fill: parent
        color: "#6c676e"
        Item {
            anchors.centerIn: parent
            width: photoImage.sourceSize.width > parent.width? parent.width:photoImage.sourceSize.width
            height: photoImage.sourceSize.height > parent.height? parent.height:photoImage.sourceSize.height
            Image {
                id: photoImage
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: photoView.source
            }
        }

        TerminalMouseArea {
            anchors.fill: parent
            onClicked: photoView.hiden()
            onPressAndHold: {}
        }

        Text {
            id: photoName
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: Core.dp(8)
            color: "white"
            font.pixelSize: Core.dp(8)
            clip: true
            elide: Text.ElideRight
            text: photoView.text
        }

        Button {
            id: rotateButton
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: Core.dp(26)
            width: Core.dp(48)
            source: "../images/203.png"

            onClicked: {
                if(photoView.rotation === 0)
                    photoView.rotation = 90
                else
                    photoView.rotation = 0
            }
        }
    }
}
