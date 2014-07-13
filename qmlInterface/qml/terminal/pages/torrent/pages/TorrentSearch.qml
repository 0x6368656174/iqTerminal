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

    TorrentFoldersFilterModel {
        id: folderModel
        foldersModel: torrentSearchModel
    }

    Image {
        id: dog1
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: Core.dp(11)
        anchors.bottomMargin: Core.dp(22)*2 + editBar.anchors.bottomMargin
        source: "../../../images/96.png"
        fillMode: Image.PreserveAspectFit
        width: Core.dp(90)
    }

    Image {
        id: dog2
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -dog1.anchors.bottomMargin
        source: "../../../images/96a.png"
        fillMode: Image.PreserveAspectFit
        height: Core.dp(90)
    }

    ListView {
        anchors.fill: parent
        anchors.bottomMargin: Core.dp(22)*2 + editBar.anchors.bottomMargin - 1
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
                    anchors.right: availibleImage.left
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
                }

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

                    function selectAllChild(select) {
                        for(var i = 0; i < folder_files_model.count; i++) {
                            folder_files_model.get(i).additionalData.isSelect = select
                        }
                    }

                    onClicked: {
                        folder_additional_data.isSelect = !folder_additional_data.isSelect
                        selectAllChild(folder_additional_data.isSelect)
                    }
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
                                anchors.left: fileImage.right
                                anchors.right: fileCheckButton.left
                                anchors.top: parent.top
                                height: Core.dp(22)
                                elide: Text.ElideRight
                                verticalAlignment: Text.AlignVCenter
                                anchors.leftMargin: Core.dp(8)
                                anchors.rightMargin: Core.dp(8)
                                font.pixelSize: Core.dp(8)
                                text: file_name
                            }

                            //ВЫБОР
                            Button {
                                id: fileCheckButton
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.rightMargin: Core.dp(6)
                                width: Core.dp(14)
                                height: width
                                source: file_additional_data.isSelect?"../../../images/34a.png":"../../../images/34.png"
                                onClicked: {
                                    file_additional_data.isSelect = !file_additional_data.isSelect

                                    var isChildSelect = false
                                    for(var i = 0; i < folder_files_model.count; i++) {
                                        if(folder_files_model.get(i).additionalData.isSelect) {
                                            isChildSelect = true
                                            break
                                        }
                                    }
                                    folder_additional_data.isSelect = isChildSelect
                                }
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

    SearchBar {
        id: searchBar
        z: 2
        onSearch: {
            folderModel.filterString = text
        }
    }

    EditBar {
        id: editBar
        z: 1
        removeButtonEnabled: false
        editButtonEnabled: false
        selectAllButtonEnabled: true
        deselectAllButtonEnabled: true

        hideOnMissClick: false

        visible: anchors.bottomMargin > -Core.dp(22)

        anchors.bottom: searchBar.top
        anchors.bottomMargin: {
            folderModel.count > 0?0:-Core.dp(22)
        }

        onButtonClicked: {
            if (buttonType === "selectAll") {
                for (var i = 0; i < folderModel.count; i++) {
                    folderModel.get(i).additionalData.isSelect = true
                    for (var j = 0; j < folderModel.get(i).filesModel.count; j++) {
                        folderModel.get(i).filesModel.get(j).additionalData.isSelect = true
                    }
                }
            } else if (buttonType === "deselectAll") {
                for (i = 0; i < folderModel.count; i++) {
                    folderModel.get(i).additionalData.isSelect = false
                    for (j = 0; j < folderModel.get(i).filesModel.count; j++) {
                        folderModel.get(i).filesModel.get(j).additionalData.isSelect = false
                    }
                }
            }
        }
        onCansel: {
            editBar.editRole = ""
        }

        onSubmit: {
            for (var i = 0; i < folderModel.count; i++) {
                var oldFolder = folderModel.get(i)
                if (oldFolder.additionalData.isSelect) {
                    var newFolder = torrentDownloadModel.appendNew()
                    newFolder.name = folderModel.get(i).name
                    newFolder.sidsAvailability = folderModel.get(i).sidsAvailability

                    for (var j = 0; j < oldFolder.filesModel.count; j++) {
                        var oldFile = oldFolder.filesModel.get(j)
                        if (oldFile.additionalData.isSelect) {
                            var newFile = newFolder.filesModel.appendNew()
                            newFile.name = oldFile.name
                            newFile.path = oldFile.path
                            newFile.size = oldFile.size
                        }
                    }
                }
            }

            editBar.editRole = ""
            torrentDownloadModel.save()
        }
    }
}
