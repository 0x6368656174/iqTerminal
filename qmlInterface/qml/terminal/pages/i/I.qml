import QtQuick 2.0
import TerminalQmlPlugin 1.0
import ".."
import "pages"
import "../../elements"

Page {
    id:iPage
    name: "i"
    property string iXmlPath: Core.dataDir + "/i.xml"
    Header {
        id: header
        activeTab: "info"
    }

    BackButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: Core.dp(70)
    }

    Item {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

//        Image {
//            anchors.fill: parent
//            fillMode: Image.PreserveAspectCrop
//            source: "../../images/1a.png"
//        }

        IInfo {
            id: iInfo
            iXmlPath: iPage.iXmlPath
            opacity: header.activeTab === "info"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }

            onDataChanged: save()
        }

        IPhoto {
            id: iPhoto
            iXmlPath: iPage.iXmlPath
            opacity: header.activeTab === "photo"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }

            onDataChanged: save()
        }

        IVideo {
            id: iVideo
            iXmlPath: iPage.iXmlPath
            opacity: header.activeTab === "video"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }

            onDataChanged: save()
            onNeedStopAllPlayer: stopAllPlayer(pageName)
        }

        IMusic {
            id: iMusic
            iXmlPath: iPage.iXmlPath
            opacity: header.activeTab === "music"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }

            onDataChanged: save()
            onNeedStopAllPlayer: stopAllPlayer(pageName)
        }
    }

    FileIO {
        id: file
        source: iXmlPath
    }

    function save() {
        console.log("I::save()")
        var data = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        data = data + "<i>\n"
        data = data + iInfo.dataToSave()
        data = data + iPhoto.dataToSave()
        data = data + iVideo.dataToSave()
        data = data + iMusic.dataToSave()
        data = data + "</i>\n"

        file.write(data)

        iInfo.reload()
        iPhoto.reload()
        iVideo.reload()
        iMusic.reload()
    }

    function stopAllPlayer(pageName) {
        if (pageName !== iVideo.name)
            iVideo.stopPlay()
        if (pageName !== iMusic.name)
            iMusic.stopPlay()
    }
}
