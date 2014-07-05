import QtQuick 2.0
import QtQuick.Dialogs 1.1
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"

Page {
    id: downloadPage
    name: "torrent_download"

    property string torrentXmlPath

    QtObject {
        id: privateData
        property bool isEdited: false
    }

    FoldersModel {
        id: folderModel
        source: torrentXmlPath
        parentElement: "torrent"
        folderAdditionalData: QtObject {
            property bool collapsed: false
            property bool isEdited: false
            property bool isSelect: false

            onIsEditedChanged: privateData.isEdited = isEdited
        }
    }

    ListView {
        anchors.fill: parent
        model: folderModel
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

        delegate: Rectangle {
            id: folderDelegate

            anchors.left: parent.left
            anchors.right: parent.right
            height: Core.dp(22) + collapsedContainer.height

            Connections {
                target: editBar
                onButtonClicked: {
                    if (buttonType === "selectAll") {
                        folder_additional_data.isSelect = true
                    } else if (buttonType === "deselectAll") {
                        folder_additional_data.isSelect = false
                    }

//                    else if (buttonType !== "add" || !folder_additional_data.isEdited) {
//                        return
//                    } else if (buttonType === "add") {
//                        fileDialog.open()
//                    }
                }
                onCansel: {
                    editBar.editRole = ""

                    //                    if (!folder_additional_data.isEdited)
//                        return

//                    folder_additional_data.isEdited = false
//                    folderTextEditor.text = folderText.text
                }

                onSubmit: {
                    if (folder_additional_data.isSelect)
                        folderModel.remove(index)
                    editBar.editRole = ""

//                    if (!folder_additional_data.isEdited)
//                        return

//                    if (editBar.editRole === "add") {
//                        folderModel.insertNew(index + 1, fileDialog.fileUrl)
//                        folder_additional_data.isEdited = false
//                    } else if (editBar.editRole === "remove") {
//                        folderModel.remove(index)
//                        privateData.isEdited = false
//                    } else if (editBar.editRole === "edit") {
//                        folderModel.get(index).name = folderTextEditor.text
//                        folder_additional_data.isEdited = false
//                    }

                    folderModel.save()
                }
            }

            Rectangle {
                id: folderNameContainer
                anchors.left: parent.left
                anchors.right: parent.right
                height: Core.dp(22)
                color: folder_additional_data.collapsed?"#da4504":"#c6c1c7"
                Behavior on color {ColorAnimation { duration: 200 } }

                Item {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: pauseButton.left
                    clip: true

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
                        visible: !folder_additional_data.isEdited || editBar.editRole !== "edit"
                        anchors.left: folderImage.right
                        anchors.leftMargin: Core.dp(8)
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: parent.height / 2 - Core.dp(2)
                        anchors.right: parent.right
                        anchors.rightMargin: anchors.leftMargin
                        elide: Text.ElideRight
                        font.pixelSize: Core.dp(8)
                        verticalAlignment: Text.AlignBottom
                        color: folder_additional_data.collapsed?"white":"black"
                        Behavior on color {ColorAnimation { duration: 200 } }
                        text: folder_name
                    }

                    TextInput {
                        id: folderTextEditor
                        clip: true
                        visible: !folderText.visible
                        anchors.fill: folderText
                        text: folderText.text
                        color: folderText.color
                        font.pixelSize: folderText.font.pixelSize
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        verticalAlignment: folderText.verticalAlignment
                        onVisibleChanged: {
                            if(visible) {
                                text = folderText.text
                                forceActiveFocus()
                            }
                        }
                        onAccepted: editBar.submit()
                    }

                    //РАЗМЕР
                    Text {
                        id: folderSizeTitle
                        anchors.left: folderText.left
                        anchors.top: folderText.bottom
                        font.pixelSize: Core.dp(6)
                        color: folderText.color
                        text: qsTr("Размер")
                    }

                    Text {
                        id: folderSizeText
                        anchors.verticalCenter: folderSizeTitle.verticalCenter
                        anchors.left: folderSizeTitle.right
                        anchors.leftMargin: Core.dp(3)
                        font.pixelSize: folderSizeTitle.font.pixelSize
                        color: folderText.color
                        font.bold: true
                        text: Core.humanReadableSize(folder_size)
                    }

                    //СТАТУС
                    Text {
                        id: folderStateTitle
                        anchors.left: folderText.left
                        anchors.leftMargin: Core.dp(60)
                        anchors.top: folderText.bottom
                        font.pixelSize: Core.dp(6)
                        color: folderText.color
                        text: qsTr("Статус")
                    }

                    Text {
                        id: folderStateText
                        anchors.verticalCenter: folderStateTitle.verticalCenter
                        anchors.left: folderStateTitle.right
                        anchors.leftMargin: Core.dp(3)
                        font.pixelSize: folderStateTitle.font.pixelSize
                        color: folderText.color
                        text: (folder_downloaded_size/folder_size*100).toFixed(0) + "%"
                    }
                }
//                Rectangle {
//                    anchors.fill: parent
//                    border.color: folder_additional_data.collapsed?"white":"#da4504"
//                    border.width: 2
//                    anchors.margins: 2
//                    opacity: folder_additional_data.isEdited?1:0
//                    color: "transparent"
//                }

//                Rectangle {
//                    id: folderPressAndHoldRect
//                    anchors.fill: parent
//                    border.color: folder_additional_data.collapsed?"white":"#da4504"
//                    border.width: 2
//                    anchors.margins: 2
//                    opacity: 0
//                    color: "transparent"
//                }

//                PropertyAnimation {
//                    id: folderPressAndHoldAmiation
//                    target: folderPressAndHoldRect
//                    property: "opacity"
//                    from: 0
//                    to: 1
//                    duration: 800
//                }

                SequentialAnimation {
                    id: folderClickAmination
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
                    visible: !privateData.isEdited
                    onClicked: {
                        folderClickAmination.restart()
                        folder_additional_data.collapsed = !folder_additional_data.collapsed
                    }
//                    onPressedChanged: {
//                        if (pressed) {
//                            folderPressAndHoldAmiation.restart()
//                        } else {
//                            folderPressAndHoldAmiation.stop()
//                            folderPressAndHoldRect.opacity = 0
//                        }
//                    }

//                    onPressAndHold: {
//                        folder_additional_data.isEdited = true
//                    }
                }

                //ПАУЗА
                Button {
                    id: pauseButton
                    width: Core.dp(9)
                    height: width
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: loadingImage.left
                    anchors.rightMargin: Core.dp(4)
                    source: folder_loading_in_process?"../../../images/51b.png":"../../../images/51a.png"
                    onClicked: {
                        folderModel.get(index).loadingInProcess = !folderModel.get(index).loadingInProcess
                        folderModel.save()
                    }
                }

                //ЗАГРУЖАЕТСЯ
                Image {
                    id: loadingImage
                    width: Core.dp(10)
                    height: width
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: availibleImage.left
                    anchors.rightMargin: Core.dp(4)
                    source: folder_loading_in_process?"../../../images/99a.png":"../../../images/99.png"
                }

                //ДОСТУПНОСТЬ
                Image {
                    id: availibleImage
                    width: Core.dp(10)
                    height: width
                    fillMode: Image.PreserveAspectFit
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: checkButton.left
                    anchors.rightMargin: Core.dp(4)
                    source: {
                        if (folder_sids_availability === 0) {
                            return "../../../images/97.png"
                        } else if (folder_sids_availability === 1) {
                            return "../../../images/97a.png"
                        } else if (folder_sids_availability === 2) {
                            return "../../../images/97b.png"
                        } else if (folder_sids_availability === 3) {
                            return "../../../images/97c.png"
                        }
                    }
                }

                //ВЫБОР
                Button {
                    id: checkButton
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: Core.dp(6)
                    width: Core.dp(14)
                    height: width
                    source: folder_additional_data.isSelect?"../../../images/34a.png":"../../../images/34.png"
                    onClicked: folder_additional_data.isSelect = !folder_additional_data.isSelect
                }

                Item {
                    id: collapsedContainer
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: folderNameContainer.bottom
                    height: {
                        if (!folder_additional_data.collapsed) {
                            return 0
                        }
                        return Core.dp(22)*folder_files_model.count + 1
                    }

                    clip: true

                    Behavior on height {NumberAnimation {duration: 200 } }

                    ListView {
                        id: childsView
                        anchors.topMargin: 1
                        anchors.fill: parent
                        interactive: false

                        add: Transition {
                            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
                            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
                        }

                        displaced: Transition {
                            NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
                        }

                        model: folder_files_model
                        delegate: Rectangle {
                            id: childDelegate

                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: Core.dp(22)

                            Text {
                                id: childNameText
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.top: parent.top
                                height: Core.dp(22)
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                anchors.leftMargin: Core.dp(34)
                                anchors.rightMargin: Core.dp(8)
                                font.pixelSize: Core.dp(8)
                                text: file_name
                            }


                            Behavior on height {NumberAnimation {duration: 200} }

                            Rectangle {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                height: 1
                                color: "#c6c1c7"
                            }
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
        selectAllButtonEnabled: true
        deselectAllButtonEnabled: true

        hideOnMissClick: false

//        visible: opacity !== 0
//        opacity: privateData.isEdited?1:0
//        anchors.bottomMargin: privateData.isEdited?0:-Core.dp(22)
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Выберите папку")
        selectFolder: true
        selectMultiple: false
        selectExisting: true
        onAccepted: editBar.submit()
        onRejected: editBar.cansel()
    }
}
