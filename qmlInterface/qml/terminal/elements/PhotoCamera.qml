import QtQuick 2.0
import QtMultimedia 5.2
import "../pages"

Page {
    id: videoPlayer
    Rectangle {
        anchors.fill: parent
        color: "#6c676e"
    }

    name: "photo_camera"

    signal hiden()

    onHiden: opacity = 0

    function show() {
        opacity = 1
    }

    Behavior on opacity {NumberAnimation {duration: 200} }
    visible: opacity !== 0
    opacity: 0

    Camera {
        id: camera
    }

    VideoOutput {
        anchors.fill: parent
        source: camera
    }
}
