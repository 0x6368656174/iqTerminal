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
        height: Core.dp(22)
        color: "#da4504"

        Item {
            id: staticPathItem
            anchors.fill: parent
            Text {
                id: parentFolderText
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: (parent.width-Core.dp(10))/2
                visible: UrlHelper.hasContainingFolder(privateData.currentFolder)
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: Core.dp(8)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                text: UrlHelper.dirName(UrlHelper.containingFolder(privateData.currentFolder))

                TerminalMouseArea {
                    anchors.fill: parent
                    platformIndependentHoverEnabled: true
                    onContainsMouseChanged: {
                        if (containsMouse) {
                            parentFolderText.font.pixelSize = Core.dp(10)
                        } else {
                            parentFolderText.font.pixelSize = Core.dp(8)
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
                width: Core.dp(10)
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                fillMode: Image.PreserveAspectFit
                source: "../images/201.png"
            }

            Text {
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: (parent.width-Core.dp(10))/2
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: Core.dp(8)
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
                width: (staticPathItem.width-Core.dp(10))/2
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: Core.dp(8)
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
                width: Core.dp(10)
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
                width: (staticPathItem.width-Core.dp(10))/2
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: Core.dp(8)
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
                width: Core.dp(10)
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
                width: (staticPathItem.width-Core.dp(10))/2
                color: "white"
                elide: Text.ElideRight
                font.pixelSize: Core.dp(8)
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
                animatePathItem.x = - (staticPathItem.width - Core.dp(10))/2 - Core.dp(10)
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
            to: - (staticPathItem.width - Core.dp(10))/2 - Core.dp(10)
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
        anchors.bottomMargin: Core.dp(22)
        model: folderModel
        clip: true
        spacing: Core.dp(2)

        delegate: Rectangle {
            id: itemContainer
            anchors.left: parent.left
            anchors.right: parent.right
            height: Core.dp(22)

            Image {
                id: itemImage
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: Core.dp(6)
                height: Core.dp(15)
                width: Core.dp(17)
                fillMode: Image.PreserveAspectFit
                source: fileIsDir?"../images/40a.png":"../images/98a.png"
            }

            Text {
                id: itemNameText
                anchors.left: itemImage.right
                anchors.right: itemCheckButton.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: parent.height / 2 - Core.dp(2)
                height: Core.dp(22)
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                anchors.leftMargin: Core.dp(8)
                anchors.rightMargin: Core.dp(8)
                font.pixelSize: Core.dp(8)
                text: fileName
            }

            //РАЗМЕР
            Text {
                id: fileSizeTitle
                anchors.left: itemNameText.left
                anchors.top: itemNameText.bottom
                font.pixelSize: Core.dp(6)
                color: itemNameText.color
                visible: !fileIsDir
                text: qsTr("Размер")
            }

            Text {
                id: fileSizeText
                anchors.verticalCenter: fileSizeTitle.verticalCenter
                anchors.left: fileSizeTitle.right
                anchors.leftMargin: Core.dp(3)
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
                font.pixelSize: Core.dp(6)
                color: itemNameText.color
                visible: fileIsDir
                text: qsTr("Элементов")
            }

            Text {
                id: folderEntryText
                anchors.verticalCenter: folderEntryTitle.verticalCenter
                anchors.left: folderEntryTitle.right
                anchors.leftMargin: Core.dp(3)
                font.pixelSize: folderEntryTitle.font.pixelSize
                color: itemNameText.color
                visible: fileIsDir
                text: fileIsDir?UrlHelper.dirEntryCount("file://" + filePath):""
            }

            TerminalMouseArea {
                anchors.fill: parent
                onClicked: {
                    if (fileIsDir) {
                        animatePathItem.currentFolder = "file://" + filePath
                        forwardAnimation.start()
                    }
                }
            }

            //ВЫБОР
            Image {
                id: itemCheckButton
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: Core.dp(6)
                width: fileIsDir && fileDialog.selectFolder || !fileIsDir && fileDialog.selectFile?Core.dp(14):0
                height: width
                fillMode: Image.PreserveAspectFit
                source: "../images/34.png"

                TerminalMouseArea {
                    anchors.fill: parent
                    platformIndependentHoverEnabled: true
                    onClicked: {
                        fileDialog.fileUrl = "file://" + filePath
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
