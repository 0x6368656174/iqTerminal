import QtQuick 2.0
import QtQuick.Dialogs 1.1
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."

Page {
    id: infoPage
    clip: true

    property string iXmlPath

    signal dataChanged()

    QtObject {
        id: privateData
        property bool isEdited: false
    }

    name: "i_info"

    XmlListModel {
        id: infoModel
        source: iXmlPath
        query: "/i/info"

        XmlRole {
            name: "user_photo"
            query: "photo/string()"
        }

        XmlRole {
            name: "user_name"
            query: "name/string()"
        }

        onCountChanged: {
            if (count === 1) {
                photoImage.source = infoModel.get(0).user_photo !== ""?"image://xml/" + Core.dataDirPath + "/i.xml":""
            }
        }
    }

    XmlListModel {
        id: statusesModel
        source: iXmlPath
        query: "/i/info/statuses/state"

        XmlRole {
            name: "state_name"
            query: "name/string()"
        }

        XmlRole {
            name: "state_text"
            query: "text/string()"
        }
        XmlRole {
            name: "state_id"
            query: "@id/string()"
            isKey: true
        }
    }

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
            property bool isEdited: false
            property bool isRemoved: false
            property url photoToSaveUrl
            anchors.left: parent.left
            anchors.right: parent.right
            height: Core.dp(132)
            color: "#a0706a78"
            Rectangle {
                visible: photoRect.isEdited
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
                    source: "image://xml/" + Core.dataDirPath + "/i.xml"
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
                visible: !privateData.isEdited
                onVisibleChanged: {
                    photoRect.color = "#a0706a78"
                }

                onPressedChanged: {
                    if (pressed) {
                        photoPressAndHoldAnimation.restart()
                    } else {
                        photoPressAndHoldAnimation.stop()
                        photoRect.color = "#a0706a78"
                    }
                }

                onPressAndHold: {
                    photoRect.isEdited = true
                }
            }
        }

        /////////ИМЯ/////////

        Repeater {
            id: nameContainter
            property bool isEdited: false
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: photoRect.bottom
            height: Core.dp(20)

            model: infoModel
            delegate: Rectangle {
                id: nameItem

                property alias nameToSave: nameTextEditor.text

                anchors.left: {
                    if (parent !== null) {
                        return parent.left
                    }
                    return undefined
                }
                anchors.right: {
                    if (parent !== null) {
                        return parent.right
                    }
                    return undefined
                }
                anchors.top: {
                    if (parent !== null) {
                        return photoRect.bottom
                    }
                    return undefined
                }

                height: Core.dp(20)
                color: "#c6c1c7"

                Rectangle {
                    visible: nameContainter.isEdited
                    anchors.fill: parent
                    color: "#ff5d1b"
                }

                Image {
                    id: namePhotoImage
                    cache: false
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.margins: Core.dp(2)
                    fillMode: Image.PreserveAspectFit
                    source: "image://xml/" + Core.dataDirPath + "/i.xml"
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
                    visible: !nameContainter.isEdited || nameEditBar.editRole !== "edit"
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.leftMargin: Core.dp(12) + (namePhotoImage.width-Core.dp(4))*namePhotoImage.scale
                    anchors.rightMargin: anchors.leftMargin
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: Core.dp(8)
                    text: user_name
                    font.bold: true
                }

                TextInput {
                    id: nameTextEditor
                    clip: true
                    visible: nameContainter.isEdited && nameEditBar.editRole === "edit"
                    anchors.fill: nameText
                    selectByMouse: true
                    selectionColor: "#c00d0d"
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: Core.dp(8)
                    text: nameText.text
                    onVisibleChanged: {
                        if (visible) {
                            forceActiveFocus()
                        }
                    }
                    onAccepted: nameEditBar.onSubmit()
                }

                ColorAnimation {
                    id: namePressAndHoldAnimation
                    target: nameItem
                    duration: 800
                    property: "color"
                    from: "#c6c1c7"
                    to: "#ff5d1b"
                }

                TerminalMouseArea {
                    id: nameMa
                    anchors.fill: parent
                    visible: !privateData.isEdited
                    onVisibleChanged: {
                        nameItem.color = "#c6c1c7"
                    }

                    onPressedChanged: {
                        if (pressed) {
                            namePressAndHoldAnimation.restart()
                        } else {
                            namePressAndHoldAnimation.stop()
                            nameItem.color = "#c6c1c7"
                        }
                    }

                    onPressAndHold: {
                        nameContainter.isEdited = true
                    }

                    onClicked: {
                        showPhotoAmination.restart()
                    }
                }
            }
        }
        /////СТАТУСЫ////////

        Rectangle {
            id: stateTextContainer
            anchors.top: nameContainter.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: Core.dp(20)

            Text {
                anchors.fill: parent
                anchors.leftMargin: Core.dp(12)
                anchors.rightMargin: anchors.leftMargin
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: Core.dp(8)
                text: qsTr("Статус")
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
            //            anchors.bottom: parent.bottom
            height: infoPage.height - nameContainter.height - stateTextContainer.height - stateSpacer.height - photoRect.height + flickItem.contentY
            model: statusesModel

            add: Transition {
                NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
                NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
            }

            displaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
            }


            delegate: Column {
                id: stateItem

                property int itemIndex: index
                property bool isRemoved: statesView.currentStateId === stateId && statusesEditBar.editRole === "remove"
                property bool isAddAfter: statesView.currentStateId === stateId && statusesEditBar.editRole === "add"
                property int stateId: state_id

                property alias dataItemColor: dataItem.color
                property alias stateNameToSave: stateTextEditor.text
                property alias stateDataToSave: detailTextEditor.text

                function reset() {
                    stateTextEditor.text = state_name
                    detailTextEditor.text = state_text
                }

                anchors.left: parent.left
                anchors.right: parent.right
                height: dataItem.height + 1

                Rectangle {
                    id: dataItem
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: detailText.paintedHeight+Core.dp(12)

                    Rectangle {
                        id: checkRect
                        visible: statesView.currentStateId === stateItem.stateId
                        anchors.fill: parent
                        color: "#ff5d1b"
                    }

                    Text {
                        id: stateText
                        visible: statesView.currentStateId !== stateItem.stateId || statusesEditBar.editRole !== "edit"
                        anchors.left: parent.left
                        anchors.leftMargin: Core.dp(12)
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        width: Core.dp(50)
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: Core.dp(8)
                        text: stateTextEditor.text
                    }
                    TextInput {
                        id: stateTextEditor
                        visible: statesView.currentStateId === stateItem.stateId && statusesEditBar.editRole === "edit"
                        anchors.fill: stateText
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: Core.dp(8)
                        text: state_name
                        clip: true
                        onAccepted: statusesEditBar.onSubmit()
                        KeyNavigation.tab: detailTextEditor
                    }

                    Text {
                        id: detailText
                        visible: statesView.currentStateId !== stateItem.stateId || statusesEditBar.editRole !== "edit"
                        wrapMode: Text.WordWrap
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.leftMargin: Core.dp(64)
                        anchors.right: parent.right
                        anchors.rightMargin: Core.dp(12)
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: Core.dp(8)
                        text: detailTextEditor.text
                    }

                    TextInput {
                        id: detailTextEditor
                        visible: statesView.currentStateId === stateItem.stateId && statusesEditBar.editRole === "edit"
                        onVisibleChanged: {
                            if (visible) {
                                forceActiveFocus()
                            }
                        }
                        wrapMode: Text.WordWrap
                        anchors.fill: detailText
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: Core.dp(8)
                        text: state_text
                        clip: true
                        onAccepted: statusesEditBar.onSubmit()
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
                        visible: !privateData.isEdited
                        onVisibleChanged: {
                            dataItem.color = "white"
                        }

                        onPressedChanged: {
                            if (pressed) {
                                stateItemPressAndHoldAnimation.restart()
                            } else {
                                stateItemPressAndHoldAnimation.stop()
                                dataItem.color = "white"
                            }
                        }

                        onPressAndHold: {
                            dataItem.color = "#ff5d1b"
                            statesView.currentStateId = stateItem.stateId
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
        id: photoEditBar
        z: 1
        removeButtonEnabled: photoImage.sourceSize.width > 0
        addButtonEnabled: photoImage.sourceSize.width === 0
        editButtonEnabled: photoImage.sourceSize.width > 0

        visible: opacity !== 0
        opacity: photoRect.isEdited && !photoFileDialog.visible?1:0
        onOpacityChanged: {
            if (opacity === 0) {
                privateData.isEdited = false
                editRole = ""
            } else {
                privateData.isEdited = true
            }
        }
        Behavior on opacity {NumberAnimation { duration: 200} }
        Behavior on anchors.bottomMargin {NumberAnimation {duration: 200} }
        anchors.bottomMargin: photoRect.isEdited && !photoFileDialog.visible?0:-Core.dp(22)

        onButtonClicked: {
            if (buttonType === "edit") {
                editRole = "edit"
                photoFileDialog.open()
            } else if (buttonType === "add") {
                editRole = "add"
                photoFileDialog.open()
            } else if (buttonType === "remove") {
                editRole = "remove"
                photoRect.isRemoved = true
            } else if (buttonType === "submit") {
            } else if (buttonType === "cancel") {
                photoRect.photoToSaveUrl = ""
                photoRect.isRemoved = false
                photoRect.isEdited = false
            }
        }

        onSubmit: {
            console.log("IInfo.photoEditBar::onSubmit()")
            infoPage.dataChanged()
            photoRect.isRemoved = false
            photoRect.isEdited = false
        }

        onHidenClicked: {
            photoRect.isRemoved = false
            photoRect.isEdited = false
        }

        FileDialog {
            id: photoFileDialog
            title: qsTr("Выберите фото")
            onAccepted: {
                if (FileValidator.isImage(fileUrl)) {
                    photoRect.photoToSaveUrl = fileUrl
                    photoEditBar.submit()
                } else {
                    console.log(qsTr("File" + fileUrl.toString() +  " is not image."))
                }
            }
            onRejected: photoRect.isEdited = false

            onVisibleChanged: {
                if (!visible)
                    photoRect.isEdited = false
            }
        }
    }

    EditBar {
        id: statusesEditBar
        visible: opacity !== 0
        opacity: statesView.currentStateId !== -1?1:0
        onOpacityChanged: {
            if (opacity === 0) {
                privateData.isEdited = false
                editRole = ""
            } else {
                privateData.isEdited = true
            }
        }
        Behavior on opacity {NumberAnimation { duration: 200} }
        Behavior on anchors.bottomMargin {NumberAnimation {duration: 200} }
        anchors.bottomMargin: statesView.currentStateId !== -1?0:-Core.dp(22)

        onButtonClicked: {
            if (buttonType === "edit") {
                editRole = "edit"
            } else if (buttonType === "remove") {
                editRole = "remove"
            } else if (buttonType === "add") {
                editRole = "add"
            } else if (buttonType === "submit") {
                onSubmit()
            } else if (buttonType === "cancel") {
                stateDelegate(statesView.currentStateId).reset()
                statesView.currentStateId = -1
            }
        }

        function onSubmit() {
            infoPage.dataChanged()
            statesView.currentStateId = -1
        }

        onHidenClicked: {
            statesView.currentStateId = -1
        }
    }

    EditBar {
        id: nameEditBar
        z:1
        addButtonEnabled: false
        removeButtonEnabled: false
        visible: opacity !== 0
        opacity: nameContainter.isEdited?1:0
        onOpacityChanged: {
            if (opacity === 0) {
                privateData.isEdited = false
                editRole = ""
            } else {
                privateData.isEdited = true
            }
        }
        Behavior on opacity {NumberAnimation { duration: 200} }
        Behavior on anchors.bottomMargin {NumberAnimation {duration: 200} }
        anchors.bottomMargin: nameContainter.isEdited?0:-Core.dp(22)

        onButtonClicked: {
            if (buttonType === "edit") {
                editRole = "edit"
            } else if (buttonType === "submit") {
                onSubmit()
            } else if (buttonType === "cancel") {
                nameContainter.itemAt(0).nameToSave = infoModel.get(0).user_name
                nameContainter.isEdited = false
            }
        }

        function onSubmit() {
            infoPage.dataChanged()
            nameContainter.isEdited = false
        }

        onHidenClicked: {
            nameContainter.isEdited = false
        }
    }

    function stateDelegate (stateId) {
        for (var i = 0; i < statesView.contentItem.children.length; i++) {
            if (parseInt(statesView.contentItem.children[i].stateId) === parseInt(stateId)) {
                return statesView.contentItem.children[i]
            }
        }
    }

    function stateMaxId() {
        var result = 0
        for (var stateIndex = 0; stateIndex < statusesModel.count; stateIndex++){
            if (statusesModel.get(stateIndex).state_id > result)
                result = parseInt(statusesModel.get(stateIndex).state_id)
        }
        return result
    }

    function dataToSave() {
        console.log("IInfo::dataToSave()")
        var result = "  <info>\n"
        result = result + "    <name>" + nameContainter.itemAt(0).nameToSave + "</name>\n"
        if (photoRect.isRemoved) {
            result = result + "    <photo></photo>\n"
        } else if (photoRect.photoToSaveUrl.toString() !== "") {
            result = result + "    <photo>" + Base64.fileToBase64(photoRect.photoToSaveUrl) + "</photo>\n"
        } else {
            result = result + "    <photo>" + infoModel.get(0).user_photo + "</photo>\n"
        }

        result = result + "    <statuses>\n"
        for (var stateIndex = 0; stateIndex < statusesModel.count; stateIndex++) {
            //Найдем объект соответсвтующий индексу модели
            var stateItem = stateDelegate(statusesModel.get(stateIndex).state_id)

            if (stateItem.isRemoved)
                continue

            result = result + "        <state id=\"" + stateItem.stateId + "\">\n"
            result = result + "          <name>" + stateItem.stateNameToSave + "</name>\n"
            result = result + "          <text>" + stateItem.stateDataToSave + "</text>\n"
            result = result + "        </state>\n"

            if (stateItem.isAddAfter) {
                result = result + "        <state id=\"" + (stateMaxId() + 1) + "\">\n"
                result = result + "          <name>Новый Статус</name>\n"
                result = result + "          <text></text>\n"
                result = result + "        </state>\n"
            }
        }

        result = result + "    </statuses>\n"
        result = result + "  </info>\n"

        return result
    }

    function reload() {
        var oldSource = photoImage.source
        photoImage.source = ""
        photoImage.source = oldSource
        infoModel.reload()
        statusesModel.reload()
        photoRect.photoToSaveUrl = ""
    }
}
