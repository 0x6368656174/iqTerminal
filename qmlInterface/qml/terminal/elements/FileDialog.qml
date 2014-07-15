import QtQuick 2.0

Item {
    id: fileDialog
    property url fileUrl
    property string title
    property bool selectMultiple: false
    property bool selectFolder: false

    visible: privateData.active

    signal accepted
    signal rejected

    function open() {
        privateData.active = true
        globalFileDialog.title = fileDialog.title
        globalFileDialog.selectMultiple = fileDialog.selectMultiple
        globalFileDialog.selectFolder = fileDialog.selectFolder
        globalFileDialog.open()
    }

    QtObject {
        id: privateData
        property bool active: false
    }

    Connections {
        target: globalFileDialog
        onAccepted : {
            if (privateData.active) {
                fileUrl = globalFileDialog.fileUrl
                fileDialog.accepted()
                privateData.active = false
            }
        }
        onRejected : {
            if (privateData.active) {
                fileUrl = globalFileDialog.fileUrl
                fileDialog.rejected()
                privateData.active = false
            }
        }
    }
}
