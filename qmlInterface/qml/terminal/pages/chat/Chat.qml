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

    function send() {
        console.log("send")
        if (attachmentBar.role === "" && textInputText.text === "")
            return
        var newMessage = messagesModel.appendNew()
        newMessage.direction = Message.Outgoing
        newMessage.text = textInputText.text
        if (attachmentBar.role === "file") {
            newMessage.type = Message.File
            newMessage.filePath = attachmentBar.attachmentFileName
        } else {
            newMessage.type = Message.Text
        }

        attachmentBar.role = ""
        attachmentBar.attachmentFileName = ""
        textInputText.text = ""
        addButton.checked = false
    }

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
        parentElement: "chat"
        source: Core.dataDir + "/chats/" + chatPage.userProfile

        itemAdditionalData: QtObject {
            property bool collapsed: false
            property bool isPlaying: false
        }

        onCountChanged: {
            var pos = chatView.contentY;
            var destPos;
            chatView.positionViewAtEnd()
            destPos = chatView.contentY
            scrollAnimation.from = pos
            scrollAnimation.to = destPos
            scrollAnimation.restart()
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

    NumberAnimation {
        id: scrollAnimation
        target: chatView
        property: "contentY"
        duration: 250
    }

    ListView {
        id: chatView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleRect.bottom
        anchors.bottom: firstButtonRow.top
        interactive: contentHeight > height
        model: messagesModel
        spacing: Core.dp(2)
        clip: true

        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
        }

        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }

        delegate:
            Item {
            id: messageDelegate
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: message_direction === Message.Incoming?0:parent.width/3
            anchors.rightMargin: message_direction === Message.Outgoing?0:parent.width/3
            clip: true

            height: {
                if (message_additional_data.isPlaying) {
                    return Core.dp(74)
                } else if (message_additional_data.collapsed) {
                    return collapsedText.height + Core.dp(20)
                }
                return Core.dp(36)
            }

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
                    anchors.verticalCenter: text.verticalCenter
                    anchors.left: parent.left
                    width: Core.dp(28)
                    height: width
                    visible: message_type !== Message.Text
                    source: {
                        if (message_was_read) {
                            if (message_type === Message.Image) {
                                return "../../images/61c.png"
                            } else if (message_type === Message.Audio) {
                                return "../../images/74c.png"
                            } else if (message_type === Message.Video) {
                                return "../../images/73c.png"
                            } else if (message_type === Message.File) {
                                return "../../images/75c.png"
                            }
                        }else {
                            if (message_type === Message.Image) {
                                return "../../images/61b.png"
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
                    height: Core.dp(36)
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

                Connections {
                    target: message_additional_data
                    onIsPlayingChanged: {
                        if (message_additional_data.isPlaying) {
                            audioPlayer.source = message_file_path
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
                        } else if (message_type === Message.Image) {
                            photoView.text = message_text
                            photoView.source = message_file_path
                            photoView.show()
                        } else if (message_type === Message.Video) {
                            videoPlayer.text = message_text
                            videoPlayer.source = message_file_path
                            videoPlayer.show()
                        } else if (message_type === Message.Audio) {
                            var newIsPlayin = !message_additional_data.isPlaying
                            for (var i = 0; i < messagesModel.count; i++) {
                                messagesModel.get(i).additionalData.isPlaying = false
                            }

                            message_additional_data.isPlaying = newIsPlayin
                        }
                    }
                }

                MediaButtons {
                    id: mediaButtons
                    visible: message_type === Message.Audio
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: text.bottom
                    position: audioPlayer.position
                    duration: audioPlayer.duration
                    isPlay: message_additional_data.isPlaying && audioPlayer.isPlay
                    isPause: message_additional_data.isPlaying && audioPlayer.isPause

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
        anchors.bottom: attachmentBar.top
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
                id: sendButton
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 56
                onClicked: chatPage.send()
            }
        }
    }

    Item {
        id: attachmentBar
        property string role
        property string attachmentFileName
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: textInput.top

        height: {
            if (role === "file") {
                return Core.dp(22)
            }
            return 0
        }

        Behavior on height {NumberAnimation { duration: 200;} }

        Rectangle {
            id: fileAttachment
            anchors.fill: parent
            visible: attachmentBar.role === "file"
            color: "#cdcdcd"

            Image {
                id: fileAttachmentImage
                source: "../../images/75c.png"
                height: parent.height
                width: height
                fillMode: Image.PreserveAspectFit
            }

            Text {
                anchors.left: fileAttachmentImage.right
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Core.dp(8)
                anchors.right: parent.right
                anchors.rightMargin: font.pixelSize
                elide: Text.ElideRight
                text: attachmentBar.attachmentFileName
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
            onAccepted: chatPage.send()
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
                id: attachmentButton
                width: (parent.width - 3)/4
                height: parent.height
                imageNumber: 75
                onClicked: fileChoiceDialog.open()
            }
        }
    }

    PhotoView {
        id: photoView
        anchors.fill: parent
    }

    VideoPlayer {
        id: videoPlayer
        property url source
        anchors.fill: parent

        Behavior on opacity {NumberAnimation {duration: 200} }
        visible: opacity !== 0
        opacity: 0
        file: source

        onHiden: opacity = 0

        function show() {
            opacity = 1
            play()
        }
    }

    FileDialog {
        id: fileChoiceDialog
        selectMultiple: false
        title: qsTr("Выбирите файл")

        onAccepted: {
            attachmentBar.role = "file"
            attachmentBar.attachmentFileName = fileUrl
            addButton.checked = false
        }

        onRejected: {
            attachmentBar.role = ""
            attachmentBar.attachmentFileName = ""
        }
    }
}
