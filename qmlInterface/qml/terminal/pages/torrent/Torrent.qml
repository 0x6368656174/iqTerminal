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

        TorrentSearch {
            id: torrentSearch

            FoldersModel {
                id: torrentSearchModel
                source: torrentXmlPath
                parentElement: "torrent/upload"
                folderAdditionalData: QtObject {
                    property bool collapsed: false
                    property bool isEdited: false
                    property bool isSelect: false
                }

                fileAdditionalData: QtObject {
                    property bool isSelect: false
                }
            }

            torrentXmlPath: torrentPage.torrentXmlPath
            opacity: header.activeTab === "search"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        TorrentDownload {
            id: torrentDownload

            FoldersModel {
                id: torrentDownloadModel
                source: torrentXmlPath
                parentElement: "torrent/download"
                folderAdditionalData: QtObject {
                    property bool collapsed: false
                    property bool isEdited: false
                    property bool isSelect: false
                }

                fileAdditionalData: QtObject {
                    property bool isSelect: false
                }
            }

            torrentXmlPath: torrentPage.torrentXmlPath
            opacity: header.activeTab === "download"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }

        TorrentsUpload {
            id: torrentUpload

            FoldersModel {
                id: torrentUploadModel
                source: torrentXmlPath
                parentElement: "torrent/upload"
                folderAdditionalData: QtObject {
                    property bool collapsed: false
                    property bool isEdited: false
                    property bool isSelect: false
                }

                fileAdditionalData: QtObject {
                    property bool isSelect: false
                }
            }

            torrentXmlPath: torrentPage.torrentXmlPath
            opacity: header.activeTab === "upload"?1:0
            visible: opacity !== 0
            Behavior on opacity {NumberAnimation {duration: 300} }
        }
    }
}
