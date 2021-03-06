import QtQuick 2.4
import QtQuick.Window 2.2
import QtMultimedia 5.4

Rectangle {
    id: qmlVideoCall
    x:0
    y:0
    width: 800
    height: 600

    // ----------------------------------------------------------------------------------------------------------------------
    // Camera view finder - одномоментно должен существовать в единственном экземпляре занимает устройство видеоввода (камеру)
    // ----------------------------------------------------------------------------------------------------------------------
    // Управление:
    // cameraViewFinder.cameraNextCamera(); // Переключиться на следующую камеру
    // cameraViewFinder.cameraStartCameraSource(); // Запустить передачу картинки с камеры в движок
    // cameraViewFinder.cameraStartScreenSource(); // Запустить передачу картинки с экрана в движок (Только на десктопах)
    // cameraViewFinder.cameraStopSource(); // Остановить передачу картинки в движок
    // cameraViewFinder.cameraStartFromBegin(); // Передать в движок ключевой кадр (цельную картинку)
    CameraViewFinder {
        id: cameraViewFinder
        x: 0
        y: 0
        width: 150
        height: 150
        color: "#bfbfbf"
        border.width: 2
        border.color: "white"
        cameraInterval: 10 // количество передаваемых кадров в секунду
        cameraWidth: 640 // ширина картинки с камеры
        cameraHeight: 480 // высота картинки с камеры
        cameraQuality: 10 // нечуствительность к изменениям в картинке 5-15
    }

    // ----------------------------------------------------------------------------------------------------------------------
    // Camera OUT image - выходное изображение (либо с камеры, либо с экрана десктопа). Можно создавать сколько удобно (не мешают друг другу)
    // ----------------------------------------------------------------------------------------------------------------------
    CameraOut {
        x: 0+10+150
        y: 0
        width: 300
        height: 300
        color: "#bfbfbf"
        border.width: 2
        border.color: "white"
    }

    // ----------------------------------------------------------------------------------------------------------------------
    // Camera IN image - входное изображение, получаемое с движка. Можно создавать сколько удобно (не мешают друг другу)
    // ----------------------------------------------------------------------------------------------------------------------
    CameraIn {
        x: 0+10+150
        y: 300
        width: 300
        height: 300
        color: "#bfbfbf"
        border.width: 2
        border.color: "white"
        visible: false
    }

}
