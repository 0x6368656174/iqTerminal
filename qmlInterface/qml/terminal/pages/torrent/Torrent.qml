import QtQuick 2.0
import TerminalQmlPlugin 1.0
import ".."
import "pages"
import "../../elements"


Page {
    id: torrentPage
    name: "torrent"

    Header {
        id: header
        activeTab: "search"
    }

    BackButton {
        id: torrentPageBackButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: (70 * applicationModel.settings.zoomFactor)
    }

    Item {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        TorrentSearch {
            id: torrentSearch
            opacity: header.activeTab === "search"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        TorrentDownload {
            id: torrentDownload
            opacity: header.activeTab === "download"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        TorrentsUpload {
            id: torrentUpload
            opacity: header.activeTab === "upload"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }
    }
}
