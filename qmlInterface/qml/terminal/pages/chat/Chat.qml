import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtMultimedia 5.2
import ".."
//import "pages"
import "../../elements"


Page {
    id: chatPage
    property string userProfile

    name: "chat"

    BackButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: Core.dp(70)
    }

    UserProfile {
        id: userProfileModel
        source: Core.dataDir + "/users/" + chatPage.userProfile
        parentElement: "user/info"

        stateModel.itemAdditionalData: QtObject {
            property bool isEdited: false
            property string nameToSave: ""
            property string textToSave: ""
        }
    }

    MessagesModel {
        id: messagesModel
        source: Core.dataDir + "/chats/" + chatPage.userProfile
        parentElement: "chat"

        itemAdditionalData: QtObject {
            property bool collapsed: false
        }
    }




    Rectangle {
        id: titleRect
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: Core.dp(40)
        Rectangle
        {
            id: photoItem
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: "#6c676e"
            width: height
            Item{
                anchors.centerIn: parent
                width: photoImage.sourceSize.width > parent.width? parent.width:photoImage.sourceSize.width
                height: photoImage.sourceSize.height > parent.height?parent.height:photoImage.sourceSize.height
                Image {
                    id: photoImage
                    anchors.fill: parent
                    source: "image://xml/" + userProfileModel.source
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        Text {
            anchors.left: photoItem.right
            anchors.bottom: photoItem.bottom
            font.pixelSize: Core.dp(6)
            anchors.leftMargin: font.pixelSize
            anchors.bottomMargin: font.pixelSize
            anchors.right: parent.right
            elide: Text.ElideRight
            text: userProfileModel.name
        }
    }



    ListView {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleRect.bottom
        anchors.bottom: firstButtonRow.top
        interactive: contentHeight > height
        model: messagesModel
        spacing: Core.dp(2)
        clip: true
        delegate:
            Item {
                id: messageDelegate
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: message_direction === Message.Incoming?0:parent.width/3
                anchors.rightMargin: message_direction === Message.Outgoing?0:parent.width/3
                height: !message_additional_data.collapsed?Core.dp(36):collapsedText.height + Core.dp(20)
                Image {
                    id: triangleOne
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: height
                    height: message_direction === Message.Incoming?Core.dp(4):0
                    anchors.leftMargin: Core.dp(4)
                    source: message_was_read?"../../images/51c.png":"../../images/51b.png"
                }

                Rectangle {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: triangleOne.bottom
                    anchors.bottom: triangleTwo.top
                    color: message_was_read?"#cdcdcd":"#f25d26"

                    Image {
                        id: typeImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        width: Core.dp(28)
                        height: width
                        visible: message_type !== Message.Text
                        source: {
                            if (message_was_read) {
                                if (message_type === Message.Image) {
                                    return "../../images/72c.png"
                                } else if (message_type === Message.Audio) {
                                    return "../../images/74c.png"
                                } else if (message_type === Message.Video) {
                                    return "../../images/73c.png"
                                } else if (message_type === Message.File) {
                                    return "../../images/75c.png"
                                }
                            }else {
                                    if (message_type === Message.Image) {
                                        return "../../images/72b.png"
                                    } else if (message_type === Message.Audio) {
                                        return "../../images/74b.png"
                                    } else if (message_type === Message.Video) {
                                        return "../../images/73b.png"
                                    } else if (message_type === Message.File) {
                                        return "../../images/75b.png"
                                }
                            }
                            return ""
                        }
                    }

                    Text {
                        id: text
                        visible: !message_additional_data.collapsed
                        anchors.left: typeImage.right
                        anchors.right: parent.right
                        anchors.rightMargin: Core.dp(8)
                        verticalAlignment: Text.AlignVCenter
                        maximumLineCount: 1
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        elide: Text.ElideRight
                        text: message_text
                        font.pixelSize: Core.dp(8)
                        color: "white"
                    }

                    Text {
                        id: collapsedText
                        visible: message_additional_data.collapsed
                        anchors.left: typeImage.right
                        anchors.right: parent.right
                        anchors.rightMargin: Core.dp(8)
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WordWrap
                        text: message_text
                        font.pixelSize: Core.dp(8)
                        color: "white"
                    }


                    TerminalMouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (message_direction === Message.Incoming) {
                                if (collapsedText.lineCount > 1) {
                                    if (message_additional_data.collapse)
                                        messagesModel.get(index).wasRead = true
                                } else {
                                    messagesModel.get(index).wasRead = true
                                }
                            }

                            if (message_type === Message.Text) {
                                if (collapsedText.lineCount > 1)
                                    message_additional_data.collapsed = !message_additional_data.collapsed
                            }
                        }
                    }
                }

                Image {
                    id: triangleTwo
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    rotation: 180
                    width: height
                    height: message_direction === Message.Outgoing?Core.dp(4):0
                    anchors.rightMargin: Core.dp(4)
                    source: triangleOne.source
                }
            }
    }





    Rectangle {
        id: firstButtonRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: textInput.top
        height: textInput.height
        color: "#b4b4b4"
        Row {
            anchors.fill: parent
            anchors.margins: 1
            spacing: 1
            ChatButton {
                id: addButton
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 53
                onClicked: addButton.checked = !addButton.checked
            }
            ChatButton {
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 54
            }
            ChatButton {
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 55
            }
            ChatButton {
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 56
            }
        }
    }

    Rectangle {
        id: textInput
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: secondButtonRow.top
        height: Core.dp(22)
        color: "white"

        signal send(var text)

        Text {
            anchors.centerIn: parent
            font.pixelSize: Core.dp(8)
            color: "#c6c1c7"
            text: qsTr("input text field")
            opacity: textInputText.text === "" && !textInputText.focus?1:0.5
        }

        TextInput {
            id: textInputText
            clip: true
            anchors.left: parent.left
            anchors.right: smileButton.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: Core.dp(8)
            anchors.rightMargin: Core.dp(8)
            font.pixelSize: Core.dp(8)
            verticalAlignment: Text.AlignVCenter
            selectByMouse: true
            selectionColor: "#c00d0d"
            onAccepted: textInput.send(textInputText.text)
        }

        ChatButton {
            id: smileButton
            width: height
            anchors.right: parent.right
            height: parent.height
            imageNumber: 78
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: "#c6c1c7"
        }
    }

    Rectangle {
        id: secondButtonRow
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: addButton.checked?0:-height
        height: textInput.height
        color: "#b4b4b4"
        Behavior on anchors.bottomMargin {NumberAnimation { duration: 200} }
        Row {
            anchors.fill: parent
            anchors.margins: 1
            spacing: 1
            ChatButton {
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 72
            }
            ChatButton {
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 73
            }
            ChatButton {
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 74
            }
            ChatButton {
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 75
            }
        }
    }
}
