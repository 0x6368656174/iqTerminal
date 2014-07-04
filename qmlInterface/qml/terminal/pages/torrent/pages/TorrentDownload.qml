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
            property bool collapsed: false
            property bool isRemoved: false /*foldersView.currentFolderId === folderId && !foldersView.isChildrenEdited && editBar.editRole === "remove"*/
            property bool isAddAfter: false /*foldersView.currentFolderId === folderId && !foldersView.isChildrenEdited && editBar.editRole === "add"*/
//            property int folderId: folder_id
//            property alias nameToSave: folderTextEditor.text
//            property alias playedChildId: childsView.playedChildId
//            property alias isPlaying: childsView.isPlaying


            anchors.left: parent.left
            anchors.right: parent.right
            height: Core.dp(22)/* + collapsedContainer.height*/

            Rectangle {
                id: folderNameContainer
                anchors.left: parent.left
                anchors.right: parent.right
                height: Core.dp(22)
                color: folderDelegate.collapsed?"#da4504":"#c6c1c7"
                Behavior on color {ColorAnimation { duration: 200 } }

                Image {
                    id: folderImage
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: Core.dp(6)
                    height: Core.dp(15)
                    width: Core.dp(17)
                    fillMode: Image.PreserveAspectFit
                    source: folderDelegate.collapsed?"../../../images/47.png":"../../../images/40.png"
                }

                Text {
                    id: folderText
//                    visible: foldersView.currentFolderId !== folderDelegate.folderId  || editBar.editRole !== "edit"  || foldersView.isChildrenEdited
                    visible: true
                    anchors.left: folderImage.right
                    anchors.leftMargin: Core.dp(8)
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: anchors.leftMargin
                    elide: Text.ElideRight
                    font.pixelSize: Core.dp(8)
                    verticalAlignment: Text.AlignVCenter
//                    color: folderDelegate.collapsed?"white":"black"
                    Behavior on color {ColorAnimation { duration: 200 } }
                    text: folderTextEditor.text
                }

                TextInput {
                    id: folderTextEditor
                    clip: true
//                    visible: foldersView.currentFolderId === folderDelegate.folderId && editBar.editRole === "edit" && !foldersView.isChildrenEdited
                    visible: false
                    anchors.fill: folderText
                    text: folder_name
                    color: folderText.color
                    font.pixelSize: Core.dp(8)
                    selectByMouse: true
                    selectionColor: "#c00d0d"
                    verticalAlignment: Text.AlignVCenter
                    onVisibleChanged: {
                        if(visible) {
                            forceActiveFocus()
                        }
                    }
//                    onAccepted: editBar.submit()
                }

//                Rectangle {
//                    anchors.fill: parent
//                    border.color: folderDelegate.collapsed?"white":"#da4504"
//                    border.width: 2
//                    anchors.margins: 2
//                    opacity: foldersView.currentFolderId === folderDelegate.folderId && !foldersView.isChildrenEdited?1:0
//                    color: "transparent"
//                }

//                Rectangle {
//                    id: folderPressAndHoldRect
//                    anchors.fill: parent
//                    border.color: folderDelegate.collapsed?"white":"#da4504"
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

//                SequentialAnimation {
//                    id: photoFolderClickAmination
//                    PropertyAnimation {
//                        target: folderImage
//                        property: "scale"
//                        from: 1.0
//                        to: 1.2
//                        duration: 200
//                    }
//                    ScriptAction {
//                        script: {
//                            folderImage.scale = 1.0
//                        }
//                    }
//                }

//                TerminalMouseArea {
//                    anchors.fill: parent
//                    visible: !foldersView.isEdited
//                    onClicked: {
//                        photoFolderClickAmination.restart()
//                        folderDelegate.collapsed = !folderDelegate.collapsed
//                    }
//                    onPressedChanged: {
//                        if (pressed) {
//                            folderPressAndHoldAmiation.restart()
//                        } else {
//                            folderPressAndHoldAmiation.stop()
//                            folderPressAndHoldRect.opacity = 0
//                        }
//                    }

//                    onPressAndHold: {
//                        foldersView.currentFolderId = folder_id
//                    }
//                }
            }
        }
    }
}
