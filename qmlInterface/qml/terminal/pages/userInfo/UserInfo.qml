import QtQuick 2.0
import TerminalQmlPlugin 1.0
import ".."
import "pages"
import "../../elements"


Page {
    id: userInfoPage
    property string userProfile
    property bool readOnly: false
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

    UserProfile {
        id: userProfileModel
        source: userInfoPage.userProfile !==""?Core.dataDir + "users/" + userInfoPage.userProfile:""
        parentElement: "user/info"

        stateModel.itemAdditionalData: QtObject {
            property bool isEdited: false
            property string nameToSave: ""
            property string textToSave: ""
        }
    }

    FoldersModel {
        id: photoFolderModel
        source: userInfoPage.userProfile !==""?Core.dataDir + "users/" + userInfoPage.userProfile:""
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
        source: userInfoPage.userProfile !==""?Core.dataDir + "users/" + userInfoPage.userProfile:""
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
        source: userInfoPage.userProfile !==""?Core.dataDir + "users/" + userInfoPage.userProfile:""
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
