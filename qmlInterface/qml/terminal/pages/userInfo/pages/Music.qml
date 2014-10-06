import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"

Page {
    id: musicPage
    property bool readOnly: false
    name: "userInfo_music"
    clip: false

    QtObject {
        id: privateData
        property bool isEdited: false
    }

    CreateNewButton {
        id: createNewButton
        visible: !musicPage.readOnly
        enabled: !privateData.isEdited
        onClicked: {
            var folder = userInfo.musicsModel.insertNew(0)
            folder.name = "Новая папка"
            userInfo.musicsModel.save()
        }
    }

    Rectangle {
        id: spacer
        visible: !musicPage.readOnly
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
        anchors.top: !musicPage.readOnly?spacer.bottom:parent.top
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

        model: userInfo?userInfo.musicsModel:undefined

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
                        if (musicPage.readOnly)
                            return
                        if (pressed && !privateData.isEdited) {
                            folderPressAndHoldAmiation.restart()
                        } else {
                            folderPressAndHoldAmiation.stop()
                            folderPressAndHoldRect.opacity = 0
                        }
                    }

                    onPressAndHold: {
                        if (musicPage.readOnly)
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
                    height: {
                        for (var i = 0; i < folder_files_model.count; i++) {
                            if (folder_files_model.get(i).additionalData.isPlaying) {
                                return folder_files_model.count * (22 * applicationModel.settings.zoomFactor) + (48 * applicationModel.settings.zoomFactor)
                            }
                        }
                        return folder_files_model.count * (22 * applicationModel.settings.zoomFactor)
                    }
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
                        clip: true
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: !file_additional_data.isPlaying?(22 * applicationModel.settings.zoomFactor):(70 * applicationModel.settings.zoomFactor)

                        function play() {

                        }

                        Behavior on height {NumberAnimation {duration: 200} }

                        Rectangle {
                            anchors.fill: parent
                            visible: file_additional_data.isEdited
                            color: "#da4504"
                        }

                        Image {
                            id: fileImage
                            anchors.left: parent.left
                            anchors.verticalCenter: childNameText.verticalCenter
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
                            height: (22 * applicationModel.settings.zoomFactor)
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

                            onClicked: file_additional_data.isPlaying = !file_additional_data.isPlaying
                            onPressedChanged: {
                                if (musicPage.readOnly)
                                    return
                                if (pressed && !privateData.isEdited) {
                                    childPressAndHoldAmiation.restart()
                                } else {
                                    childPressAndHoldAmiation.stop()
                                    childDelegate.color = "white"
                                }
                            }

                            onPressAndHold: {
                                if (musicPage.readOnly)
                                    return
                                if (!privateData.isEdited) {
                                    file_additional_data.isEdited = true
                                    privateData.isEdited = true
                                }
                            }
                        }

                        Connections {
                            target: file_additional_data
                            onIsPlayingChanged: {
                                if (file_additional_data.isPlaying) {
                                    for (var i = 0; i < userInfo.musicsModel.count; i++) {
                                        var folder = userInfo.musicsModel.get(i)
                                        for (var j = 0; j < folder.filesModel.count; j++) {
                                            if (i !== folderIndex || j !== fileIndex) {
                                                folder.filesModel.get(j).additionalData.isPlaying = false
                                            }
                                        }
                                    }

                                    audioPlayer.source = file_path
                                    audioPlayer.play()
                                } else {
                                    audioPlayer.stop()
                                    audioPlayer.source = ""
                                }
                            }
                        }

                        ListView.onRemove:{
                            if (mediaButtons.isPlay)
                                audioPlayer.stop()
                        }

                        Item {
                            anchors.top: childNameText.bottom
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: (38 * applicationModel.settings.zoomFactor)
                            clip: true

                            MediaButtons {
                                id: mediaButtons
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                position: audioPlayer.position
                                duration: audioPlayer.duration
                                isPlay: file_additional_data.isPlaying && audioPlayer.isPlay
                                isPause: file_additional_data.isPlaying && audioPlayer.isPause
                                isStop: file_additional_data.isPlaying && audioPlayer.isStop

                                onPlayClicked: {
                                    audioPlayer.play()
                                }

                                onPauseClicked: {
                                    audioPlayer.pause()
                                }

                                onStopClicked: {
                                    audioPlayer.stop()
                                }

                                onPositionSliderClicked: {
                                    audioPlayer.seek(newPosition)
                                }

                                Connections {
                                    target: audioPlayer
                                    onFinished: {
                                        if (file_additional_data.isPlaying && source.toString() === file_path.toString()) {
                                            //Дошли до конца, включим следующую
                                            var nextFolder = folderIndex
                                            var nextFile = fileIndex + 1
                                            if (nextFile >= folder_files_model.count) {
                                                nextFile = 0
                                                //Найдем следующую непустую папку
                                                var found = false
                                                for (var i = folderIndex + 1; i < foldersView.model.count; i++) {
                                                    if (foldersView.model.get(folderIndex).filesModel.count > 0) {
                                                        nextFolder = i
                                                        found = true
                                                        break
                                                    }
                                                }
                                                if (!found) {
                                                    for (i = 0; i < folderIndex + 1; i++) {
                                                        if (foldersView.model.get(folderIndex).filesModel.count > 0) {
                                                            nextFolder = i
                                                            found = true
                                                            break
                                                        }
                                                    }
                                                }
                                                if (!found)
                                                    return
                                            }

                                            //Если следующий - это текущий
                                            if (nextFolder === folderIndex && nextFile === fileIndex) {
                                                audioPlayer.seek(0)
                                                audioPlayer.play()
                                                return
                                            }

                                            //Включим следующий
                                            foldersView.positionViewAtIndex(nextFolder, ListView.Beginning)
                                            childsView.positionViewAtIndex(nextFile, ListView.Center)
                                            var folder = userInfo.musicsModel.get(nextFolder)
                                            folder.additionalData.collapsed = true
                                            folder.filesModel.get(nextFile).additionalData.isPlaying = true
                                        }
                                    }
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
                for (var i = 0; i < userInfo.musicsModel.count; i++) {
                    var folder = userInfo.musicsModel.get(i)
                    if (folder.additionalData.isEdited) {
                        userInfo.musicsModel.remove(i)
                        userInfo.musicsModel.save()
                        break
                    }
                    var fileRemoved = false
                    for (var j = 0; j < folder.filesModel.count; j++) {
                        var file = folder.filesModel.get(j)
                        if (file.additionalData.isEdited) {
                            folder.filesModel.remove(j)
                            userInfo.musicsModel.save()
                            fileRemoved = true
                            break
                        }
                    }
                    if (fileRemoved)
                        break
                }
            } else if (editRole === "edit") {
                for (i = 0; i < userInfo.musicsModel.count; i++) {
                    folder = userInfo.musicsModel.get(i)
                    if (folder.additionalData.isEdited) {
                        folder.name = folder.additionalData.nameToSave
                        userInfo.musicsModel.save()
                        break
                    }
                    var fileEdited = false
                    for (j = 0; j < folder.filesModel.count; j++) {
                        file = folder.filesModel.get(j)
                        if (file.additionalData.isEdited) {
                            file.name = file.additionalData.nameToSave
                            userInfo.musicsModel.save()
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
            for (var i = 0; i < userInfo.musicsModel.count; i++) {
                var folder = userInfo.musicsModel.get(i)
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
        title: qsTr("Select audio") + applicationModel.settings.translatorStringEnd
        anchors.bottom: parent.bottom
        height: userInfoPage.height
        onVisibleChanged: userInfoPageBackButton.visible = !visible

        onAccepted: {
            for (var i = 0; i < userInfo.musicsModel.count; i++) {
                var folder = userInfo.musicsModel.get(i)
                if (folder.additionalData.isEdited) {
                    folder.filesModel.insertNew(0, fileUrl)
                    userInfo.musicsModel.save()
                    break
                }
                var fileAdd = false
                for (var j = 0; j < folder.filesModel.count; j++) {
                    var file = folder.filesModel.get(j)
                    if (file.additionalData.isEdited) {
                        folder.filesModel.insertNew(j + 1, fileUrl)
                        userInfo.musicsModel.save()
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
