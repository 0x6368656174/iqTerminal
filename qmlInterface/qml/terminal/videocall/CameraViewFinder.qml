import QtQuick 2.4
import QtMultimedia 5.4

Rectangle {
    id: qmlCameraViewFinder
    property int cameraInterval: 10 // количество передаваемых кадров в секунду
    property int cameraWidth: 640 // ширина картинки с камеры
    property int cameraHeight: 480 // высота картинки с камеры
    property real cameraQuality: 10 // нечуствительность к изменениям в картинке 5-15
    property bool cameraStartFromBeginFlag: false // флаг разовой отправки следующим фреймом полной картинки
    property int cameraState: 0 // состояние камеры 0-ожидание, 1-передача картинки с камеры, 2-передача картинки с экрана
    property real coefSizeCameraViewFinder: 1
    width: 320
    height: 240
    clip: true
    Binding {
        target: qmlCameraViewFinder
        property: "coefSizeCameraViewFinder"
        value: ((qmlCameraViewFinder.height/qmlCameraViewFinder.cameraHeight)<(qmlCameraViewFinder.width/qmlCameraViewFinder.cameraWidth) ? (qmlCameraViewFinder.height/qmlCameraViewFinder.cameraHeight) : (qmlCameraViewFinder.width/qmlCameraViewFinder.cameraWidth))
    }
    function cameraInit() {
        timerViewFinder.stop();
        camera.stop();
        engineVideoCall.setCameraObject(camera);
        engineVideoCall.setSelectedCameraDevice(0); // current
        viewfinder.source = camera;
        camera.start();
        timerViewFinder.start();
    }
    function cameraNextCamera() {
        timerViewFinder.stop();
        camera.stop();
        engineVideoCall.setSelectedCameraDevice(1); // next
        camera.start();
        timerViewFinder.start();
    }
    function cameraStartFromBegin() { cameraStartFromBeginFlag = true; }
    function cameraStartCameraSource() { cameraState = 1; }
    function cameraStartScreenSource() { cameraState = 2; }
    function cameraStopSource() { cameraState = 0; }
    Camera { id: camera }
    VideoOutput {
        anchors.margins: 2
        anchors.centerIn: parent
        width: cameraWidth*coefSizeCameraViewFinder-viewfinder.anchors.margins*2
        height: cameraHeight*coefSizeCameraViewFinder-viewfinder.anchors.margins*2
        id: viewfinder
        source: camera
        autoOrientation: true
        Component.onCompleted: { cameraInit(); }
    }
    Timer {
        property int priorState: 0
        id: timerViewFinder
        repeat: false
        running: false
        triggeredOnStart: false
        interval: 1000/cameraInterval //100
        onTriggered: {
            if(viewfinder.visible)
            {
                if(priorState!=cameraState)
                {
                    cameraStartFromBeginFlag = true;
                    priorState = cameraState;
                    if(priorState===0) engineVideoCall.stopCameraOutSlot();
                }
                if(priorState>0)
                {
                    if(cameraStartFromBeginFlag){cameraStartFromBeginFlag=false; engineVideoCall.startCurCameraOutFromBeginSlot();}
                    if(priorState===1) viewfinder.grabToImage(function(result) { engineVideoCall.updateCurCameraOutSlot(result.image,cameraQuality);},Qt.size(cameraWidth, cameraHeight));
                    if(priorState===2) engineVideoCall.updateCurCameraOutSlot();
                }
            }
            timerViewFinder.start();
        }
    }
}

