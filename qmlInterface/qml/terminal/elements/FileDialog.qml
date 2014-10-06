import QtQuick 2.0
import TerminalQmlPlugin 1.0
import Qt.labs.folderlistmodel 2.1
import "../pages"

Page {
    id: fileDialog
    property url fileUrl
    property string title
    property bool selectMultiple: false
    property bool selectFolder: false
    property bool selectFile: true
    property bool selfSelect: false
    property bool onlyFolder: true
    property alias showFiles: folderModel.showFiles
    property alias showDirsFirst: folderModel.showDirsFirst
    property alias showHidden: folderModel.showHidden

    signal accepted()
    signal rejected()

    onAccepted: opacity = 0
    onRejected: opacity = 0

    QtObject {
        id: privateData
        property bool active: false
        property url currentFolder: Core.homeDir
    }

    function open() {
        opacity = 1
    }

    name: "file_dialog"
    visible: opacity > 0
    opacity: 0

    Rectangle {
        id: pathContainer
        anchors.left: parent.left
        anchors.right: parent.right
        height: 22 * applicationModel.settings.zoomFactor
        color: "#da4504"

        Item {
            id: staticPathItem
            anchors.fill: parent
            Text {
                id: parentFolderText
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: (parent.width-10 * applicationModel.settings.zoomFactor)/2
                visible: UrlHelper.hasContainingFolder(privateData.currentFolder)
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: 8 * applicationModel.settings.zoomFactor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: UrlHelper.dirName(UrlHelper.containingFolder(privateData.currentFolder))

                TerminalMouseArea {
                    anchors.fill: parent
                    platformIndependentHoverEnabled: true
                    onContainsMouseChanged: {
                        if (containsMouse) {
                            parentFolderText.font.pixelSize = 10 * applicationModel.settings.zoomFactor
                        } else {
                            parentFolderText.font.pixelSize = 8 * applicationModel.settings.zoomFactor
                        }
                    }

                    onClicked:{
                        animatePathItem.currentFolder = privateData.currentFolder
                        backAnimation.start()
                    }
                }
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                width: 10 * applicationModel.settings.zoomFactor
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                fillMode: Image.PreserveAspectFit
                source: "../images/201.png"
            }

            Text {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: (parent.width-10 * applicationModel.settings.zoomFactor)/2
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: 8 * applicationModel.settings.zoomFactor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: UrlHelper.dirName(privateData.currentFolder)
            }
        }

        Rectangle {
            id: animatePathItem
            property url currentFolder
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width*1.5
            visible: false
            color: pathContainer.color
            Text {
                id: animatedText1
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: (staticPathItem.width-10 * applicationModel.settings.zoomFactor)/2
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: 8 * applicationModel.settings.zoomFactor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: {
                    if (UrlHelper.hasContainingFolder(UrlHelper.containingFolder(UrlHelper.containingFolder(animatePathItem.currentFolder)))){
                        return UrlHelper.dirName(UrlHelper.containingFolder(UrlHelper.containingFolder(animatePathItem.currentFolder)))
                    }
                    return ""
                }
            }

            Image {
                id: animatedImage1
                anchors.left: animatedText1.right
                width: 10 * applicationModel.settings.zoomFactor
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                fillMode: Image.PreserveAspectFit
                source: "../images/201.png"
            }

            Text {
                id: animatedText2
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: animatedImage1.right
                width: (staticPathItem.width-10 * applicationModel.settings.zoomFactor)/2
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: 8 * applicationModel.settings.zoomFactor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: {
                    if (UrlHelper.hasContainingFolder(UrlHelper.containingFolder(animatePathItem.currentFolder))){
                        return UrlHelper.dirName(UrlHelper.containingFolder(animatePathItem.currentFolder))
                    }
                    return ""
                }
            }

            Image {
                id: animatedImage2
                anchors.left: animatedText2.right
                width: 10 * applicationModel.settings.zoomFactor
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                fillMode: Image.PreserveAspectFit
                source: "../images/201.png"
            }

            Text {
                id: animatedText3
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: animatedImage2.right
                width: (staticPathItem.width-10 * applicationModel.settings.zoomFactor)/2
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: 8 * applicationModel.settings.zoomFactor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: UrlHelper.dirName(animatePathItem.currentFolder)
            }
        }
    }

    SequentialAnimation {
        id: backAnimation
        ScriptAction {
            script: {
                animatePathItem.x = - (staticPathItem.width - (10 * applicationModel.settings.zoomFactor))/2 - (10 * applicationModel.settings.zoomFactor)
                animatePathItem.visible = true
            }
        }
        PropertyAnimation {
            target: animatePathItem
            property: "x"
            to: 0
            duration: 200
        }
        ScriptAction {
            script: {
                privateData.currentFolder = UrlHelper.containingFolder(animatePathItem.currentFolder)
                animatePathItem.visible = false
            }
        }
    }

    SequentialAnimation {
        id: forwardAnimation
        ScriptAction {
            script: {
                animatePathItem.x = 0
                animatePathItem.visible = true
            }
        }
        PropertyAnimation {
            target: animatePathItem
            property: "x"
            to: - (staticPathItem.width - (10 * applicationModel.settings.zoomFactor))/2 - (10 * applicationModel.settings.zoomFactor)
            duration: 200
        }
        ScriptAction {
            script: {
                privateData.currentFolder = animatePathItem.currentFolder
                animatePathItem.visible = false
            }
        }
    }

    FolderListModel {
        id: folderModel
        folder: privateData.currentFolder
        showDirsFirst: true
    }

    ListView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: pathContainer.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: (22 * applicationModel.settings.zoomFactor)
        model: folderModel
        clip: true
        spacing: (2 * applicationModel.settings.zoomFactor)

        delegate: Rectangle {
            id: itemContainer
            anchors.left: parent.left
            anchors.right: parent.right
            height: (22 * applicationModel.settings.zoomFactor)

            Image {
                id: itemImage
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: (6 * applicationModel.settings.zoomFactor)
                height: (15 * applicationModel.settings.zoomFactor)
                width: (17 * applicationModel.settings.zoomFactor)
                fillMode: Image.PreserveAspectFit
                source: fileIsDir?"../images/40a.png":"../images/98a.png"
            }

            Text {
                id: itemNameText
                anchors.left: itemImage.right
                anchors.right: itemCheckButton.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: parent.height / 2 - (2 * applicationModel.settings.zoomFactor)
                height: (22 * applicationModel.settings.zoomFactor)
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
                anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                text: fileName
            }

            //РАЗМЕР
            Text {
                id: fileSizeTitle
                anchors.left: itemNameText.left
                anchors.top: itemNameText.bottom
                font.pixelSize: (6 * applicationModel.settings.zoomFactor)
                color: itemNameText.color
                visible: !fileIsDir
                text: qsTr("Size") + applicationModel.settings.translatorStringEnd
            }

            Text {
                id: fileSizeText
                anchors.verticalCenter: fileSizeTitle.verticalCenter
                anchors.left: fileSizeTitle.right
                anchors.leftMargin: (3 * applicationModel.settings.zoomFactor)
                font.pixelSize: fileSizeTitle.font.pixelSize
                color: itemNameText.color
                visible: !fileIsDir
                text: Core.humanReadableSize(fileSize)
            }

            //КОЛЛИЧЕСТВО ЭЛЕМЕНТОВ
            Text {
                id: folderEntryTitle
                anchors.left: itemNameText.left
                anchors.top: itemNameText.bottom
                font.pixelSize: (6 * applicationModel.settings.zoomFactor)
                color: itemNameText.color
                visible: fileIsDir
                text: qsTr("Elemens") + applicationModel.settings.translatorStringEnd
            }

            Text {
                id: folderEntryText
                anchors.verticalCenter: folderEntryTitle.verticalCenter
                anchors.left: folderEntryTitle.right
                anchors.leftMargin: (3 * applicationModel.settings.zoomFactor)
                font.pixelSize: folderEntryTitle.font.pixelSize
                color: itemNameText.color
                visible: fileIsDir
                text: {
                    if (fileIsDir) {
                        if (Core.operatingSystem === Core.Windows) {
                            return UrlHelper.dirEntryCount("file:///" + filePath)
                        } else {
                            return UrlHelper.dirEntryCount("file://" + filePath)
                        }
                    }
                    return ""
                }
            }

            TerminalMouseArea {
                anchors.fill: parent
                onClicked: {
                    if (fileIsDir) {
                        if (Core.operatingSystem === Core.Windows) {
                            var windowsPath = filePath
                            var windowsDrive = windowsPath.substring(0, 1)
                            windowsDrive = windowsDrive.toUpperCase()
                            windowsPath = windowsDrive + windowsPath.substring(1)
                            animatePathItem.currentFolder = "file:///" + windowsPath
                        } else {
                            animatePathItem.currentFolder = "file://" + filePath
                        }
                        forwardAnimation.start()
                    }
                }
            }

            //ВЫБОР
            Image {
                id: itemCheckButton
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: (6 * applicationModel.settings.zoomFactor)
                width: fileIsDir && fileDialog.selectFolder || !fileIsDir && fileDialog.selectFile?(14 * applicationModel.settings.zoomFactor):0
                height: width
                fillMode: Image.PreserveAspectFit
                source: "../images/34.png"

                TerminalMouseArea {
                    anchors.fill: parent
                    platformIndependentHoverEnabled: true
                    onClicked: {
                        if (Core.operatingSystem === Core.Windows) {
                            var windowsPath = filePath
                            var windowsDrive = windowsPath.substring(0, 1)
                            windowsDrive = windowsDrive.toUpperCase()
                            windowsPath = windowsDrive + windowsPath.substring(1)
                            fileDialog.fileUrl = "file:///" + windowsPath
                        } else {
                            fileDialog.fileUrl = "file://" + filePath
                        }
                        fileDialog.accepted()
                    }
                    onContainsMouseChanged: {
                        if (containsMouse) {
                            itemCheckButton.source = "../images/34a.png"
                        } else {
                            itemCheckButton.source = "../images/34.png"
                        }
                    }
                }
            }
        }
    }

    EditBar {
        id: editBar
        z: 1
        addButtonEnabled: false
        editButtonEnabled: false
        removeButtonEnabled: false
        canselButtonEnabled: true
        hideOnMissClick: false
        selectAllButtonEnabled: fileDialog.selfSelect

        onButtonClicked: {
            if (buttonType === "selectAll") {
                fileDialog.fileUrl = privateData.currentFolder
                fileDialog.accepted()
            }
        }

        onCansel: {
            fileDialog.rejected()
        }
    }
}
