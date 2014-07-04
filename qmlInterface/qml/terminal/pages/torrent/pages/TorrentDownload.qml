import QtQuick 2.0
import QtQuick.Dialogs 1.1
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"

Page {
    id: downloadPage
    name: "torrent_download"

    property string torrentXmlPath

    QtObject {
        id: privateData
        property bool isEdited: false
    }

    FoldersModel {
        id: folderModel
    }
}
