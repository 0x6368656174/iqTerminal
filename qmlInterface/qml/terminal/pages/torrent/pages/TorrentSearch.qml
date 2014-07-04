import QtQuick 2.0
import QtQuick.Dialogs 1.1
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"


Page {
    id: searchPage
    name: "torrent_search"

    property string torrentXmlPath

    QtObject {
        id: privateData
        property bool isEdited: false
    }

    XmlListModel {
        id: infoModel
        source: iXmlPath
        query: "/i/info"

        XmlRole {
            name: "user_photo"
            query: "photo/string()"
        }

        XmlRole {
            name: "user_name"
            query: "name/string()"
        }

        onCountChanged: {
            if (count === 1) {
                photoImage.source = infoModel.get(0).user_photo !== ""?"image://xml/" + Core.dataDirPath + "/i.xml":""
            }
        }
    }
}
