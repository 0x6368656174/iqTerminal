import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../pages"

Page {
    id: photoSlider
    property int folderIndex: -1
    property int photoIndex: -1
    property var photosModel

    width: rotation === 0?parent.width:parent.height
    height: rotation === 0?parent.height:parent.width

    onOpacityChanged: {
        if (opacity !== 0) {
            var oldFolderIndex = folderIndex
            var oldPhotoIndex = photoIndex
            folderIndex = -1
            photoIndex = -1
            photoIndex = oldPhotoIndex
            folderIndex = oldFolderIndex
        }
    }

    signal hiden()

    name: "photo_slider"

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
                source:  {
                    if (folderIndex !== -1 && photoIndex !== -1 && photosModel.get(folderIndex).filesModel.count > 0) {
                        if(photosModel.get(folderIndex).filesModel.get(photoIndex))
                            return photosModel.get(folderIndex).filesModel.get(photoIndex).path
                    }
                    return ""
                }
                fillMode: Image.PreserveAspectFit
            }
        }

        TerminalMouseArea {
            anchors.fill: parent
            onClicked: photoSlider.hiden()
            onPressAndHold: {}
        }

        Button {
            id: previousFolderButton
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.leftMargin: Core.dp(10)
            anchors.topMargin: Core.dp(5)
            width: Core.dp(13)
            height: Core.dp(9)
            source: "../images/41.png"

            onClicked: {
                if (folderIndex > 0) {
                    folderIndex--
                }
            }
        }

        MouseArea {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: previousFolderButton.right
            anchors.rightMargin: -Core.dp(12)
            anchors.bottom: folderCounter.verticalCenter
            onClicked: previousFolderButton.clicked()
        }

        Text {
            id: folderCounter
            anchors.horizontalCenter: previousFolderButton.horizontalCenter
            anchors.top: previousFolderButton.bottom
            anchors.topMargin: Core.dp(2)
            color: "white"
            font.pixelSize: Core.dp(8)
            text: photosModel?(folderIndex + 1) + "/" + photosModel.count:""
        }

        Text {
            id: folderName
            anchors.verticalCenter: folderCounter.verticalCenter
            anchors.left: folderCounter.right
            anchors.leftMargin: Core.dp(2)
            anchors.right: parent.right
            anchors.rightMargin: Core.dp(8)
            color: "white"
            font.pixelSize: Core.dp(8)
            elide: Text.ElideRight
            text: folderIndex !== -1?photosModel.get(folderIndex).name:""
        }

        Button {
            id: nextFolderButton
            anchors.horizontalCenter: previousFolderButton.horizontalCenter
            anchors.top: folderCounter.bottom
            anchors.topMargin: Core.dp(2)
            width: Core.dp(13)
            height: Core.dp(9)
            source: "../images/41c.png"

            onClicked: {
                if (folderIndex < photosModel.count - 1) {
                    folderIndex++
                }
            }
        }

        MouseArea {
            anchors.left: parent.left
            anchors.top: folderCounter.verticalCenter
            anchors.right: nextFolderButton.right
            anchors.rightMargin: -Core.dp(12)
            anchors.bottom: parent.bottom
            onClicked: nextFolderButton.clicked()
        }

        Text {
            id: photoName
            anchors.left: previousPhotoButton.left
            anchors.bottom: previousPhotoButton.top
            anchors.bottomMargin: Core.dp(2)
            color: "white"
            font.pixelSize: Core.dp(8)
            text: {
                if (folderIndex !== -1 && photoIndex !== -1 && photosModel.get(folderIndex).filesModel.count > 0) {
                    if (photosModel.get(folderIndex).filesModel.get(photoIndex))
                        return photosModel.get(folderIndex).filesModel.get(photoIndex).name
                }
                return ""
            }
        }

        Button {
            id: previousPhotoButton
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: Core.dp(10)
            anchors.bottomMargin: Core.dp(5)
            height: Core.dp(13)
            width: Core.dp(9)
            source: "../images/41a.png"

            onClicked: {
                if (photoIndex > 0) {
                    photoIndex--
                }
            }
        }

        MouseArea {
            anchors.left: parent.left
            anchors.right: photoCounter.horizontalCenter
            anchors.top: previousPhotoButton.top
            anchors.topMargin: -Core.dp(12)
            anchors.bottom: parent.bottom
            onClicked: previousPhotoButton.clicked()
        }

        Text {
            id: photoCounter
            anchors.verticalCenter: previousPhotoButton.verticalCenter
            anchors.left: previousPhotoButton.right
            anchors.leftMargin: Core.dp(2)
            color: "white"
            font.pixelSize: Core.dp(8)
            text: {
                if (folderIndex !== -1 && photoIndex !== -1 && photosModel.get(folderIndex).filesModel.count > 0) {
                    return (photoIndex + 1) + "/" +  photosModel.get(folderIndex).filesModel.count
                }
                return "0/0"
            }
        }

        Button {
            id: nextPhotoButton
            anchors.verticalCenter: previousPhotoButton.verticalCenter
            anchors.left: photoCounter.right
            anchors.leftMargin: Core.dp(2)
            height: Core.dp(13)
            width: Core.dp(9)
            source: "../images/41b.png"

            onClicked: {
                if (photoIndex < photosModel.get(folderIndex).filesModel.count - 1) {
                    photoIndex++
                }
            }
        }

        MouseArea {
            anchors.left: photoCounter.horizontalCenter
            anchors.right: parent.right
            anchors.top: nextPhotoButton.top
            anchors.topMargin: -Core.dp(12)
            anchors.bottom: parent.bottom
            onClicked: nextPhotoButton.clicked()
        }

        Button {
            id: rotateButton
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            height: Core.dp(26)
            width: Core.dp(48)
            source: "../images/203.png"

            onClicked: {
                if(photoSlider.rotation === 0)
                    photoSlider.rotation = 90
                else
                    photoSlider.rotation = 0
            }
        }
    }
}
