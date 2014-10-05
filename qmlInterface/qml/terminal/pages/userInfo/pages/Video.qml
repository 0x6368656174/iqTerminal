import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"

Page {
    id: videoPage
    property bool readOnly: false
    name: "userInfo_video"
    clip: false

    QtObject {
        id: privateData
        property bool isEdited: false
    }

    CreateNewButton {
        id: createNewButton
        visible: !videoPage.readOnly
        enabled: !privateData.isEdited
        onClicked: {
            var folder = userInfo.videosModel.insertNew(0)
            folder.name = "Новая папка"
            userInfo.videosModel.save()
        }
    }

    Rectangle {
        id: spacer
        visible: !videoPage.readOnly
        anchors.top: createNewButton.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Core.dp(2)
    }

    ListView {
        id: foldersView
        visible: photoSlider.opacity !== 1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: !videoPage.readOnly?spacer.bottom:parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: Core.dp(22) + editBar.anchors.bottomMargin - 1
        interactive: contentHeight > height
        spacing: Core.dp(5)
        clip: true

        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
        }

        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
        }

        model: userInfo.videosModel

        delegate: Rectangle {
            property int folderIndex: index
            id: folderDelegate
            anchors.left: parent.left
            anchors.right: parent.right
            height: Core.dp(22) + collapsedContainer.height/* + Core.dp(4)*/

            Rectangle {
                id: folderNameContainer
                anchors.left: parent.left
                anchors.right: parent.right
                height: Core.dp(22)
                color: folder_additional_data.collapsed?"#da4504":"#c6c1c7"
                Behavior on color {ColorAnimation { duration: 200 } }

                Image {
                    id: folderImage
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: Core.dp(6)
                    height: Core.dp(15)
                    width: Core.dp(17)
                    fillMode: Image.PreserveAspectFit
                    source: folder_additional_data.collapsed?"../../../images/47.png":"../../../images/40.png"
                }

                Text {
                    id: folderText
                    visible: !folder_additional_data.isEdited  || editBar.editRole !== "edit"
                    anchors.left: folderImage.right
                    anchors.leftMargin: Core.dp(8)
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: anchors.leftMargin
                    elide: Text.ElideRight
                    font.pixelSize: Core.dp(8)
                    verticalAlignment: Text.AlignVCenter
                    color: folder_additional_data.collapsed?"white":"black"
                    Behavior on color {ColorAnimation { duration: 200 } }
                    text: folder_name
                }

                TextInput {
                    id: folderTextEditor
                    clip: true
                    visible: folder_additional_data.isEdited && editBar.editRole === "edit"
                    anchors.fill: folderText
                    text: folder_name
                    onTextChanged: folder_additional_data.nameToSave = text
                    color: folderText.color
                    font.pixelSize: Core.dp(8)
                    selectByMouse: true
                    selectionColor: "#c00d0d"
                    verticalAlignment: Text.AlignVCenter
                    onVisibleChanged: {
                        if(visible) {
                            text = folder_name
                            forceActiveFocus()
                        }
                    }
                    onAccepted: editBar.submit()
                }

                Rectangle {
                    anchors.fill: parent
                    border.color: folder_additional_data.collapsed?"white":"#da4504"
                    border.width: 2
                    anchors.margins: 2
                    opacity: folder_additional_data.isEdited?1:0
                    color: "transparent"
                }

                Rectangle {
                    id: folderPressAndHoldRect
                    anchors.fill: parent
                    border.color: folder_additional_data.collapsed?"white":"#da4504"
                    border.width: 2
                    anchors.margins: 2
                    opacity: 0
                    color: "transparent"
                }

                PropertyAnimation {
                    id: folderPressAndHoldAmiation
                    target: folderPressAndHoldRect
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 800
                }

                SequentialAnimation {
                    id: photoFolderClickAmination
                    PropertyAnimation {
                        target: folderImage
                        property: "scale"
                        from: 1.0
                        to: 1.2
                        duration: 200
                    }
                    ScriptAction {
                        script: {
                            folderImage.scale = 1.0
                        }
                    }
                }

                TerminalMouseArea {
                    anchors.fill: parent
                    onClicked: {
                        photoFolderClickAmination.restart()
                        folder_additional_data.collapsed = !folder_additional_data.collapsed
                    }
                    onPressedChanged: {
                        if (videoPage.readOnly)
                            return
                        if (pressed && !privateData.isEdited) {
                            folderPressAndHoldAmiation.restart()
                        } else {
                            folderPressAndHoldAmiation.stop()
                            folderPressAndHoldRect.opacity = 0
                        }
                    }

                    onPressAndHold: {
                        if (videoPage.readOnly)
                            return
                        if (!privateData.isEdited) {
                            folder_additional_data.isEdited = true
                            privateData.isEdited = true
                        }
                    }
                }
            }

            Item {
                id: collapsedContainer
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: folderNameContainer.bottom
                height: folder_additional_data.collapsed?childsView.height:0
                clip: true

                Behavior on height {NumberAnimation {duration: 200 } }

                ListView {
                    id: childsView
                    interactive: false
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.rightMargin: -Core.dp(2)
                    height: folder_files_model.count * Core.dp(22)
                    model: folder_files_model

                    add: Transition {
                        NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
                        NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
                    }

                    displaced: Transition {
                        NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
                    }

                    delegate: Rectangle {
                        property int fileIndex: index
                        id: childDelegate
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: Core.dp(22)

                        Rectangle {
                            anchors.fill: parent
                            visible: file_additional_data.isEdited
                            color: "#da4504"
                        }

                        Image {
                            id: fileImage
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            fillMode: Image.PreserveAspectFit
                            anchors.leftMargin: Core.dp(30)
                            height: Core.dp(15)
                            source: "../../../images/98.png"
                        }

                        Text {
                            id: childNameText
                            visible: !file_additional_data.isEdited || editBar.editRole !== "edit"
                            anchors.left: fileImage.right
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                            anchors.leftMargin: Core.dp(8)
                            anchors.rightMargin: Core.dp(34)
                            font.pixelSize: Core.dp(8)
                            text: file_name
                        }

                        TextInput {
                            id: childNameEditor
                            anchors.fill: childNameText
                            clip: true
                            visible: file_additional_data.isEdited && editBar.editRole === "edit"
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: Core.dp(8)
                            selectByMouse: true
                            selectionColor: "#c00d0d"
                            text: file_name
                            onTextChanged: file_additional_data.nameToSave = text
                            onVisibleChanged: {
                                if(visible) {
                                    text = file_name
                                    forceActiveFocus()
                                }
                            }
                            onAccepted: editBar.submit()
                        }

                        Rectangle {
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            height: 1
                            color: "#c6c1c7"
                        }

                        ColorAnimation {
                            id: childPressAndHoldAmiation
                            target: childDelegate
                            property: "color"
                            from: "white"
                            to: "#da4504"
                            duration: 800
                        }

                        TerminalMouseArea {
                            anchors.fill: parent

                            onClicked: {
                                videoPlayer.text = file_name
                                videoPlayer.file = file_path
                                videoPlayer.opacity = 1
                                videoPlayer.play()
                            }
                            onPressedChanged: {
                                if (videoPage.readOnly)
                                    return
                                if (pressed && !privateData.isEdited) {
                                    childPressAndHoldAmiation.restart()
                                } else {
                                    childPressAndHoldAmiation.stop()
                                    childDelegate.color = "white"
                                }
                            }

                            onPressAndHold: {
                                if (videoPage.readOnly)
                                    return
                                if (!privateData.isEdited) {
                                    file_additional_data.isEdited = true
                                    privateData.isEdited = true
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    EditBar {
        id: editBar
        visible: opacity !== 0
        opacity: privateData.isEdited && !fileDialog.visible?1:0
        anchors.bottomMargin: privateData.isEdited && !fileDialog.visible?0:-Core.dp(22)
        hideOnMissClick: false
        canselButtonEnabled: true

        onButtonClicked: {
            if (buttonType === "add") {
                fileDialog.open()
            }
        }

        onSubmit: {
            if (editRole === "remove") {
                for (var i = 0; i < userInfo.videosModel.count; i++) {
                    var folder = userInfo.videosModel.get(i)
                    if (folder.additionalData.isEdited) {
                        userInfo.videosModel.remove(i)
                        userInfo.videosModel.save()
                        break
                    }
                    var fileRemoved = false
                    for (var j = 0; j < folder.filesModel.count; j++) {
                        var file = folder.filesModel.get(j)
                        if (file.additionalData.isEdited) {
                            folder.filesModel.remove(j)
                            userInfo.videosModel.save()
                            fileRemoved = true
                            break
                        }
                    }
                    if (fileRemoved)
                        break
                }
            } else if (editRole === "edit") {
                for (i = 0; i < userInfo.videosModel.count; i++) {
                    folder = userInfo.videosModel.get(i)
                    if (folder.additionalData.isEdited) {
                        folder.name = folder.additionalData.nameToSave
                        userInfo.videosModel.save()
                        break
                    }
                    var fileEdited = false
                    for (j = 0; j < folder.filesModel.count; j++) {
                        file = folder.filesModel.get(j)
                        if (file.additionalData.isEdited) {
                            file.name = file.additionalData.nameToSave
                            userInfo.videosModel.save()
                            fileEdited = true
                            break
                        }
                    }
                    if (fileEdited)
                        break
                }
            }
            cansel()
        }

        onCansel: {
            for (var i = 0; i < userInfo.videosModel.count; i++) {
                var folder = userInfo.videosModel.get(i)
                folder.additionalData.isEdited = false
                for (var j = 0; j < folder.filesModel.count; j++) {
                    var file = folder.filesModel.get(j)
                    file.additionalData.isEdited = false
                }
            }
            privateData.isEdited = false
        }
    }

    FileDialog {
        id: fileDialog
        selectMultiple: false
        title: qsTr("Выбирите видео")
        anchors.bottom: parent.bottom
        height: userInfoPage.height
        onVisibleChanged: userInfoPageBackButton.visible = !visible

        onAccepted: {
            for (var i = 0; i < userInfo.videosModel.count; i++) {
                var folder = userInfo.videosModel.get(i)
                if (folder.additionalData.isEdited) {
                    folder.filesModel.insertNew(0, fileUrl)
                    userInfo.videosModel.save()
                    break
                }
                var fileAdd = false
                for (var j = 0; j < folder.filesModel.count; j++) {
                    var file = folder.filesModel.get(j)
                    if (file.additionalData.isEdited) {
                        folder.filesModel.insertNew(j + 1, fileUrl)
                        userInfo.videosModel.save()
                        fileAdd = true
                        break
                    }
                }
                if (fileAdd)
                    break
            }
            editBar.cansel()
        }

        onRejected: {
            editBar.cansel()
        }
    }
}
