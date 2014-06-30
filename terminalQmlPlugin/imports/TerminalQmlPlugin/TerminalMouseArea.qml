import QtQuick 2.0
import TerminalQmlPlugin 1.0

MouseArea {
    property bool platformIndependentHoverEnabled: false

    hoverEnabled: {
        if (Core.operatingSystem !== Core.Android) {
            return platformIndependentHoverEnabled;
        }
        return false;
    }
}
