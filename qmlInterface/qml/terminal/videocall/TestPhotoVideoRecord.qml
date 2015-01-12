import QtQuick 2.4
import QtMultimedia 5.4

Rectangle {
    x:0
    y:0
    width:800
    height:600

    // ----------------------------------------------------------------------------------------------------------------------
    // Camera recorder (Photo/Video) - Еще в разработке, надо дождаться релиза Qt5.4.
    //      У меня работает фото и видео на Android. Фото на Windows. Фото на Linux.
    // ----------------------------------------------------------------------------------------------------------------------
    // cameraRec.cameraNextCamera(); // переключить камеру на следующую
    // cameraRec.cameraSetMode(0); // установить режим фотографирования
    // cameraRec.cameraSetMode(1); // установить режим видеозаписи
    // cameraRec.cameraClick(); // клик - в режиме фото делает фото, в режиме видео первое нажатие включает запись, второе нажатие останавливает запись
    CameraRec {
        id: cameraRec
        x: 0
        y: 0
        width: 320
        height: 240
        //mode: 0 // 0 - photo, 1 - video (по умолчанию режим фото)
        pathNewImageFile: "d:/test.jpg" // Windows
        //pathNewImageFile: "/home/bfg/test.jpg" //Linux
        //pathNewImageFile: "/home/bfg/test.jpg" // Android
        pathNewVideoFile: "file:///d:/test.mp4" // Windows
        //pathNewVideoFile: "/home/bfg/test.mp4" // Linux
        onImageSaved: { // Сигнал отрабатывает после получения Фото
            photoPreview.source = "";
            photoPreview.source = "file:///%1".arg(pathImageSaved); // вариант для десктопов (Windows, Linux)
            //photoPreview.source = pathImageSaved; // вариант для планшета (Android)
        }
        onVideoSaved: { // Сигнал отрабатывает после остановки записи видео (при ее успешном начале :-D , а пока работает только на Android)
            videoPreview.source = "";
            videoPreview.source = pathVideoSaved;
            //videoPreview.visible = true;
        }
    }


    // ----------------------------------------------------------------------------------------------------------------------
    // пример просмотра итоговой фотки
    // ----------------------------------------------------------------------------------------------------------------------
    Rectangle {
        id: photoPreview
        z: 5
        x: 350
        y: 0
        width: 320
        height: 240
        visible: true
        property alias source : previewImage.source
        color: "silver"
        Image {
            id: previewImage
            anchors.fill : parent
            asynchronous: true
            cache: false
            smooth: true
            fillMode: Image.PreserveAspectFit
        }
    }

    // ----------------------------------------------------------------------------------------------------------------------
    // пример просмотра итогового видео
    // ----------------------------------------------------------------------------------------------------------------------
    Rectangle {
        id: videoPreview
        z: 10
        x: 350
        y: 250
        width: 320
        height: 240
        visible: true
        color: "silver"
        //property string source: ""
        property alias source : playerVideo.source
        MediaPlayer {
            id: playerVideo
            autoPlay: true
            onStatusChanged: {
                if (status == MediaPlayer.EndOfMedia)
                {
                    videoPreview.closed();
                }
            }
        }
        VideoOutput {
            source: playerVideo
            anchors.fill : parent
            autoOrientation: true
        }
    }

}

