import QtQuick 2.0
import QtQuick.Dialogs 1.1
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../../elements"

Page {
    id: page

    property string iXmlPath

    name: "i_photo"

    signal dataChanged()

    QtObject {
        id: privateData
        property bool addFirstFolder: false
    }

    Rectangle {
        id: createNewButton
        anchors.left: parent.left
        anchors.right: parent.right
        height: Core.dp(22)
        color: createNewButtonMA.containsMouse?"#da4504":"#c6c1c7"

        Item {
            anchors.leftMargin: Core.dp(12)
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: createNewButtonText.width
            Text {
                id: createNewButtonText
                anchors.centerIn: parent
                font.pixelSize: Core.dp(8)
                text: qsTr("Создать новую")
                color: createNewButtonMA.containsMouse?"white":"black"
            }
        }

        SequentialAnimation {
            id: createButtonClickAnimation
            PropertyAnimation {
                target: createNewButtonText
                property: "scale"
                from: 1.0
                to: 1.2
                duration: 200
            }
            ScriptAction {
                script: {
                    createNewButtonText.scale = 1.0
                }
            }
        }

        Timer {
            id: enableButtonTimer
            interval: 1000
        }

        TerminalMouseArea {
            id: createNewButtonMA
            anchors.fill: parent
            platformIndependentHoverEnabled: true
            enabled: !foldersView.isEdited && !enableButtonTimer.running

            onClicked: {
                enableButtonTimer.start()
                createButtonClickAnimation.restart()
                privateData.addFirstFolder = true
                page.dataChanged()
            }
        }
    }






    Rectangle {
        id: spacer
        anchors.top: createNewButton.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        height: Core.dp(2)
    }



    XmlListModel {
        id: foldersModel
        source: iXmlPath
        query: "/i/photos/folder"

        XmlRole {
            name: "folder_name"
            query: "name/string()"
        }
        XmlRole {
            name: "folder_id"
            query: "@id/string()"
            isKey: true
        }
    }

    ListView {
        id: foldersView
        visible: photoSlider.opacity !== 1

        cacheBuffer: 10000

        property bool isEdited: currentFolderId !== -1
        property bool isChildrenEdited: false
        property int currentFolderId: -1

        function reload() {
            foldersModel.reload()
            for (var i = 0; i < foldersModel.count; i++) {
                getFolderDelegate(foldersModel.get(i).folder_id).reload()
            }
        }

        function reset() {
            for (var i = 0; i < foldersModel.count; i++) {
                getFolderDelegate(foldersModel.get(i).folder_id).reset()
            }
            currentFolderId = -1
            isChildrenEdited = false
        }

        function submit() {
            for (var i = 0; i < foldersModel.count; i++) {
                getFolderDelegate(foldersModel.get(i).folder_id).submit()
            }
            currentFolderId = -1
            isChildrenEdited = false
        }

        function getFolderDelegate (folderId) {
            for (var i = 0; i < foldersView.contentItem.children.length; i++) {
                if (parseInt(foldersView.contentItem.children[i].folderId) === parseInt(folderId)) {
                    return foldersView.contentItem.children[i]
                }
            }
            return undefined
        }

        function getFolderMaxId() {
            var result = -1
            for (var i = 0; i < foldersModel.count; i++){
                if (foldersModel.get(i).folder_id > result) {
                    result = parseInt(foldersModel.get(i).folder_id)
                }
            }
            return result
        }

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: spacer.bottom
        anchors.bottom: parent.bottom
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

        model: foldersModel

        delegate: Rectangle {
            id: folderDelegate

            property bool collapsed: false
            property bool isRemoved: foldersView.currentFolderId === folderId && !foldersView.isChildrenEdited && editBar.editRole === "remove"
            property bool isAddAfter: foldersView.currentFolderId === folderId && !foldersView.isChildrenEdited && editBar.editRole === "add"
            property int folderId: folder_id
            property alias nameToSave: folderTextEditor.text

            function reset() {
                nameToSave = folder_name
                childsView.reset()
            }

            function submit() {
                childsView.submit()
            }

            function reload() {
                childsModel.reload()
            }

            function getChildDelegate (childId) {
                for (var i = 0; i < childsView.count; i++) {
                    if (parseInt(childsView.itemAt(i).childId) === parseInt(childId)) {
                        return childsView.itemAt(i)
                    }
                }
                return undefined
            }

            function getChildMaxId() {
                var result = -1
                for (var i = 0; i < childsModel.count; i++){
                    if (childsModel.get(i).child_id > result) {
                        result = parseInt(childsModel.get(i).child_id)
                    }
                }
                return result
            }


            function dataToSave() {
                var result = ""
                for (var i = 0; i < childsModel.count; i++) {
                    var photoItem = getChildDelegate(childsModel.get(i).child_id)

                    if (photoItem.isRemoved)
                        continue

                    result = result + "    <photo id=\"" + photoItem.childId + "\">\n"
                    result = result + "      <name>" + photoItem.nameToSave + "</name>\n"
                    result = result + "      <file>" + photoItem.fileToSave + "</file>\n"
                    result = result + "    </photo>\n"

                    if (photoItem.addAfterFileName !== "") {
                        result = result + "    <photo id=\"" + (getChildMaxId() + 1) + "\">\n"
                        result = result + "      <name>Фото</name>\n"
                        result = result + "      <file>" + photoItem.addAfterFileName + "</file>\n"
                        result = result + "    </photo>\n"
                    }
                }

                return result
            }

            anchors.left: parent.left
            anchors.right: parent.right
            height: Core.dp(22) + childsFlow.height/* + Core.dp(4)*/

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
                    visible: foldersView.currentFolderId !== folderDelegate.folderId  || editBar.editRole !== "edit"  || foldersView.isChildrenEdited
                    anchors.left: folderImage.right
                    anchors.leftMargin: Core.dp(8)
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: anchors.leftMargin
                    elide: Text.ElideRight
                    font.pixelSize: Core.dp(8)
                    verticalAlignment: Text.AlignVCenter
                    color: folderDelegate.collapsed?"white":"black"
                    Behavior on color {ColorAnimation { duration: 200 } }
                    text: folderTextEditor.text
                }

                TextInput {
                    id: folderTextEditor
                    clip: true
                    visible: foldersView.currentFolderId === folderDelegate.folderId && editBar.editRole === "edit" && !foldersView.isChildrenEdited
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
                    onAccepted: editBar.submit()
                }

                Rectangle {
                    anchors.fill: parent
                    border.color: folderDelegate.collapsed?"white":"#da4504"
                    border.width: 2
                    anchors.margins: 2
                    opacity: foldersView.currentFolderId === folderDelegate.folderId && !foldersView.isChildrenEdited?1:0
                    color: "transparent"
                }

                Rectangle {
                    id: folderPressAndHoldRect
                    anchors.fill: parent
                    border.color: folderDelegate.collapsed?"white":"#da4504"
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
                    visible: !foldersView.isEdited
                    onClicked: {
                        photoFolderClickAmination.restart()
                        folderDelegate.collapsed = !folderDelegate.collapsed
                    }
                    onPressedChanged: {
                        if (pressed) {
                            folderPressAndHoldAmiation.restart()
                        } else {
                            folderPressAndHoldAmiation.stop()
                            folderPressAndHoldRect.opacity = 0
                        }
                    }

                    onPressAndHold: {
                        foldersView.currentFolderId = folder_id
                    }
                }
            }

            XmlListModel {
                id: childsModel
                source: iXmlPath
                query: "/i/photos/folder[@id='" + folderDelegate.folderId + "']/photo"

                XmlRole {
                    name: "child_name"
                    query: "name/string()"
                }

                XmlRole {
                    name: "child_file"
                    query: "file/string()"
                }

                XmlRole {
                    name: "child_id"
                    query: "@id/string()"
                    isKey: true
                }
            }

            Item {
                id: collapsedContainer
                property int columnCount: width / Core.dp(60) < 1?1:width / Core.dp(60)
                property int rowCountInt: childsModel.count / columnCount
                property int rowCount:  {
                    if (childsModel.count / columnCount - rowCountInt !== 0) {
                        return rowCountInt + 1
                    }
                    return rowCountInt
                }

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: folderNameContainer.bottom
                height: folderDelegate.collapsed?rowCount * Core.dp(74) - Core.dp(2):0
                clip: true

                Behavior on height {NumberAnimation {duration: 200 } }

                Flow {
                    id: childsFlow
                    anchors.fill: parent
                    spacing: Core.dp(2)

                    Repeater {
                        id: childsView
                        property int currentChildId: -1
                        property int folderId: folder_id

                        function reset() {
                            for (var i = 0; i < childsModel.count; i++) {
                                getChildDelegate((childsModel.get(i).child_id)).reset()
                            }
                            currentChildId = -1
                        }

                        function submit() {
                            for (var i = 0; i < childsModel.count; i++) {
                                getChildDelegate((childsModel.get(i).child_id)).submit()
                            }
                            currentChildId = -1
                        }

                        model: childsModel
                        delegate: Rectangle {
                            id: childDelegate
                            property int childId: child_id
                            property alias nameToSave: childNameEditor.text
                            property string fileToSave: child_file
                            property bool isRemoved: foldersView.currentFolderId === childsView.folderId && childsView.currentChildId === childId && editBar.editRole === "remove"
                            property bool isAddAfter: foldersView.currentFolderId === childsView.folderId && childsView.currentChildId === childId && editBar.editRole === "add"
                            property string addAfterFileName: ""

                            function reset() {
                                childNameEditor.text = child_name
                                addAfterFileName = ""
                            }

                            function submit() {
                                addAfterFileName = ""
                            }

                            onIsAddAfterChanged: {
                                if (isAddAfter) {
                                    fileDialog.open()
                                }
                            }


                            FileDialog {
                                id: fileDialog
                                title: qsTr("Выберите фото")
                                onAccepted: {
                                    if (FileValidator.isImage(fileUrl)) {
                                        childDelegate.addAfterFileName = fileUrl
                                        editBar.submit()
                                    } else {
                                        console.log(qsTr("File" + fileUrl.toString() +  " is not image."))
                                        editBar.cansel()
                                    }
                                }
                                onRejected: editBar.cansel()

                                onVisibleChanged: {
                                    if (!visible)
                                        editBar.cansel()
                                }
                            }

                            width: (childsFlow.width - Core.dp(2)*(collapsedContainer.columnCount - 1)) / collapsedContainer.columnCount
                            height: Core.dp(72)
                            color: "#6c676e"

                            Item
                            {
                                anchors.fill: parent
                                anchors.bottomMargin: Core.dp(12)
                                Item{
                                    anchors.centerIn: parent
                                    width: photoImage.sourceSize.width > parent.width? parent.width:photoImage.sourceSize.width
                                    height: photoImage.sourceSize.height > Core.dp(60)?Core.dp(60):photoImage.sourceSize.height
                                    Image {
                                        id: photoImage
                                        anchors.fill: parent
                                        source: child_file
                                        fillMode: Image.PreserveAspectFit
                                    }
                                }
                            }

                            Rectangle {
                                id: childNameContainer
                                width: parent.width
                                height: Core.dp(12)
                                anchors.bottom: parent.bottom
                                color: "#dddddd"

                                Rectangle {
                                    anchors.fill: parent
                                    color: "#da4504"
                                    opacity: childsView.currentChildId === childDelegate.childId?1:0
                                }

                                Text {
                                    id: childNameText
                                    visible: foldersView.currentFolderId !== childsView.folderId || childsView.currentChildId !== childId || editBar.editRole !== "edit"
                                    anchors.fill: parent
                                    elide: Text.ElideRight
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    anchors.leftMargin: Core.dp(8)
                                    anchors.rightMargin: Core.dp(8)
                                    font.pixelSize: Core.dp(8)
                                    text: childNameEditor.text
                                }

                                TextInput {
                                    id: childNameEditor
                                    anchors.fill: childNameText
                                    clip: true
                                    visible: foldersView.currentFolderId === childsView.folderId && childsView.currentChildId === childId && editBar.editRole === "edit"
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignHCenter
                                    font.pixelSize: Core.dp(8)
                                    selectByMouse: true
                                    selectionColor: "#c00d0d"
                                    text: child_name
                                    onVisibleChanged: {
                                        if(visible) {
                                            forceActiveFocus()
                                        }
                                    }
                                    onAccepted: editBar.submit()
                                }
                            }

                            ColorAnimation {
                                id: childPressAndHoldAmiation
                                target: childNameContainer
                                property: "color"
                                from: "#dddddd"
                                to: "#da4504"
                                duration: 800
                            }

                            TerminalMouseArea {
                                anchors.fill: parent

                                visible: !foldersView.isEdited
                                onClicked: {
                                    photoSlider.folderId = childsView.folderId
                                    photoSlider.photoId = childDelegate.childId
                                    photoSlider.opacity = 1
                                }
                                onPressedChanged: {
                                    if (pressed) {
                                        childPressAndHoldAmiation.restart()
                                    } else {
                                        childPressAndHoldAmiation.stop()
                                        childNameContainer.color = "#dddddd"
                                    }
                                }

                                onPressAndHold: {
                                    foldersView.currentFolderId = childsView.folderId
                                    foldersView.isChildrenEdited = true
                                    childsView.currentChildId = child_id
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    PhotoSlider {
        id: photoSlider
        iXmlPath: page.iXmlPath
        opacity: 0
        anchors.fill: parent
        visible: opacity > 0
        onHiden: opacity = 0

        Behavior on opacity { NumberAnimation { duration: 200; } }
        z: 2
    }

    EditBar {
        id: editBar
        z: 1

        visible: opacity !== 0
        opacity: foldersView.isEdited?1:0
        anchors.bottomMargin: foldersView.isEdited?0:-Core.dp(22)

        onSubmit: {
            page.dataChanged()
            foldersView.submit()
        }

        onCansel: {
            foldersView.reset()
        }
    }

    function dataToSave() {
        var result = "<photos>\n"
        if (privateData.addFirstFolder) {
            result = result + "  <folder id=\"" + (foldersView.getFolderMaxId() + 1) + "\">\n"
            result = result + "    <name>Новая папка</name>\n"
            result = result + "  </folder>\n"
        }

        for (var folderIndex = 0; folderIndex < foldersModel.count; folderIndex++){
            //Найдем папу соответсвтующую индексу
            var folderItem = foldersView.getFolderDelegate(foldersModel.get(folderIndex).folder_id)

            if (folderItem.isRemoved)
                continue

            result = result + "  <folder id=\"" + folderItem.folderId + "\">\n"
            result = result + "    <name>" + folderItem.nameToSave + "</name>\n"

            result = result + folderItem.dataToSave() + "\n"

            result = result + "  </folder>\n"

            if (folderItem.isAddAfter) {
                result = result + "  <folder id=\"" + (foldersView.getFolderMaxId() + 1) + "\">\n"
                result = result + "    <name>Новая папка</name>\n"
                result = result + "  </folder>\n"
            }

        }

        result = result + "</photos>\n"

        return result
    }

    function reload() {
        privateData.addFirstFolder = false
        foldersView.reload()
        photoSlider.reload()
    }
}
