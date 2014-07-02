import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"

Page {
    id: photoSlider
    property int folderId: -1
    property int photoId: -1
    property string iXmlPath

    function reload() {
        folderId = -1
        photoId = -1
        foldersModel.reload()
    }

    signal hiden()

    onFolderIdChanged: {
        privateData.currentFolderIndex = getFolderIndex(folderId)
    }

    onPhotoIdChanged:  {
        privateData.currentPhotoIndex = getPhotoIndex(photoId)
    }

    QtObject {
        id: privateData
        property int currentFolderIndex: -1
        onCurrentFolderIndexChanged: {
            if (currentFolderIndex !== -1) {
                folderId  = foldersModel.get(currentFolderIndex).folder_id
            }
        }

        property int currentPhotoIndex: -1
        onCurrentPhotoIndexChanged: {
            if (currentPhotoIndex !== -1) {
                photoId = childsModel.get(currentPhotoIndex).child_id
            }
        }
    }

    name: "photo_view"

    XmlListModel {
        id: foldersModel
        source: iXmlPath
        query: "/i/photos/folder"

        XmlRole {
            name: "folder_name"
            query: "name/string()"
        }
        XmlRole {
            name: "folder_id"
            query: "@id/string()"
            isKey: true
        }
    }

    XmlListModel {
        id: childsModel
        source: iXmlPath
        query: "/i/photos/folder[" + (privateData.currentFolderIndex + 1) + "]/photo"

        XmlRole {
            name: "child_name"
            query: "name/string()"
        }

        XmlRole {
            name: "child_file"
            query: "file/string()"
        }

        XmlRole {
            name: "child_id"
            query: "@id/string()"
            isKey: true
        }

        onStatusChanged: {
            if (status === XmlListModel.Ready) {
                if (photoId !== -1) {
                    privateData.currentPhotoIndex = -1
                    privateData.currentPhotoIndex = getPhotoIndex(photoId)
                } else if (folderId !== -1 && childsModel.count > 0) {
                    privateData.currentPhotoIndex = 0
                    photoId = childsModel.get(0).child_id
                }
            }
        }
    }

    function getFolderIndex(folderId) {
        for (var i = 0; i < foldersModel.count; i++) {
            if (parseInt(foldersModel.get(i).folder_id) === folderId)
                return i
        }
        return -1
    }

    function getPhotoIndex(photoId) {
        for (var i = 0; i < childsModel.count; i++) {
            if (parseInt(childsModel.get(i).child_id) === photoId)
                return i
        }
        return -1
    }

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
                source: {
                    if (privateData.currentPhotoIndex !== -1) {
                        return childsModel.get(privateData.currentPhotoIndex).child_file
                    } else {
                        return ""
                    }
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
            source: "../../../images/41.png"

            onClicked: {
                if (privateData.currentFolderIndex > 0) {
                    photoId = -1
                    privateData.currentFolderIndex--
                }
            }
        }

        Text {
            id: folderCounter
            anchors.horizontalCenter: previousFolderButton.horizontalCenter
            anchors.top: previousFolderButton.bottom
            anchors.topMargin: Core.dp(2)
            color: "white"
            font.pixelSize: Core.dp(8)
            text: (privateData.currentFolderIndex + 1) + "/" + foldersModel.count
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
            text: privateData.currentFolderIndex !== -1?foldersModel.get(privateData.currentFolderIndex).folder_name:""
        }

        Button {
            id: nextFolderButton
            anchors.horizontalCenter: previousFolderButton.horizontalCenter
            anchors.top: folderCounter.bottom
            anchors.topMargin: Core.dp(2)
            width: Core.dp(13)
            height: Core.dp(9)
            source: "../../../images/41c.png"

            onClicked: {
                if (privateData.currentFolderIndex < foldersModel.count - 1) {
                    photoId = -1
                    privateData.currentFolderIndex++
                }
            }
        }

        Text {
            id: photoName
            anchors.left: previousPhotoButton.left
            anchors.bottom: previousPhotoButton.top
            anchors.bottomMargin: Core.dp(2)
            color: "white"
            font.pixelSize: Core.dp(8)
            text: privateData.currentPhotoIndex !== -1?childsModel.get(privateData.currentPhotoIndex).child_name:""
        }

        Button {
            id: previousPhotoButton
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: Core.dp(10)
            anchors.bottomMargin: Core.dp(5)
            height: Core.dp(13)
            width: Core.dp(9)
            source: "../../../images/41a.png"

            onClicked: {
                if (privateData.currentPhotoIndex > 0) {
                    privateData.currentPhotoIndex--
                }
            }
        }

        Text {
            id: photoCounter
            anchors.verticalCenter: previousPhotoButton.verticalCenter
            anchors.left: previousPhotoButton.right
            anchors.leftMargin: Core.dp(2)
            color: "white"
            font.pixelSize: Core.dp(8)
            text: {
                if (childsModel.count > 0) {
                    return (privateData.currentPhotoIndex + 1) + "(" + childsModel.count + ")"
                }
                return "0(0)"
            }
        }

        Button {
            id: nextPhotoButton
            anchors.verticalCenter: previousPhotoButton.verticalCenter
            anchors.left: photoCounter.right
            anchors.leftMargin: Core.dp(2)
            height: Core.dp(13)
            width: Core.dp(9)
            source: "../../../images/41b.png"

            onClicked: {
                if (privateData.currentPhotoIndex < childsModel.count - 1) {
                    privateData.currentPhotoIndex++
                }
            }
        }
    }
}
