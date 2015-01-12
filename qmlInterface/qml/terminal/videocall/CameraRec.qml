import QtQuick 2.4
import QtMultimedia 5.4


Rectangle {
    //    width: 800
    //    height: 600
    //    x: 0
    //    y: 0
    //    color: "silver"

    id: cameraRec
    property int mode: 0 // 0 - photo, 1 - video
    property string pathNewImageFile: ""
    property string pathNewVideoFile: ""
    property bool deviceAvaliable: true
    property string pathImageSaved: ""
    property string pathVideoSaved: ""

    signal imageSaved()
    signal videoSaved()

    function cameraSetMode(initMode) {
        camera.stop();
        mode = initMode;
        camera.captureMode = (mode==0)?Camera.CaptureStillImage:Camera.CaptureVideo
        camera.start();
    }
    function cameraInit() {
        engineVideoCall.setCameraObject(camera);
        //camera.stop();
        engineVideoCall.setSelectedCameraDevice(0); // current
        viewfinder.source = camera;
        camera.captureMode = (mode==0)?Camera.CaptureStillImage:Camera.CaptureVideo
        camera.start();
    }
    function cameraNextCamera() {
        camera.stop();
        engineVideoCall.setSelectedCameraDevice(1); // next
        camera.captureMode = (mode==0)?Camera.CaptureStillImage:Camera.CaptureVideo
        camera.start();
    }
    function cameraClick() {
        if(cameraRec.mode == 0)
        {
            camera.imageCapture.captureToLocation(pathNewImageFile); // работает
            return;
        }
        if(cameraRec.mode == 1)
        {
             if(camera.videoRecorder.recorderStatus == CameraRecorder.RecordingStatus)
             {
                 camera.videoRecorder.stop();
                 //camera.stop();
                 pathVideoSaved=camera.videoRecorder.actualLocation;
                 videoSaved();
             } else {
                 camera.videoRecorder.outputLocation = pathNewVideoFile; //"file:///%1".arg(pathNewFile);
                 camera.videoRecorder.record();
             }
             return;
        }
    }

    // ----------------------------------------------------------------------------------------------------------------------
    // camera
    // ----------------------------------------------------------------------------------------------------------------------
    Camera {
        id: camera
        //captureMode: (mode==0)?Camera.CaptureStillImage:Camera.CaptureVideo
        imageCapture {
            //resolution: Qt.size(640, 480) //Qt.size(320, 240)
            onImageSaved: {
                pathImageSaved = path;
                imageSaved();
            }
        }
        //videoRecorder.resolution: Qt.size(640, 480) //"640x480"
        //videoRecorder.audioEncodingMode: CameraRecorder.ConstantBitrateEncoding;
        videoRecorder.audioBitRate: 128000
        //videoRecorder.mediaContainer: "mkv"
        //videoRecorder.mediaContainer: "mp4"
        onError: {
            console.log(errorString);
        }
    }
    VideoOutput {
        id: viewfinder
        source: camera
        visible: true //cameraUI.state == "PhotoCapture" || cameraUI.state == "VideoCapture"
        anchors.fill: parent
        autoOrientation: true
        Component.onCompleted: { cameraInit(); }
    }

}

