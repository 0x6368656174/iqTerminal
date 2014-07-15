import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtMultimedia 5.2
import ".."
import "pages"
import "../../elements"


Page {
    id: userInfoPage
    property string userProfile
    onUserProfileChanged: {
        header.activeTab = "info"
    }

    name: "userInfo"

    Header {
        id: header
        activeTab: "info"
    }

    BackButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: Core.dp(70)
    }

    FoldersModel {
        id: photoFolderModel
        source: userProfile
        parentElement: "user/photos"
        folderAdditionalData: QtObject {
            property bool isEdited: false
            property bool collapsed: false
            property string nameToSave: ""
        }
        fileAdditionalData: QtObject {
            property bool isEdited: false
            property string nameToSave: ""
        }
    }

    FoldersModel {
        id: videoFolderModel
        source: userProfile
        parentElement: "user/videos"
        folderAdditionalData: QtObject {
            property bool isEdited: false
            property bool collapsed: false
            property string nameToSave: ""
        }
        fileAdditionalData: QtObject {
            property bool isEdited: false
            property string nameToSave: ""
        }
    }

    FoldersModel {
        id: musicFolderModel
        source: userProfile
        parentElement: "user/music"
        folderAdditionalData: QtObject {
            property bool isEdited: false
            property bool collapsed: false
            property string nameToSave: ""
        }
        fileAdditionalData: QtObject {
            property bool isEdited: false
            property bool isPlaying: false
            property string nameToSave: ""
        }
    }

    Item {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Info {
            id: infoPage
            userProfile: userInfoPage.userProfile
            opacity: header.activeTab === "info"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        Photo {
            id: photoPage
            userProfile: userInfoPage.userProfile
            opacity: header.activeTab === "photo"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        Video {
            id: videoPage
            userProfile: userInfoPage.userProfile
            opacity: header.activeTab === "video"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        Music {
            id: mucisPage
            userProfile: userInfoPage.userProfile
            opacity: header.activeTab === "music"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }
    }

    PhotoSlider {
        id: photoSlider
        photosModel: photoFolderModel
        opacity: 0
        anchors.fill: parent
        visible: opacity > 0
        onHiden: opacity = 0

        Behavior on opacity { NumberAnimation { duration: 200; } }
        z: 2
    }

    VideoPlayer {
        id: videoPlayer

        opacity: 0
        anchors.fill: parent
        visible: opacity > 0
        onHiden: opacity = 0
        onIsPlayChanged: {
            if (isPlay)
                audioPlayer.stop()
        }

        Behavior on opacity { NumberAnimation { duration: 200; } }
        z: 2
    }

    Audio {
        id: audioPlayer
        property bool isPlay: false
        property bool isPause: false
        onIsPlayChanged: {
            if (isPlay)
                videoPlayer.stop()
        }

        onPlaying: {
            isPlay = true
            isPause = false
        }
        onPaused:  {
            isPlay = false
            isPause = true
        }
        onStopped: {
            isPlay = false
            isPause = false
        }
    }
}
