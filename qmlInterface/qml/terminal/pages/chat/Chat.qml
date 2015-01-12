import QtQuick 2.2
import TerminalQmlPlugin 1.0
import QtMultimedia 5.2
import ".."
//import "pages"
import "../../elements"
import "../videocall"

Page {
    id: chatPage
    property User user

    name: "chat"

    function send() {
        var plainText = TextDecorator.toPlainText(textInputText.getFormattedText(0, textInputText.length))
        if (attachmentBar.role === "" && audioRecordBar.role === "" && plainText === "")
            return

        var newMessage = user.messagesModel.appendNew()
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

        proxy.sendMessage(newMessage)

        attachmentBar.role = ""
        audioRecordBar.role = ""
        audioRecordBar.source = ""
        audioButton.checked = false
        attachmentBar.attachmentFileName = ""
        textInputText.text = ""
        addButton.checked = false

        user.messagesModel.save()
    }

    BackButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: (70 * applicationModel.settings.zoomFactor)
        visible: !fileChoiceDialog.visible
    }

    Component {
        id: messagesModelAdditionalData
        QtObject {
            property bool collapsed: false
            property bool isPlaying: false
        }
    }

    onUserChanged: {
        user.messagesModel.itemAdditionalData = messagesModelAdditionalData
    }

    Connections {
        target: user?user.messagesModel:null
        onCountChanged: {
            chatView.animatedPosition(user.messagesModel.count - 1)
        }

    }

    MessagesFilterModel {
        id: messageFilterModel
        messagesModel: user?user.messagesModel:null
    }


    Rectangle {
        id: titleRect
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        height: (20 * applicationModel.settings.zoomFactor)
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
                    source: user?"image://xml/" + user.userInfo.userProfile.source:""
                    fillMode: Image.PreserveAspectFit
                }
            }
        }

        Text {
            anchors.left: photoItem.right
            anchors.bottom: photoItem.bottom
            anchors.top: photoItem.top
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: (8 * applicationModel.settings.zoomFactor)
            anchors.leftMargin: font.pixelSize
            anchors.rightMargin: font.pixelSize
            anchors.right: parent.right
            elide: Text.ElideRight
            text: user?user.userInfo.userProfile.name:""
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                userInfoPage.userInfo = user.userInfo
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
        spacing: (2 * applicationModel.settings.zoomFactor)
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
                        return (50 * applicationModel.settings.zoomFactor)
                    return (36 * applicationModel.settings.zoomFactor)
                } else if (message_additional_data.isPlaying) {
                    return (74 * applicationModel.settings.zoomFactor)
                } else if (message_type == Message.Text && messageText.lineCount > 1) {
                    return messageText.paintedHeight + (28 * applicationModel.settings.zoomFactor)
                }
                return (38 * applicationModel.settings.zoomFactor)
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
                height: message_direction === Message.Incoming?(4 * applicationModel.settings.zoomFactor):0
                anchors.leftMargin: (4 * applicationModel.settings.zoomFactor)
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
                    width: message_type !== Message.Text?(28 * applicationModel.settings.zoomFactor):0
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
                Rectangle {
                    id: statusFile
                    width: 3
                    height: parent.height
                    anchors.bottom: parent.bottom
                    anchors.left: typeImage.right
                    color: message_was_read?"#f00000":"#d4a4a4"
                    visible: {
                        if((message_type === Message.Text) || (message_direction === Message.Outgoing))
                            return false
                        else
                            return true
                    }

                }

                Text {
                    id: messageText
                    //                    anchors.left: typeImage.right
                    anchors.left: statusFile.right
                    anchors.leftMargin: message_type !== Message.Text?2:(8 * applicationModel.settings.zoomFactor)
                    anchors.right: parent.right
                    anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                    anchors.topMargin: (12 * applicationModel.settings.zoomFactor)
                    anchors.top: parent.top
                    wrapMode: Text.WordWrap
                    text: TextDecorator.toFormattedText(message_text)
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
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
                        if (message_direction === Message.Outgoing) return
                        if (message_direction === Message.Incoming)
                            if(message_was_read === false) {
                                proxy.command("filechat",message_file_path)
                                return
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
                            for (var i = 0; i < user.messagesModel.count; i++) {
                                user.messagesModel.get(i).additionalData.isPlaying = false
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
                    anchors.leftMargin: message_text!==""?0:(8 * applicationModel.settings.zoomFactor)
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: {
                        if (message_text !== "")
                            return messageText.paintedHeight + (14 * applicationModel.settings.zoomFactor)
                        return (6 * applicationModel.settings.zoomFactor)
                    }
                    position: message_additional_data.isPlaying?audioPlayer.position:0
                    duration: message_additional_data.isPlaying?audioPlayer.duration:0
                    isPlay: message_additional_data.isPlaying && audioPlayer.isPlay
                    isPause: message_additional_data.isPlaying && audioPlayer.isPause

                    onPlayClicked: {
                        if (!message_additional_data.isPlaying) {
                            for (var i = 0; i < user.messagesModel.count; i++) {
                                user.messagesModel.get(i).additionalData.isPlaying = false
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
                    //                    anchors.left: message_direction === Message.Incoming?undefined:parent.left
                    //                    anchors.right: message_direction === Message.Outgoing?undefined:parent.right
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
                    anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                    anchors.bottomMargin: (2 * applicationModel.settings.zoomFactor)
                    font.pixelSize: (5 * applicationModel.settings.zoomFactor)
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
                height: message_direction === Message.Outgoing?(4 * applicationModel.settings.zoomFactor):0
                anchors.rightMargin: (4 * applicationModel.settings.zoomFactor)
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

        height: audioButton.checked?(38 * applicationModel.settings.zoomFactor):0
        Behavior on height {NumberAnimation { duration: 200;} }

        AudioRecorder {
            id: audioRecorder
        }

        ChatButton {
            id: recordButton
            anchors.left: parent.left
            width: (38 * applicationModel.settings.zoomFactor)
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
            anchors.leftMargin: (38 * applicationModel.settings.zoomFactor)
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
                return (22 * applicationModel.settings.zoomFactor)
            }
            return 0
        }

        Behavior on height {NumberAnimation {duration: 200;} }

        Rectangle {
            id: fileAttachment
            anchors.left: parent.left
            anchors.right: parent.right
            height: (22 * applicationModel.settings.zoomFactor)
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
                font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                anchors.right: parent.right
                anchors.rightMargin: font.pixelSize
                elide: Text.ElideRight
                text: attachmentBar.attachmentFileName
            }
        }
    }

    Rectangle {
        id: smilesRow
        property int smilesCount: 10
        property int columnCount: (width - 2*smilesGrid.anchors.margins) / (20 * applicationModel.settings.zoomFactor) < 1?1:(width - 2*smilesGrid.anchors.margins) / (20 * applicationModel.settings.zoomFactor)
        property int rowCountInt: smilesCount / columnCount
        property int rowCount:  smilesCount / columnCount - rowCountInt !== 0?rowCountInt + 1:rowCountInt
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
                                 +"height=\""+(8 * applicationModel.settings.zoomFactor) + "\""
                                 +"width=\""+(8 * applicationModel.settings.zoomFactor)+"\"/>")
            smileButton.checked = false
        }

        GridView {
            id: smilesGrid
            anchors.fill: parent
            anchors.margins: (4 * applicationModel.settings.zoomFactor)
            cellHeight: cellWidth
            cellWidth: width/smilesRow.columnCount
            model: smilesRow.smilesCount
            delegate: Item {
                width: smilesGrid.cellWidth
                height: smilesGrid.cellHeight
                ChatButton {
                    width: parent.width - (4 * applicationModel.settings.zoomFactor)
                    height: parent.height - (4 * applicationModel.settings.zoomFactor)
                    anchors.centerIn: parent
                    hoverColor: backgroundColor
                    clip: false
                    imageFolder: "../../smiles/"
                    imageNumber: index
                    platformIndependentHoverEnabled: smilesRow.height === smilesRow.collapcedHeight()
                    onClicked: smilesRow.insertSmile(index+".png")
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
            width: (parent.width - 4)/6
            height: parent.height
            anchors.left: parent.left
            imageNumber: 200
            onClicked: addButton.checked = !addButton.checked
        }
        Item {
            id: subRows
            width: (parent.width - 4)*4/6
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
                    id: videoConferenceButton
                    width: (parent.width - 2)/3
                    height: parent.height
                    imageNumber: 54
                    onClicked: {
                        videoCall.userId = user.id
                        videoCall.start()
                    }
                }
                ChatButton {
                    id: smileButton
                    width: (parent.width - 2)/3
                    height: parent.height
                    imageNumber: 78
                    onClicked: smileButton.checked = !smileButton.checked
                }
                ChatButton {
                    id: searchButton
                    width: (parent.width - 2)/3
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
            width: (parent.width - 4)/7
            anchors.left: subRows.right
            anchors.leftMargin: 1
            imageNumber: 56
            onClicked: chatPage.send()
        }
        Rectangle {
            id: onlineStatus
            height: parent.height
            anchors.left: sendButton.right
            anchors.leftMargin: 1
            anchors.right: parent.right

            Rectangle {
                id: chatStatus
                height: (parent.height-1)/3
                width: parent.width
                anchors.top: parent.top
                color: user.userInfo.onlinec?"#00FF00":"#FF0000"
            }
            Rectangle {
                id: audioStatus
                height: (parent.height-1)/3
                width: parent.width
                anchors.top: chatStatus.bottom
                anchors.topMargin: 1
                color: user.userInfo.onlinea?"#00FF00":"#FF0000"
            }
            Rectangle {
                id: videoStatus
                height: (parent.height-1)/3
                width: parent.width
                anchors.top: audioStatus.bottom
                anchors.topMargin: 1
                color: user.userInfo.onlinev?"#00FF00":"#FF0000"
            }
        }
    }

    Rectangle {
        id: textInput
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: (22 * applicationModel.settings.zoomFactor)
        color: "white"

        Text {
            anchors.centerIn: parent
            font.pixelSize: (8 * applicationModel.settings.zoomFactor)
            color: "#c6c1c7"
            text: qsTr("input text field") + applicationModel.settings.translatorStringEnd
            opacity: textInputText.text === "" && !textInputText.focus?1:0.5
        }

        Flickable {
            id: textInputFlick
            clip: true
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
            anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
            interactive: textInputText.lineCount > 0
            contentHeight: textInputText.paintedHeight + (14 * applicationModel.settings.zoomFactor)
            function ensureVisible(r) {
                if (contentY >= r.y)
                    contentY = r.y;
                else if (contentY+height <= r.y+r.height + (7 * applicationModel.settings.zoomFactor))
                    contentY = r.y+r.height-height + (13 * applicationModel.settings.zoomFactor);
            }

            TextEdit {
                id: textInputText
                anchors.left: parent.left
                anchors.right: parent.right
                textFormat: TextEdit.RichText
                y: (7 * applicationModel.settings.zoomFactor)
                font.pixelSize: (8 * applicationModel.settings.zoomFactor)
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
        title: qsTr("Select file") + applicationModel.settings.translatorStringEnd

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
