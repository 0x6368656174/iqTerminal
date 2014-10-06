import QtQuick 2.0
import TerminalQmlPlugin 1.0

Button {
    width: 13 * applicationModel.settings.zoomFactor
    height: 26 * applicationModel.settings.zoomFactor
    source: "../images/92.png"
    onClicked: main.backPage()
    z: 1
}
