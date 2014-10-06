import QtQuick 2.0
import TerminalQmlPlugin 1.0


Text {
    id: text
    height: (24 * applicationModel.settings.zoomFactor)
    verticalAlignment: Text.AlignVCenter
    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
    elide: Text.ElideRight

    signal clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: text.clicked()
    }
}
