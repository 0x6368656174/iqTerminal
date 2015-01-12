import QtQuick 2.4

//320х240
Rectangle {
    width: 320
    height: 240
    clip: true
    Image {
        anchors.margins: 2
        anchors.fill: parent
        id: selfImage
        asynchronous: false //true
        cache: true //false
        source: "image://cameraImageProvider/CurrentCameraIn"
        smooth: true
        fillMode: Image.PreserveAspectFit
        Connections {
            target: engineVideoCall
            ignoreUnknownSignals : true
            onRepaintCurrentCameraInSelfSignalQml: {
                selfImage.source = "image://cameraImageProvider/CurrentCameraIn" + Qt.formatDateTime(new Date(), "yyyyMMdd hh:mm:ss.zzz")
            }
        }
        Component.onCompleted: { selfImage.source = "image://cameraImageProvider/CurrentCameraIn" + Qt.formatDateTime(new Date(), "yyyyMMdd hh:mm:ss.zzz") }
    }

}

