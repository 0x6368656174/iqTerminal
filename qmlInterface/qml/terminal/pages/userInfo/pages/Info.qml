import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"


Page {
    id: infoPage
    property bool readOnly: false
    QtObject {
        id: privateData
        property bool isEdited: false
        property bool photoIsEdited: false
        property bool nameIsEdited: false
    }

    name: "userInfo_info"

    Flickable
    {
        id: flickItem
        anchors.fill: parent
        interactive: contentHeight > height
        contentHeight: {
            if(photoRect.height + nameContainter.height + stateTextContainer.height + stateSpacer.height + statesView.contentHeight > parent.height)
                return parent.height + photoRect.height
            return parent.height
        }

        PropertyAnimation {
            id: showPhotoAmination
            target: flickItem
            property: "contentY"
            to :0
            duration: 200
        }

        /////////ФОТО//////////
        Rectangle {
            id: photoRect
            anchors.left: parent.left
            anchors.right: parent.right
            height: (132 * applicationModel.settings.zoomFactor)
            color: "#a0706a78"
            Rectangle {
                visible: privateData.photoIsEdited
                anchors.fill: parent
                color: "#ff5d1b"
            }

            Item {
                anchors.centerIn: parent
                width: photoImage.sourceSize.width > parent.width? parent.width:photoImage.sourceSize.width
                height: photoImage.sourceSize.height > parent.height? parent.height:photoImage.sourceSize.height
                Image {
                    id: photoImage
                    cache: false
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: userInfo?"image://xml/" + userInfo.userProfile.source:""
                }
            }

            ColorAnimation {
                id: photoPressAndHoldAnimation
                target: photoRect
                property: "color"
                from: "#a0706a78"
                to: "#ff5d1b"
                duration: 800
            }

            TerminalMouseArea {
                id: photoRectMa
                anchors.fill: parent
                visible: !privateData.isEdited && !infoPage.readOnly
                onPressedChanged: {
                    if (pressed) {
                        photoPressAndHoldAnimation.restart()
                    } else {
                        photoPressAndHoldAnimation.stop()
                        photoRect.color = "#a0706a78"
                    }
                }

                onPressAndHold: {
                    privateData.isEdited = true
                    privateData.photoIsEdited = true
                }
            }
        }

        /////////ИМЯ/////////

        Rectangle {
            id: nameContainter
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: photoRect.bottom
            height: (20 * applicationModel.settings.zoomFactor)
            color: "#c6c1c7"

            Rectangle {
                visible: privateData.nameIsEdited
                anchors.fill: parent
                color: "#ff5d1b"
            }

            Image {
                id: namePhotoImage
                cache: false
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: (2 * applicationModel.settings.zoomFactor)
                fillMode: Image.PreserveAspectFit
                source: userInfo?"image://xml/" + userInfo.userProfile.source:""
                scale: {
                    if (flickItem.contentY < photoRect.height - height)
                        return 0
                    if ((height - (photoRect.height - flickItem.contentY))/height > 1)
                        return 1
                    return (height - (photoRect.height - flickItem.contentY))/height
                }
            }

            Text {
                id: nameText
                visible: !privateData.nameIsEdited || (editBar.editRole !== "edit" && editBar.editRole !== "add")
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.leftMargin: (12 * applicationModel.settings.zoomFactor) + (namePhotoImage.width-(4 * applicationModel.settings.zoomFactor))*namePhotoImage.scale
                anchors.rightMargin: anchors.leftMargin
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                text: userInfo?userInfo.userProfile.name:""
                font.bold: true
            }

            TextInput {
                id: nameTextEditor
                clip: true
                visible: privateData.nameIsEdited && (editBar.editRole === "edit" || editBar.editRole === "add")
                anchors.fill: nameText
                selectByMouse: true
                selectionColor: "#c00d0d"
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                text: userInfo?userInfo.userProfile.name:""
                onVisibleChanged: {
                    if (visible) {
                        text = userInfo.userProfile.name
                        forceActiveFocus()
                    }
                }
                onAccepted: editBar.submit()
            }

            ColorAnimation {
                id: namePressAndHoldAnimation
                target: nameContainter
                duration: 800
                property: "color"
                from: "#c6c1c7"
                to: "#ff5d1b"
            }

            TerminalMouseArea {
                id: nameMa
                anchors.fill: parent
                visible: !privateData.isEdited
                onPressedChanged: {
                    if (infoPage.readOnly)
                        return
                    if (pressed) {
                        namePressAndHoldAnimation.restart()
                    } else {
                        namePressAndHoldAnimation.stop()
                        nameContainter.color = "#c6c1c7"
                    }
                }

                onPressAndHold: {
                    if (infoPage.readOnly)
                        return
                    privateData.isEdited = true
                    privateData.nameIsEdited = true
                }

                onClicked: {
                    showPhotoAmination.restart()
                }
            }
        }

        /////СТАТУСЫ////////
        Rectangle {
            id: stateTextContainer
            anchors.top: nameContainter.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: (20 * applicationModel.settings.zoomFactor)

            Text {
                anchors.fill: parent
                anchors.leftMargin: (12 * applicationModel.settings.zoomFactor)
                anchors.rightMargin: anchors.leftMargin
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                text: qsTr("State") + applicationModel.settings.translatorStringEnd
                color: "#da4504"
                font.bold: true
            }
        }

        Rectangle {
            id: stateSpacer
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: stateTextContainer.bottom
            height: 1
            color: "#c6c1c7"
        }

        CreateNewButton {
            id: createNewButton
            anchors.top: stateSpacer.bottom
            visible: userInfo?userInfo.userProfile.stateModel.count === 0:false
            enabled: !privateData.isEdited
            text: qsTr("New") + applicationModel.settings.translatorStringEnd
            spacerVisible: false
            onClicked: {
                var newSatate = userInfo.userProfile.stateModel.appendNew()
            }
        }


        ListView {
            id: statesView

            property int currentStateId: -1

            interactive: {
                if (contentHeight <= height)
                    return false
                if (contentHeight > height  && flickItem.contentY < photoRect.height)
                    return false
                return true
            }
            clip: true
            anchors.top: stateSpacer.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: infoPage.height - nameContainter.height - stateTextContainer.height - stateSpacer.height - photoRect.height + flickItem.contentY - (22 * applicationModel.settings.zoomFactor) - editBar.anchors.bottomMargin + 1
            model: userInfo?userInfo.userProfile.stateModel:undefined

            add: Transition {
                NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
                NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
            }

            displaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
            }


            delegate: Column {
                id: stateItem
                anchors.left: parent.left
                anchors.right: parent.right
                height: dataItem.height + 1

                function cancel() {
                    stateTextEditor.text = state_name
                    detailTextEditor.text = state_text
                }

                Rectangle {
                    id: dataItem
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: Math.max(detailText.paintedHeight, stateText.paintedHeight) + (12 * applicationModel.settings.zoomFactor)

                    Rectangle {
                        id: checkRect
                        visible: state_additional_data.isEdited
                        anchors.fill: parent
                        color: "#ff5d1b"
                    }

                    Text {
                        id: stateText
                        visible: !state_additional_data.isEdited || editBar.editRole !== "edit"
                        anchors.left: parent.left
                        anchors.leftMargin: (12 * applicationModel.settings.zoomFactor)
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: (50 * applicationModel.settings.zoomFactor)
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: state_name
                        wrapMode: Text.Wrap
                    }
                    TextInput {
                        id: stateTextEditor
                        visible: state_additional_data.isEdited && editBar.editRole === "edit"
                        onVisibleChanged: {
                            if (visible) {
                                text = state_name
                            }
                        }

                        anchors.fill: stateText
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: state_name
                        onTextChanged: state_additional_data.nameToSave = text
                        clip: true
                        onAccepted: editBar.submit()
                        KeyNavigation.tab: detailTextEditor
                        wrapMode: TextInput.Wrap
                    }

                    Text {
                        id: detailText
                        visible: !state_additional_data.isEdited || editBar.editRole !== "edit"
                        wrapMode: Text.Wrap
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: (64 * applicationModel.settings.zoomFactor)
                        anchors.right: parent.right
                        anchors.rightMargin: (12 * applicationModel.settings.zoomFactor)
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: state_text
                    }

                    TextInput {
                        id: detailTextEditor
                        visible: state_additional_data.isEdited && editBar.editRole === "edit"
                        onVisibleChanged: {
                            if (visible) {
                                text = state_text
                                forceActiveFocus()
                            }
                        }
                        wrapMode: Text.Wrap
                        anchors.fill: detailText
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: state_text
                        onTextChanged: state_additional_data.textToSave = text
                        clip: true
                        onAccepted: editBar.submit()
                        KeyNavigation.tab: stateTextEditor
                    }

                    ColorAnimation {
                        id: stateItemPressAndHoldAnimation
                        target: dataItem
                        property: "color"
                        from: "white"
                        to: "#ff5d1b"
                        duration: 800
                    }

                    TerminalMouseArea {
                        id: ma
                        anchors.fill: parent
                        visible: !privateData.isEdited && !infoPage.readOnly
                        onPressedChanged: {
                            if (pressed) {
                                stateItemPressAndHoldAnimation.restart()
                            } else {
                                stateItemPressAndHoldAnimation.stop()
                                dataItem.color = "white"
                            }
                        }

                        onPressAndHold: {
                            privateData.isEdited = true
                            state_additional_data.isEdited = true
                        }

                    }
                }

                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 1
                    color: "#c6c1c7"
                }
            }
        }
    }

    EditBar {
        id: editBar
        visible: opacity !== 0
        opacity: privateData.isEdited?1:0
        anchors.bottomMargin: privateData.isEdited?0:-(22 * applicationModel.settings.zoomFactor)
        hideOnMissClick: false
        canselButtonEnabled: true

        property bool photoIsEdited
        property bool nameIsEdited
        property bool stateIsEdited

        addButtonEnabled: {
            if (photoIsEdited) {
                if (userInfo.userProfile.photoIsNull()) {
                    return true
                } else {
                    return false
                }
            } else if (nameIsEdited) {
                if (userInfo.userProfile.name === "") {
                    return true
                } else {
                    return false
                }
            } else if (stateIsEdited) {
                return true
            }
            return false
        }

        removeButtonEnabled: {
            if (photoIsEdited) {
                if (!userInfo.userProfile.photoIsNull()) {
                    return true
                } else {
                    return false
                }
            } else if (nameIsEdited) {
                if (userInfo.userProfile.name !== "") {
                    return true
                } else {
                    return false
                }
            } else if (stateIsEdited) {
                return true
            }
            return false
        }

        editButtonEnabled: {
            if (photoIsEdited) {
                if (!userInfo.userProfile.photoIsNull()) {
                    return true
                } else {
                    return false
                }
            } else if (nameIsEdited) {
                if (userInfo.userProfile.name !== "") {
                    return true
                } else {
                    return false
                }
            } else if (stateIsEdited) {
                return true
            }
            return false
        }

        onButtonClicked: {
            if (photoIsEdited) {
                if (buttonType === "add" || buttonType === "edit") {
                    fileDialog.open()
                    cansel()
                }
            }
        }

        onSubmit: {
            if (photoIsEdited) {
                if (editRole === "remove") {
                    userInfo.userProfile.removePhoto()
                    userInfo.userProfile.save()
                    reloadPhotos()
                }
            } else if (nameIsEdited) {
                if (editRole === "edit" || editRole === "add") {
                    userInfo.userProfile.name = nameTextEditor.text
                } else if (editRole === "remove") {
                    userInfo.userProfile.name = ""
                }
                userInfo.userProfile.save()
            } else if (stateIsEdited) {
                for (var i = 0; i < userInfo.userProfile.stateModel.count; i++) {
                    var state = userInfo.userProfile.stateModel.get(i)
                    if(state.additionalData.isEdited) {
                        if (editRole === "add") {
                            userInfo.userProfile.stateModel.insertNew(i+1)
                        } else if (editRole === "edit") {
                            state.name = state.additionalData.nameToSave
                            state.text = state.additionalData.textToSave
                        } else if (editRole === "remove") {
                            userInfo.userProfile.stateModel.remove(i)
                        }
                        userInfo.userProfile.save()
                        break
                    }
                }
            }

            cansel()
        }

        onCansel: {
            privateData.photoIsEdited = false
            privateData.nameIsEdited = false
            nameTextEditor.text = userInfo.userProfile.name
            for (var i = 0; i < userInfo.userProfile.stateModel.count; i++) {
                userInfo.userProfile.stateModel.get(i).additionalData.isEdited = false
            }
            privateData.isEdited = false
        }

        onVisibleChanged: {
            if (visible){
                photoIsEdited = privateData.photoIsEdited
                nameIsEdited = privateData.nameIsEdited
                stateIsEdited = false
                for (var i = 0; i < userInfo.userProfile.stateModel.count; i++) {
                    if(userInfo.userProfile.stateModel.get(i).additionalData.isEdited) {
                        stateIsEdited = true
                        break
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        selectMultiple: false
        title: qsTr("Select image") + applicationModel.settings.translatorStringEnd

        onAccepted: {
            userInfo.userProfile.setPhoto(fileUrl)
            userInfo.userProfile.save()
            reloadPhotos()
        }
    }

    function reloadPhotos() {

        var oldSource = photoImage.source
        photoImage.source = ""
        namePhotoImage.source = ""
        photoImage.source = oldSource
        namePhotoImage.source = oldSource
    }
}
