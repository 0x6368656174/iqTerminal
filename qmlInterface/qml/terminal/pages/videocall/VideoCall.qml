import QtQuick 2.2
import ".."
import "../../elements"
import "../../videocall"
import "../chat"

Page {
    property int userId
    property real speakerVolume: {
        var volume = 1 - (speakerSlideItem.y - speakerLevelBack.y) / (speakerLevelBack.height - speakerSlideItem.height)
        if (volume < 0)
            return 0
        if (volume > 1)
            return 1
        return volume
    }
    property real microphoneVolume: {
        var volume = 1 - (microSlideItem.y - microLevelBack.y) / (microLevelBack.height - microSlideItem.height)
        if (volume < 0)
            return 0
        if (volume > 1)
            return 1
        return volume
    }
    onMicrophoneVolumeChanged: console.log("Microphone Volume: " + microphoneVolume)
    onSpeakerVolumeChanged: console.log("Speaker Volume: " + speakerVolume)

    name: "videoCall"

    function start() {
        showRightPage(name)
        startCallButton.clicked()
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.8
    }

    Item {
        id: rotationItem
        anchors.centerIn: parent
        width: rotation === 0?parent.width:parent.height
        height: rotation === 0?parent.height:parent.width

        BackButton {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: (70 * applicationModel.settings.zoomFactor)
            onClicked: stopCallButton.clicked()
        }

        // ----------------------------------------------------------------------------------------------------------------------
        // Camera IN image - входное изображение, получаемое с движка. Можно создавать сколько удобно (не мешают друг другу)
        // ----------------------------------------------------------------------------------------------------------------------
        CameraIn {
            id: cameraIn
            width: bottomFlipable.flipped?bottomItem.width:topItem.width
            height: bottomFlipable.flipped?bottomItem.height:topItem.height
            Behavior on width {NumberAnimation {duration: 200}}
            Behavior on height {NumberAnimation {duration: 200}}
            visible: false
            color: "transparent"
        }

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
            width: !bottomFlipable.flipped?bottomItem.width:topItem.width
            height: !bottomFlipable.flipped?bottomItem.height:topItem.height
            Behavior on width {NumberAnimation {duration: 200}}
            Behavior on height {NumberAnimation {duration: 200}}

            //TODO исправиль работу CameraViewFinder при visible: false, убрать это и добавить visible: false
            x:100000
            y:100000
            color: "transparent"
            cameraInterval: 10 // количество передаваемых кадров в секунду
            cameraWidth: 640 // ширина картинки с камеры
            cameraHeight: 480 // высота картинки с камеры
            cameraQuality: 10 // нечуствительность к изменениям в картинке 5-15

            //Красный квадратик для тестирования переключения экранов.
            //В релизе необходим удалить!!!
            //TODO удалить в релизе
            Rectangle {
                width: 30
                height: 30
                anchors.centerIn: parent
                color: "red"
            }
        }

        Item {
            id: topItem
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: bottomItem.top
            anchors.bottomMargin: 20 * applicationModel.settings.zoomFactor

            Flipable {
                id: topFlipable
                anchors.fill: parent
                front: ShaderEffectSource {
                    anchors.fill: topFlipable
                    sourceItem: cameraIn
                }

                back: ShaderEffectSource {
                    anchors.fill: topFlipable
                    sourceItem: cameraViewFinder
                }

                transform: Rotation {
                    id: topFlipableRotation
                    origin.x: topFlipable.width/2
                    origin.y: topFlipable.height/2
                    axis.x: 0;
                    axis.y: 1;
                    axis.z: 0
                    angle: 0
                }

                states: State {
                    name: "back"
                    PropertyChanges { target: topFlipableRotation; angle: 180 }
                    when: bottomFlipable.flipped
                }

                transitions: Transition {
                    id: topFlipableTransition
                    NumberAnimation { target: topFlipableRotation; property: "angle"; duration: 500 }
                }
            }
        }

        Item {
            id: bottomItem
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: buttonRow.top
            width: 160 * applicationModel.settings.zoomFactor
            height: 120 * applicationModel.settings.zoomFactor

            Flipable {
                id: bottomFlipable
                property bool flipped: false

                anchors.fill: parent

                front: ShaderEffectSource {
                    anchors.fill: bottomFlipable
                    sourceItem: cameraViewFinder
                }

                back: ShaderEffectSource {
                    anchors.fill: bottomFlipable
                    sourceItem: cameraIn
                }

                transform: Rotation {
                    id: bottomFlipableRotation
                    origin.x: bottomFlipable.width/2
                    origin.y: bottomFlipable.height/2
                    axis.x: 0;
                    axis.y: 1;
                    axis.z: 0
                    angle: 0
                }

                states: State {
                    name: "back"
                    PropertyChanges { target: bottomFlipableRotation; angle: 180 }
                    when: bottomFlipable.flipped
                }

                transitions: Transition {
                    id: bottomFlipableTransition
                    NumberAnimation { target: bottomFlipableRotation; property: "angle"; duration: 500 }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: bottomFlipable.flipped = !bottomFlipable.flipped
                }
            }

            Image {
                anchors.fill: parent
                source: "image://xml/" + applicationModel.aboutMe.userProfile.source
                fillMode: Image.PreserveAspectFit
                cache: false
                visible: cameraViewFinder.state === 0
            }
        }

        Item {
            id: microLevelControl
            anchors.left: parent.left
            anchors.leftMargin: anchors.bottomMargin
            anchors.bottom: buttonRow.top
            anchors.bottomMargin: 10 * applicationModel.settings.zoomFactor

            Image {
                id: microLevelBack
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: 20 * applicationModel.settings.zoomFactor
                height: 100 * applicationModel.settings.zoomFactor
                source: "../../images/209.png"
                MouseArea {
                    id: microMA
                    anchors.fill: parent
                    onPressed: {
                        microSlideItem.y = mapToItem(microSlideItem.parent, mouse.x, mouse.y).y
                    }
                    drag.target: microSlideItem
                    drag.axis: Drag.YAxis
                    drag.minimumY: microLevelBack.y
                    drag.maximumY: microLevelBack.y + microLevelBack.height - microSlideItem.height
                }
            }

            Item {
                id: microSlideItem
                anchors.horizontalCenter: microLevelBack.horizontalCenter
                y: microLevelBack.y
                width: 20 * applicationModel.settings.zoomFactor
                height: 2.4 * applicationModel.settings.zoomFactor
                Image {
                    id: microSlideImage
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    source: "../../images/210.png"
                    scale: microMA.pressed?1.2:1
                }
            }

            Item {
                anchors.left: microLevelBack.right
                anchors.leftMargin: 2 * applicationModel.settings.zoomFactor
                anchors.bottom: microLevelBack.bottom
                height: 20 * applicationModel.settings.zoomFactor
                width: height
                Image {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    scale: microMuteMa.pressed?1.2:1
                    source: "../../images/211.png"
                }
                MouseArea {
                    id: microMuteMa
                    anchors.fill: parent
                    onClicked: microSlideItem.y = microLevelBack.y + microLevelBack.height - microSlideItem.height
                }
            }
        }

        Item {
            id: speakerLevelControl
            anchors.right: parent.right
            anchors.rightMargin: anchors.bottomMargin
            anchors.bottom: buttonRow.top
            anchors.bottomMargin: 10 * applicationModel.settings.zoomFactor

            Image {
                id: speakerLevelBack
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                mirror: true
                width: 20 * applicationModel.settings.zoomFactor
                height: 100 * applicationModel.settings.zoomFactor
                source: "../../images/209.png"
                MouseArea {
                    id: speakerMA
                    anchors.fill: parent
                    drag.target: speakerSlideItem
                    drag.axis: Drag.YAxis
                    drag.minimumY: speakerLevelBack.y
                    drag.maximumY: speakerLevelBack.y + speakerLevelBack.height - speakerSlideItem.height
                    onPressed: {
                        speakerSlideItem.y = mapToItem(speakerSlideItem.parent, mouse.x, mouse.y).y
                    }
                }
            }

            Item {
                id: speakerSlideItem
                anchors.horizontalCenter: speakerLevelBack.horizontalCenter
                y: speakerLevelBack.y
                width: 20 * applicationModel.settings.zoomFactor
                height: 2.4 * applicationModel.settings.zoomFactor
                Image {
                    id: speakerSlideImage
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    source: "../../images/210.png"
                    scale: speakerMA.pressed?1.2:1
                }
            }

            Item {
                anchors.right: speakerLevelBack.left
                anchors.leftMargin: 2 * applicationModel.settings.zoomFactor
                anchors.bottom: speakerLevelBack.bottom
                height: 20 * applicationModel.settings.zoomFactor
                width: height
                Image {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.height
                    scale: speakerMuteMa.pressed?1.2:1
                    source: "../../images/212.png"
                }
                MouseArea {
                    id: speakerMuteMa
                    anchors.fill: parent
                    onClicked: speakerSlideItem.y = speakerLevelBack.y + speakerLevelBack.height - speakerSlideItem.height
                }
            }
        }


        Rectangle {
            id: buttonRow
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: (22 * applicationModel.settings.zoomFactor)
            color: "#b4b4b4"
            clip: true
            ChatButton {
                id: addButton
                width: (parent.width - 3)/6
                height: parent.height
                anchors.left: parent.left
                imageNumber: 200
                onClicked: addButton.checked = !addButton.checked
            }
            ChatButton {
                id: stopCallButton
                width: (parent.width - 3)/6
                height: parent.height
                anchors.left: addButton.right
                imageNumber: 213
                onClicked: {
                    console.log("Stop Call Clicked")
                    proxy.command("video-",userId)
                    cameraViewFinder.cameraStopSource()
                }
            }
            Item {
                id: subRows
                width: (parent.width - 2)/2
                anchors.left: stopCallButton.right
                anchors.leftMargin: 1
                height: buttonRow.height * 2
                anchors.top: parent.top
                anchors.topMargin: !addButton.checked?0:-buttonRow.height
                Behavior on anchors.topMargin { NumberAnimation {duration: 200} }

                Row {
                    id: firstButtonSubRow
                    anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.left: parent.left
                    width: parent.width
                    height: buttonRow.height
                    spacing: 1
                    visible: subRows.anchors.topMargin !== -buttonRow.height
                    ChatButton {
                        id: cameraEnableButton
                        width: (parent.width - 2)/3
                        height: parent.height
                        imageNumber: 214
                        onClicked: {
                            proxy.command("video",userId)
                            cameraViewFinder.cameraStartCameraSource()
                        }
                    }
                    ChatButton {
                        id: cameraDisableButton
                        width: (parent.width - 2)/3
                        height: parent.height
                        imageNumber: 215
                        onClicked: {
                            proxy.command("video-",userId)
                            cameraViewFinder.cameraStopSource()
                        }
                    }
                    ChatButton {
                        id: desktopButton
                        width: (parent.width - 2)/3
                        height: parent.height
                        imageNumber: 219
                        onClicked: console.log("Desktop button clicked")
                    }
                }
                Row {
                    id: secondButtonSubRow
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.left: parent.left
                    width: parent.width
                    height: buttonRow.height
                    spacing: 1
                    visible: subRows.anchors.topMargin !== 0
                    ChatButton {
                        id: changeCamButton
                        width: (parent.width - 1)/2
                        height: parent.height
                        imageNumber: 217
                        onClicked: {
                            console.log("Change camera button clicked")
                            cameraViewFinder.cameraNextCamera()
                        }
                    }
                    ChatButton {
                        id: changeMicroButton
                        width: (parent.width - 1)/2
                        height: parent.height
                        imageNumber: 218
                        onClicked: console.log("Change microphone button clicked")
                    }
                }
            }
            ChatButton {
                id: startCallButton
                height: parent.height
                anchors.left: subRows.right
                anchors.leftMargin: 1
                anchors.right: parent.right
                imageNumber: 216
                onClicked: console.log("Start Call Clicked")
            }
        }

        Button {
            id: rotateButton
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: (26 * applicationModel.settings.zoomFactor)
            width: (48 * applicationModel.settings.zoomFactor)
            source: "../../images/203.png"

            onClicked: {
                if(rotationItem.rotation === 0)
                    rotationItem.rotation = 90
                else
                    rotationItem.rotation = 0
            }
        }
    }
}

