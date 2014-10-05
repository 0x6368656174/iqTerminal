import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"

Page {
    id: photosPage
    name: "userInfo_photo"
    property bool readOnly: false
    clip: false

    QtObject {
        id: privateData
        property bool isEdited: false
    }

    CreateNewButton {
        id: createNewButton
        visible: !photosPage.readOnly
        enabled: !privateData.isEdited
        onClicked: {
            var folder = userInfo.photosModel.insertNew(0)
            folder.name = "Новая папка"
            userInfo.photosModel.save()
        }
    }

    Rectangle {
        id: spacer
        visible: !photosPage.readOnly
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
        anchors.top: !photosPage.readOnly?spacer.bottom:parent.top
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

        model: userInfo.photosModel

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
                        if (photosPage.readOnly)
                            return
                        if (pressed && !privateData.isEdited) {
                            folderPressAndHoldAmiation.restart()
                        } else {
                            folderPressAndHoldAmiation.stop()
                            folderPressAndHoldRect.opacity = 0
                        }
                    }

                    onPressAndHold: {
                        if (photosPage.readOnly)
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
                height: folder_additional_data.collapsed?childsView.height - Core.dp(2):0
                clip: true

                Behavior on height {NumberAnimation {duration: 200 } }

                GridView {
                    id: childsView
                    property int columnCount: width / Core.dp(60) < 1?1:width / Core.dp(60)
                    property int rowCountInt: folder_files_model.count / columnCount
                    property int rowCount:  {
                        if (folder_files_model.count / columnCount - rowCountInt !== 0) {
                            return rowCountInt + 1
                        }
                        return rowCountInt
                    }
                    interactive: false
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.rightMargin: -Core.dp(2)
                    height: rowCount*cellHeight
                    cellHeight: Core.dp(74)
                    cellWidth: width/columnCount
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
                        width: childsView.cellWidth - Core.dp(2)
                        height: childsView.cellHeight - Core.dp(2)
                        color: "#6c676e"

                        Item
                        {
                            anchors.fill: parent
                            anchors.bottomMargin: Core.dp(12)
                            Item{
                                anchors.centerIn: parent
                                width: photoImage.sourceSize.width > parent.width? parent.width:photoImage.sourceSize.width
                                height: photoImage.sourceSize.height > Core.dp(60)?Core.dp(60):photoImage.sourceSize.height
                                Image {
                                    id: photoImage
                                    anchors.fill: parent
                                    source: file_path
                                    fillMode: Image.PreserveAspectFit
                                }
                            }
                        }

                        Rectangle {
                            id: childNameContainer
                            width: parent.width
                            height: Core.dp(12)
                            anchors.bottom: parent.bottom
                            color: "#dddddd"

                            Rectangle {
                                anchors.fill: parent
                                color: "#da4504"
                                opacity: file_additional_data.isEdited?1:0
                            }

                            Text {
                                id: childNameText
                                visible: !file_additional_data.isEdited || editBar.editRole !== "edit"
                                anchors.fill: parent
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                anchors.leftMargin: Core.dp(8)
                                anchors.rightMargin: Core.dp(8)
                                font.pixelSize: Core.dp(8)
                                text: file_name
                            }

                            TextInput {
                                id: childNameEditor
                                anchors.fill: childNameText
                                clip: true
                                visible: file_additional_data.isEdited && editBar.editRole === "edit"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
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
                        }

                        ColorAnimation {
                            id: childPressAndHoldAmiation
                            target: childNameContainer
                            property: "color"
                            from: "#dddddd"
                            to: "#da4504"
                            duration: 800
                        }

                        TerminalMouseArea {
                            anchors.fill: parent

                            onClicked: {
                                photoSlider.folderIndex = folderIndex
                                photoSlider.photoIndex = fileIndex
                                photoSlider.opacity = 1
                            }
                            onPressedChanged: {
                                if (photosPage.readOnly)
                                    return
                                if (pressed && !privateData.isEdited) {
                                    childPressAndHoldAmiation.restart()
                                } else {
                                    childPressAndHoldAmiation.stop()
                                    childNameContainer.color = "#dddddd"
                                }
                            }

                            onPressAndHold: {
                                if (photosPage.readOnly)
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
                for (var i = 0; i < userInfo.photosModel.count; i++) {
                    var folder = userInfo.photosModel.get(i)
                    if (folder.additionalData.isEdited) {
                        userInfo.photosModel.remove(i)
                        userInfo.photosModel.save()
                        break
                    }
                    var fileRemoved = false
                    for (var j = 0; j < folder.filesModel.count; j++) {
                        var file = folder.filesModel.get(j)
                        if (file.additionalData.isEdited) {
                            folder.filesModel.remove(j)
                            userInfo.photosModel.save()
                            fileRemoved = true
                            break
                        }
                    }
                    if (fileRemoved)
                        break
                }
            } else if (editRole === "edit") {
                for (i = 0; i < userInfo.photosModel.count; i++) {
                    folder = userInfo.photosModel.get(i)
                    if (folder.additionalData.isEdited) {
                        folder.name = folder.additionalData.nameToSave
                        userInfo.photosModel.save()
                        break
                    }
                    var fileEdited = false
                    for (j = 0; j < folder.filesModel.count; j++) {
                        file = folder.filesModel.get(j)
                        if (file.additionalData.isEdited) {
                            file.name = file.additionalData.nameToSave
                            userInfo.photosModel.save()
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
            for (var i = 0; i < userInfo.photosModel.count; i++) {
                var folder = userInfo.photosModel.get(i)
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
        title: qsTr("Выбирите изображени")
        anchors.bottom: parent.bottom
        height: userInfoPage.height
        onVisibleChanged: userInfoPageBackButton.visible = !visible

        onAccepted: {
            for (var i = 0; i < userInfo.photosModel.count; i++) {
                var folder = userInfo.photosModel.get(i)
                if (folder.additionalData.isEdited) {
                    folder.filesModel.insertNew(0, fileUrl)
                    userInfo.photosModel.save()
                    break
                }
                var fileAdd = false
                for (var j = 0; j < folder.filesModel.count; j++) {
                    var file = folder.filesModel.get(j)
                    if (file.additionalData.isEdited) {
                        folder.filesModel.insertNew(j + 1, fileUrl)
                        userInfo.photosModel.save()
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
