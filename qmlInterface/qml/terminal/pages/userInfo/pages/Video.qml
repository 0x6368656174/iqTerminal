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
        height: (2 * applicationModel.settings.zoomFactor)
    }

    ListView {
        id: foldersView
        visible: photoSlider.opacity !== 1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: !videoPage.readOnly?spacer.bottom:parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: (22 * applicationModel.settings.zoomFactor) + editBar.anchors.bottomMargin - 1
        interactive: contentHeight > height
        spacing: (5 * applicationModel.settings.zoomFactor)
        clip: true

        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
        }

        displaced: Transition {
            NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
        }

        model: userInfo?userInfo.videosModel:undefined

        delegate: Rectangle {
            property int folderIndex: index
            id: folderDelegate
            anchors.left: parent.left
            anchors.right: parent.right
            height: (22 * applicationModel.settings.zoomFactor) + collapsedContainer.height/* + (4 * applicationModel.settings.zoomFactor)*/

            Rectangle {
                id: folderNameContainer
                anchors.left: parent.left
                anchors.right: parent.right
                height: (22 * applicationModel.settings.zoomFactor)
                color: folder_additional_data.collapsed?"#da4504":"#c6c1c7"
                Behavior on color {ColorAnimation { duration: 200 } }

                Image {
                    id: folderImage
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: (6 * applicationModel.settings.zoomFactor)
                    height: (15 * applicationModel.settings.zoomFactor)
                    width: (17 * applicationModel.settings.zoomFactor)
                    fillMode: Image.PreserveAspectFit
                    source: folder_additional_data.collapsed?"../../../images/47.png":"../../../images/40.png"
                }

                Text {
                    id: folderText
                    visible: !folder_additional_data.isEdited  || editBar.editRole !== "edit"
                    anchors.left: folderImage.right
                    anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: anchors.leftMargin
                    elide: Text.ElideRight
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
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
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
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
                    anchors.rightMargin: -(2 * applicationModel.settings.zoomFactor)
                    height: folder_files_model.count * (22 * applicationModel.settings.zoomFactor)
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
                        height: (22 * applicationModel.settings.zoomFactor)

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
                            anchors.leftMargin: (30 * applicationModel.settings.zoomFactor)
                            height: (15 * applicationModel.settings.zoomFactor)
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
                            anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
                            anchors.rightMargin: (34 * applicationModel.settings.zoomFactor)
                            font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                            text: file_name
                        }

                        TextInput {
                            id: childNameEditor
                            anchors.fill: childNameText
                            clip: true
                            visible: file_additional_data.isEdited && editBar.editRole === "edit"
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: (8 * applicationModel.settings.zoomFactor)
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
        anchors.bottomMargin: privateData.isEdited && !fileDialog.visible?0:-(22 * applicationModel.settings.zoomFactor)
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
        selectMultiple: true
        selectFolder: true
        title: qsTr("Select video") + applicationModel.settings.translatorStringEnd
        anchors.bottom: parent.bottom
        height: userInfoPage.height
        onVisibleChanged: userInfoPageBackButton.visible = !visible

        onAccepted: {
            //Добавим новые папки
            for (var i = 0; i < fileDialog.foldersUrls.length; i++) {
                userInfo.videosModel.insertNew(0, fileDialog.foldersUrls[i])
            }

            //Добавим выделеные файлы
            for (i = 0; i < userInfo.videosModel.count; i++) {
                var folder = userInfo.videosModel.get(i)
                if (folder.additionalData.isEdited) {
                    //Если редактируетя папка
                    for (var j = 0; j < fileDialog.filesUrls.length; j++) {
                        folder.filesModel.insertNew(j, fileDialog.filesUrls[j])
                    }
                    break
                }
                var fileAdd = false
                for (j = 0; j < folder.filesModel.count; j++) {
                    var file = folder.filesModel.get(j)
                    if (file.additionalData.isEdited) {
                        //Если редактируется файл
                        for (var k = 0; k < fileDialog.filesUrls.length; k++) {
                            folder.filesModel.insertNew(j + k + 1, fileDialog.filesUrls[k])
                        }
                        fileAdd = true
                        break
                    }
                }
                if (fileAdd)
                    break
            }

            userInfo.videosModel.save()
            editBar.cansel()
        }

        onRejected: {
            editBar.cansel()
        }
    }
}
