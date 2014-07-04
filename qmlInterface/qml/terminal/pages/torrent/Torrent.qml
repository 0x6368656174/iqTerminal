import QtQuick 2.0
import TerminalQmlPlugin 1.0
import ".."
//import "pages"
import "../../elements"


Page {
    id: torrentPage
    name: torrent
    property string iXmlPath: Core.dataDir + "/i.xml"

    Header {
        activeTab: "search"
    }

    BackButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: Core.dp(70)
    }
}
