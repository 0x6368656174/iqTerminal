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
        height: (2 * applicationModel.settings.zoomFactor)
    }

    ListView {
        id: foldersView
        visible: photoSlider.opacity !== 1

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: !photosPage.readOnly?spacer.bottom:parent.top
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

        model: userInfo?userInfo.photosModel:undefined

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
                height: folder_additional_data.collapsed?childsView.height - (2 * applicationModel.settings.zoomFactor):0
                clip: true

                Behavior on height {NumberAnimation {duration: 200 } }

                GridView {
                    id: childsView
                    property int columnCount: width / (60 * applicationModel.settings.zoomFactor) < 1?1:width / (60 * applicationModel.settings.zoomFactor)
                    property int rowCountInt: folder_files_model.count / columnCount
                    property int rowCount:  folder_files_model.count / columnCount - rowCountInt !== 0?rowCountInt + 1:rowCountInt
                    interactive: false
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.rightMargin: -(2 * applicationModel.settings.zoomFactor)
                    height: rowCount*cellHeight
                    cellHeight: (74 * applicationModel.settings.zoomFactor)
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
                        width: childsView.cellWidth - (2 * applicationModel.settings.zoomFactor)
                        height: childsView.cellHeight - (2 * applicationModel.settings.zoomFactor)
                        color: "#6c676e"

                        Item
                        {
                            anchors.fill: parent
                            anchors.bottomMargin: (12 * applicationModel.settings.zoomFactor)
                            Item{
                                anchors.centerIn: parent
                                width: photoImage.sourceSize.width > parent.width? parent.width:photoImage.sourceSize.width
                                height: photoImage.sourceSize.height > (60 * applicationModel.settings.zoomFactor)?(60 * applicationModel.settings.zoomFactor):photoImage.sourceSize.height
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
                            height: (12 * applicationModel.settings.zoomFactor)
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
                                anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
                                anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                                font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                                text: file_name
                            }

                            TextInput {
                                id: childNameEditor
                                anchors.fill: childNameText
                                clip: true
                                visible: file_additional_data.isEdited && editBar.editRole === "edit"
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
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
        selectMultiple: true
        selectFolder: true
        title: qsTr("Select image") + applicationModel.settings.translatorStringEnd
        anchors.bottom: parent.bottom
        height: userInfoPage.height
        onVisibleChanged: userInfoPageBackButton.visible = !visible

        onAccepted: {
            //Добавим новые папки
            for (var i = 0; i < fileDialog.foldersUrls.length; i++) {
                userInfo.photosModel.insertNew(0, fileDialog.foldersUrls[i])
            }

            //Добавим выделеные файлы
            for (i = 0; i < userInfo.photosModel.count; i++) {
                var folder = userInfo.photosModel.get(i)
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

            userInfo.photosModel.save()
            editBar.cansel()
        }

        onRejected: {
            editBar.cansel()
        }
    }
}
