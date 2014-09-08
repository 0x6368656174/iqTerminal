import QtQuick 2.0

Rectangle {
    property int duration: 30
    property bool runing: false
    property real progress: 0

    signal finished()

    id: progressBar

    color: "#a41515"
    Rectangle {
        id: bar
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: progressBar.width*(progress/100)
        color: "#b1b1b1"

        NumberAnimation {
            target: progressBar
            property: "progress"
            from: 0
            to: 100
            duration: progressBar.duration * 1000
            running: progressBar.runing

            onStopped: {
                if (progressBar.progress == 100) {
                    progressBar.finished()
                }
                progressBar.runing = false
            }
        }
    }
}
