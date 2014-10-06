import QtQuick 2.0
import TerminalQmlPlugin 1.0
import ".."
import "pages"
import "../../elements"


Page {
    id: userInfoPage
    property UserInfo userInfo
    property bool readOnly: false

    Component {
        id: userProfileStateModelAdditionalData
        QtObject {
            property bool isEdited: false
            property string nameToSave: ""
            property string textToSave: ""
        }
    }

    Component {
        id: photosModelFolderAdditionalData
        QtObject {
            property bool isEdited: false
            property bool collapsed: false
            property string nameToSave: ""
        }
    }
    Component {
        id: photosModelFileAdditionalData
        QtObject {
            property bool isEdited: false
            property string nameToSave: ""
        }
    }

    Component {
        id: videosModelFolderAdditionalData
        QtObject {
            property bool isEdited: false
            property bool collapsed: false
            property string nameToSave: ""
        }
    }
    Component {
        id: videosModelFileAdditionalData
        QtObject {
            property bool isEdited: false
            property string nameToSave: ""
        }
    }

    Component {
        id: musicsModelFolderAdditionalData
        QtObject {
            property bool isEdited: false
            property bool collapsed: false
            property string nameToSave: ""
        }
    }
    Component {
        id: musicsModelFileAdditionalData
        QtObject {
            property bool isEdited: false
            property bool isPlaying: false
            property string nameToSave: ""
        }
    }

    onUserInfoChanged: {
        header.activeTab = "info"

        userInfo.userProfile.stateModel.itemAdditionalData = userProfileStateModelAdditionalData
        userInfo.photosModel.folderAdditionalData = photosModelFolderAdditionalData
        userInfo.photosModel.fileAdditionalData = photosModelFileAdditionalData
        userInfo.videosModel.folderAdditionalData = videosModelFolderAdditionalData
        userInfo.videosModel.fileAdditionalData = videosModelFileAdditionalData
        userInfo.musicsModel.folderAdditionalData = musicsModelFolderAdditionalData
        userInfo.musicsModel.fileAdditionalData = musicsModelFileAdditionalData
    }

    name: "userInfo"

    Header {
        id: header
        activeTab: "info"
    }

    BackButton {
        id: userInfoPageBackButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: (70 * applicationModel.settings.zoomFactor)
    }

    Item {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Info {
            id: infoPage
            opacity: header.activeTab === "info"?1:0
            visible: opacity !== 0
            readOnly: userInfoPage.readOnly
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        Photo {
            id: photoPage
            opacity: header.activeTab === "photo"?1:0
            visible: opacity !== 0
            readOnly: userInfoPage.readOnly
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        Video {
            id: videoPage
            opacity: header.activeTab === "video"?1:0
            visible: opacity !== 0
            readOnly: userInfoPage.readOnly
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        Music {
            id: mucisPage
            opacity: header.activeTab === "music"?1:0
            visible: opacity !== 0
            readOnly: userInfoPage.readOnly
            Behavior on opacity {NumberAnimation {duration: 300} }
        }
    }

    PhotoSlider {
        id: photoSlider
        photosModel: userInfo?userInfo.photosModel:undefined
        opacity: 0
        anchors.centerIn: parent
        visible: opacity > 0
        onHiden: opacity = 0

        Behavior on opacity { NumberAnimation { duration: 200; } }
        z: 2
    }

    VideoPlayer {
        id: videoPlayer
        playlist: {
            var result = []
            if (!userInfo)
                return result
            for (var i = 0; i < userInfo.videosModel.count; i++) {
                var folder = userInfo.videosModel.get(i)
                for (var j = 0; j < folder.filesModel.count; j++) {
                    var playlistItem = [folder.filesModel.get(j).name, folder.filesModel.get(j).path]
                    result.push(playlistItem)
                }
            }
            return result
        }

        onIsPlayChanged: {
            if (isPlay)
                audioPlayer.stop()
        }

        opacity: 0
        anchors.centerIn: parent
        visible: opacity > 0
        onHiden: opacity = 0
        onVisibleChanged: userInfoPageBackButton.visible = !visible

        Behavior on opacity { NumberAnimation { duration: 200; } }
        z: 2
    }

    Connections {
        target: audioPlayer
        onIsPlayChanged: {
            if (audioPlayer.isPlay)
                videoPlayer.stop()
        }
    }


}
