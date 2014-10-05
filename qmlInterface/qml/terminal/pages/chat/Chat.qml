import QtQuick 2.2
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
        var plainText = TextDecorator.toPlainText(textInputText.getFormattedText(0, textInputText.length))
        if (attachmentBar.role === "" && audioRecordBar.role === "" && plainText === "")
            return
        var newMessage = messagesModel.appendNew()
        newMessage.sendDateTime = new Date()
        newMessage.direction = Message.Outgoing
        newMessage.text = plainText
        if (attachmentBar.role === "file") {
            newMessage.type = Message.File
            newMessage.filePath = attachmentBar.attachmentFileName
        } else if (audioRecordBar.role !== "" && audioRecordBar.role !== "record") {
            audioRecorder.stop()
            newMessage.type = Message.Audio
            newMessage.filePath = audioRecordBar.source
        } else
        {
            newMessage.type = Message.Text
        }

        attachmentBar.role = ""
        audioRecordBar.role = ""
        audioRecordBar.source = ""
        audioButton.checked = false
        attachmentBar.attachmentFileName = ""
        textInputText.text = ""
        addButton.checked = false

        messagesModel.save()
    }

    BackButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: Core.dp(70)
    }

    UserProfile {
        id: userProfileModel
        source: Core.dataDir + "users/" + chatPage.userProfile
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
        source: Core.dataDir + "chats/" + chatPage.userProfile

        itemAdditionalData: QtObject {
            property bool collapsed: false
            property bool isPlaying: false
        }

        onCountChanged: {
            chatView.animatedPosition(count - 1)
        }
    }

    MessagesFilterModel {
        id: messageFilterModel
        messagesModel: messagesModel
    }


    Rectangle {
        id: titleRect
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: Core.dp(20)
        color: "#c6c1c7"
        Item
        {
            id: photoItem
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
//            color: "#6c676e"
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
            anchors.top: photoItem.top
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: Core.dp(8)
            anchors.leftMargin: font.pixelSize
            anchors.rightMargin: font.pixelSize
            anchors.right: parent.right
            elide: Text.ElideRight
            text: userProfileModel.name
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                userInfoPage.userProfile = userProfile
                showRightPage(userInfoPage.name)
            }
        }
    }

    ListView {
        id: chatView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleRect.bottom
        anchors.bottom: audioRecordBar.top
        interactive: contentHeight > height
        model: messageFilterModel
        spacing: Core.dp(2)
        clip: true

        add: Transition {
            NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
            NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
        }

        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 1000 }
        }

        function animatedPosition(index) {
            chatView.positionViewAtIndex(index, ListView.Contain)
        }

        delegate:
            Item {
            id: messageDelegate
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: message_direction === Message.Incoming?0:parent.width/10
            anchors.rightMargin: message_direction === Message.Outgoing?0:parent.width/10
            clip: true

            height: {
                if (message_text === "") {
                    if (message_type === Message.Audio)
                        return Core.dp(50)
                    return Core.dp(36)
                } else if (message_additional_data.isPlaying) {
                    return Core.dp(74)
                } else if (message_type == Message.Text && messageText.lineCount > 1) {
                    return messageText.paintedHeight + Core.dp(28)
                }
                return Core.dp(38)
            }
            Connections {
                target: message_additional_data
                onCollapsedChanged: posTimer.start()
            }

            Timer {
                id: posTimer
                interval: 10
                repeat: false
                onTriggered: chatView.animatedPosition(index)
            }

            Image {
                id: triangleOne
                anchors.left: parent.left
                anchors.top: parent.top
                width: height
                height: message_direction === Message.Incoming?Core.dp(4):0
                anchors.leftMargin: Core.dp(4)
                source: "../../images/51cb.png"
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: triangleOne.bottom
                anchors.bottom: triangleTwo.top
                color: message_direction === Message.Incoming?"#f25d26":"#888888"

                Image {
                    id: typeImage
                    anchors.verticalCenter: messageText.verticalCenter
                    anchors.left: parent.left
                    width: message_type !== Message.Text?Core.dp(28):0
                    height: width
                    visible: message_type !== Message.Text
                    source: {
                        if (message_direction === Message.Outgoing) {
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
                    id: messageText
                    anchors.left: typeImage.right
                    anchors.leftMargin: message_type !== Message.Text?0:Core.dp(8)
                    anchors.right: parent.right
                    anchors.rightMargin: Core.dp(8)
                    anchors.topMargin: Core.dp(12)
                    anchors.top: parent.top
                    wrapMode: Text.WordWrap
                    text: TextDecorator.toFormattedText(message_text)
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
                    if (message_additional_data.isPlaying)
                        audioPlayer.stop()
                }

                TerminalMouseArea {
                    anchors.fill: parent
                    onClicked: {
                        if (message_direction === Message.Incoming) {
                            if (messageText.lineCount > 1) {
                                if (message_additional_data.collapse)
                                    message_was_read = true
                            } else {
                                message_was_read = true
                            }
                            messagesModel.save()
                        }

                        if (message_type === Message.Text) {

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
                            if (newIsPlayin) {
                                audioPlayer.stop()
                                audioPlayer.source = ""
                            }
                            for (var i = 0; i < messagesModel.count; i++) {
                                messagesModel.get(i).additionalData.isPlaying = false
                            }

                            message_additional_data.isPlaying = newIsPlayin
                        } else if (message_type === Message.File) {
                            fileSaveDialog.fileToSave = message_file_path
                            fileSaveDialog.open()
                        }
                    }
                }

                MediaButtons {
                    id: mediaButtons
                    style: message_direction === Message.Outgoing?"orange":"white"
                    visible: {
                        if (message_type !== Message.Audio) {
                            return false
                        } else if (message_text !== "" && message_additional_data.isPlaying) {
                            return true
                        } else if (message_text === "") {
                            return true
                        }

                        return false
                    }
                    anchors.left: parent.left
                    anchors.leftMargin: message_text!==""?0:Core.dp(8)
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: {
                        if (message_text !== "")
                            return messageText.paintedHeight + Core.dp(14)
                        return Core.dp(6)
                    }
                    position: message_additional_data.isPlaying?audioPlayer.position:0
                    duration: message_additional_data.isPlaying?audioPlayer.duration:0
                    isPlay: message_additional_data.isPlaying && audioPlayer.isPlay
                    isPause: message_additional_data.isPlaying && audioPlayer.isPause

                    onPlayClicked: {
                        if (!message_additional_data.isPlaying) {
                            for (var i = 0; i < messagesModel.count; i++) {
                                messagesModel.get(i).additionalData.isPlaying = false
                            }

                            message_additional_data.isPlaying = true
                        } else {
                            audioPlayer.play()
                        }
                    }

                    onPauseClicked: {
                        if (message_additional_data.isPlaying)
                            audioPlayer.pause()
                    }

                    onStopClicked: {
                        if (message_additional_data.isPlaying)
                            audioPlayer.stop()
                    }

                    onPositionSliderClicked: {
                        if (message_additional_data.isPlaying)
                            audioPlayer.seek(newPosition)
                    }
                }

                Text {
                    id: sendTimeText
                    anchors.left: message_direction === Message.Incoming?undefined:parent.left
                    anchors.right: message_direction === Message.Outgoing?undefined:parent.right
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: Core.dp(8)
                    anchors.rightMargin: Core.dp(8)
                    anchors.bottomMargin: Core.dp(2)
                    font.pixelSize: Core.dp(5)
                    color: "white"
                    text: Qt.formatDateTime(message_send_date_time, "dd.MM.yy hh:mm")
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
                source: "../../images/51c.png"
            }

        }
    }


    Rectangle {
        id: audioRecordBar
        property string role: ""
        property url source: audioRecorder.outputLocation
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: firstButtonRow.top
        color: "#868686"

        height: audioButton.checked?Core.dp(38):0
        Behavior on height {NumberAnimation { duration: 200;} }

        AudioRecorder {
            id: audioRecorder
        }

        ChatButton {
            id: recordButton
            anchors.left: parent.left
            width: Core.dp(38)
            height: width
            imageNumber: audioRecordBar.role !== "recording"?74:80
            onClicked: {
                if (audioRecordBar.role !== "recording") {
                    audioRecordBar.role = "recording"
                    console.log("Start record... ")
                    audioRecorder.record()
                } else {
                    audioRecorder.stop()
                    audioRecordBar.role = "finish"
                    console.log("End record in " + audioRecordBar.source)
                }
            }
        }

        MediaButtons {
            id: recordMediaButtons
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: Core.dp(38)
            position: audioPlayer.position
            duration: audioPlayer.duration
            isPlay: audioPlayer.isPlay
            isPause: audioPlayer.isPause

            onPlayClicked: {
                audioPlayer.source = audioRecordBar.source
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

        TerminalMouseArea {
            visible: audioRecordBar.role !== "finish"
            anchors.fill: recordMediaButtons
            onClicked: {}
        }
    }

    Item {
        id: attachmentBar
        property string role
        property string attachmentFileName
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: textInput.top
        clip: true
        visible: height > 0

        height: {
            if (role === "file") {
                return Core.dp(22)
            }
            return 0
        }

        Behavior on height {NumberAnimation {duration: 200;} }

        Rectangle {
            id: fileAttachment
            anchors.left: parent.left
            anchors.right: parent.right
            height: Core.dp(22)
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
        id: smilesRow
        property int smilesCount: 15
        property int columnCount: (width - 2*smilesGrid.anchors.margins) / Core.dp(20) < 1?1:(width - 2*smilesGrid.anchors.margins) / Core.dp(20)
        property int rowCountInt: smilesCount / columnCount
        property int rowCount:  {
            if (smilesCount / columnCount - rowCountInt !== 0) {
                return rowCountInt + 1
            }
            return rowCountInt
        }
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: firstButtonRow.top
        height: smileButton.checked?smilesRow.collapcedHeight():0
        visible: height > 0
        color: "#848484"

        function collapcedHeight() {
            return 2*smilesGrid.anchors.margins + rowCount*smilesGrid.cellHeight
        }

        Behavior on height {NumberAnimation {duration: 200;} }

        function insertSmile(smileImage) {
            textInputText.insert(textInputText.cursorPosition, "<img src=\""+ Core.dataDir +"smiles/"+smileImage+"\" "
                                 +"height=\""+Core.dp(8) + "\""
                                 +"width=\""+Core.dp(8)+"\"/>")
            smileButton.checked = false
        }

        GridView {
            id: smilesGrid
            anchors.fill: parent
            anchors.margins: Core.dp(4)
            cellHeight: cellWidth
            cellWidth: width/smilesRow.columnCount
            model: smilesRow.smilesCount
            delegate: Item {
                width: smilesGrid.cellWidth
                height: smilesGrid.cellHeight
                ChatButton {
                    width: parent.width - Core.dp(4)
                    height: parent.height - Core.dp(4)
                    anchors.centerIn: parent
                    hoverColor: backgroundColor
                    clip: false
                    imageFolder: "../../smiles/"
                    imageNumber: 1
                    platformIndependentHoverEnabled: smilesRow.height === smilesRow.collapcedHeight()
                    onClicked: smilesRow.insertSmile("1.png")
                }
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
        clip: true
        ChatButton {
            id: addButton
            width: (parent.width - 3)/6
            height: parent.height
            anchors.left: parent.left
            imageNumber: 200
            onClicked: addButton.checked = !addButton.checked
        }
        Item {
            id: subRows
            width: (parent.width - 3)*4/6
            anchors.left: addButton.right
            anchors.leftMargin: 1
            height: firstButtonRow.height * 2
            anchors.top: parent.top
            anchors.topMargin: !addButton.checked?0:-firstButtonRow.height
            Behavior on anchors.topMargin { NumberAnimation {duration: 200} }

            Row {
                id: firstButtonSubRow
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                width: (parent.width - 3)/2
                height: firstButtonRow.height
                spacing: 1
                visible: subRows.anchors.topMargin !== -firstButtonRow.height
                ChatButton {
                    id: audioConferenceButton
                    width: (parent.width - 3)/4
                    height: parent.height
                    imageNumber: 54
                }
                ChatButton {
                    id: videoConferenceButton
                    width: (parent.width - 3)/4
                    height: parent.height
                    imageNumber: 55
                }
                ChatButton {
                    id: smileButton
                    width: (parent.width - 3)/4
                    height: parent.height
                    imageNumber: 78
                    onClicked: smileButton.checked = !smileButton.checked
                }
                ChatButton {
                    id: searchButton
                    width: (parent.width - 3)/4
                    height: parent.height
                    imageNumber: 202
                    onClicked: messageFilterModel.filterString = TextDecorator.toPlainText(textInputText.getFormattedText(0, textInputText.length))
                }
            }
            Row {
                id: secondButtonSubRow
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.left: parent.left
                width: (parent.width - 3)*4/6
                height: firstButtonRow.height
                spacing: 1
                visible: subRows.anchors.topMargin !== 0
                ChatButton {
                    id: photoButton
                    width: (parent.width - 3)/4
                    height: parent.height
                    imageNumber: 72
                }
                ChatButton {
                    id: videoButton
                    width: (parent.width - 3)/4
                    height: parent.height
                    imageNumber: 73
                }
                ChatButton {
                    id: audioButton
                    width: (parent.width - 3)/4
                    height: parent.height
                    imageNumber: 74
                    onClicked: {
                        audioButton.checked = !audioButton.checked
                        audioRecordBar.role = "record"
                    }
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
        ChatButton {
            id: sendButton
            height: parent.height
            anchors.left: subRows.right
            anchors.leftMargin: 1
            anchors.right: parent.right
            imageNumber: 56
            onClicked: chatPage.send()
        }
    }

    Rectangle {
        id: textInput
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: Core.dp(22)
        color: "white"

        Text {
            anchors.centerIn: parent
            font.pixelSize: Core.dp(8)
            color: "#c6c1c7"
            text: qsTr("input text field")
            opacity: textInputText.text === "" && !textInputText.focus?1:0.5
        }

        Flickable {
            id: textInputFlick
            clip: true
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: Core.dp(8)
            anchors.rightMargin: Core.dp(8)
            interactive: textInputText.lineCount > 0
            contentHeight: textInputText.paintedHeight + Core.dp(14)
            function ensureVisible(r) {
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height + Core.dp(7))
                    contentY = r.y+r.height-height + Core.dp(13);
            }

            TextEdit {
                id: textInputText
                anchors.left: parent.left
                anchors.right: parent.right
                textFormat: TextEdit.RichText
                y: Core.dp(7)
                font.pixelSize: Core.dp(8)
                wrapMode: TextEdit.Wrap
                selectByMouse: true
                selectionColor: "#c00d0d"
                onCursorRectangleChanged: textInputFlick.ensureVisible(cursorRectangle)

                Keys.onPressed: {
                    if (event.modifiers === Qt.ControlModifier && event.key === Qt.Key_Return) {
                        chatPage.send()
                    }
                }
            }
        }

        TerminalMouseArea {
            anchors.fill: parent
            visible: !textInputText.focus
            onClicked: textInputText.forceActiveFocus()
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 1
            color: "#c6c1c7"
        }
    }

    PhotoView {
        id: photoView
        anchors.centerIn: parent
    }

    VideoPlayer {
        id: videoPlayer
        property url source
        anchors.centerIn: parent

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
        id: fileSaveDialog
        property url fileToSave
        selectFolder: false
        selectFile: false
        selfSelect: true

        onAccepted: {
            FileSystem.cp(fileToSave, fileUrl)
        }
    }

    FileDialog {
        id: fileChoiceDialog
        selectMultiple: false
        title: qsTr("Выбирите файл")

        onAccepted: {
            attachmentBar.role = "file"
            attachmentBar.attachmentFileName = fileUrl
        }

        onRejected: {
            attachmentBar.role = ""
            attachmentBar.attachmentFileName = ""
        }
    }

    PhotoCamera {
        id: photoCamera
        anchors.fill: parent
    }
}
