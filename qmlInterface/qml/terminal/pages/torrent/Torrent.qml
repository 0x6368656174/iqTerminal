import QtQuick 2.0
import TerminalQmlPlugin 1.0
import ".."
import "pages"
import "../../elements"


Page {
    id: torrentPage
    name: torrent
    property string torrentXmlPath: Core.dataDir + "/torrent.xml"

    Header {
        id: header
        activeTab: "search"
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

        TorrentDownload {
            id: torrentDownload
            torrentXmlPath: torrentPage.torrentXmlPath
            opacity: header.activeTab === "download"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }
    }
}
