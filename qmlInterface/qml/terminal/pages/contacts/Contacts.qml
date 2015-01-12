import QtQuick 2.0
import QtQuick.Dialogs 1.1
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import "../.."
import ".."
import "../../elements"

Page {
    id: allPage
    name: "all"

    function showError(error) {
        errorPage.text = error
        errorPage.visible = true
    }

    Item {
        id: items
        anchors.fill: parent

        Component {
            id: contactsModelAdditionalData
            QtObject {
                property bool isSelect: false
            }
        }

        Component.onCompleted: {
            applicationModel.contactsModel.itemAdditionalData = contactsModelAdditionalData
        }

        BackButton {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: (70 * applicationModel.settings.zoomFactor)
        }

        QtObject {
            id: privateData
            property bool isEdited: false
        }

        UsersFilterModel {
            id: usersModel
            filterModel: applicationModel.contactsModel
        }

        GridView {
            id: usersView
            property int columnCount: width / (60 * applicationModel.settings.zoomFactor) < 1?1:width / (60 * applicationModel.settings.zoomFactor)
            interactive: contentHeight > height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: -(2 * applicationModel.settings.zoomFactor)
            anchors.top: parent.top
            anchors.bottom: searchBar.top
            cellHeight: (76 * applicationModel.settings.zoomFactor)
            cellWidth: width/columnCount
            model: usersModel

            add: Transition {
                NumberAnimation { property: "opacity"; from: 0; to: 1.0; duration: 400 }
                NumberAnimation { property: "scale"; from: 0; to: 1.0; duration: 400 }
            }

            displaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 400; easing.type: Easing.OutBounce }
            }

            delegate: Rectangle {
                id: userDelegate
                width: usersView.cellWidth - (2 * applicationModel.settings.zoomFactor)
                height: usersView.cellHeight - (2 * applicationModel.settings.zoomFactor)
                color: "#6c676e"

                Item
                {
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: userNameContainer.top
                    Item{
                        anchors.centerIn: parent
                        width: photoImage.sourceSize.width > parent.width? parent.width:photoImage.sourceSize.width
                        height: photoImage.sourceSize.height > (60 * applicationModel.settings.zoomFactor)?(60 * applicationModel.settings.zoomFactor):photoImage.sourceSize.height
                        Image {
                            id: photoImage
                            anchors.fill: parent
                            source: "image://xml/" + user_info.userProfile.source
                            fillMode: Image.PreserveAspectFit
                        }
                    }
                }

                Rectangle {
                    id: userNameContainer
                    width: parent.width
                    height: (14 * applicationModel.settings.zoomFactor)
                    anchors.bottom: parent.bottom
                    color: "#dddddd"

                    //ВЫБОР
                    Item {
                        id: selectItem
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: (3 * applicationModel.settings.zoomFactor)
                        width: (10 * applicationModel.settings.zoomFactor)
                        height: width

                        Button {
                            id: userCheckButton
                            anchors.centerIn: parent
                            width: privateData.isEdited?(10 * applicationModel.settings.zoomFactor):0
                            height: width
                            Behavior on width {NumberAnimation {duration: 200 } }
                            source: user_additional_data.isSelect?"../../images/34a.png":"../../images/34.png"
                            onClicked: user_additional_data.isSelect = !user_additional_data.isSelect
                        }
                    }

                    //Состояние
                    Image {
                        id: stateImage
                        visible: userCheckButton.width === 0
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: (3 * applicationModel.settings.zoomFactor)
                        width: (10 * applicationModel.settings.zoomFactor)
                        height: width
                        source: {
                            if (!user_friendship_accepted)
                                return "../../images/204c.png"
                            else if (user_info.sms)
                                return "../../images/204b.png"
                            return "../../images/204a.png"
                        }
                    }

                    Text {
                        id: userNameText
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.left: parent.left
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        anchors.leftMargin: (16 * applicationModel.settings.zoomFactor)
                        anchors.rightMargin: (6 * applicationModel.settings.zoomFactor)
                        font.pixelSize: (6 * applicationModel.settings.zoomFactor)
                        text: user_info.userProfile.name
                    }
                }

                ColorAnimation {
                    target: userNameContainer
                    id: userPressAndHoldAmiation
                    property: "color"
                    from: "#dddddd"
                    to: "#da4504"
                    duration: 800
                }

                TerminalMouseArea {
                    anchors.fill: parent

                    visible: !privateData.isEdited
                    onClicked: {
                        if (user_friendship_accepted) {
                            chat.user = usersModel.get(index)
                            proxy.command("chat",index)
                            showRightPage(chat.name)
                        } else
                            showError(qsTr("Waiting adding contacts from ") + user_info.userProfile.name)
                    }
                    onPressedChanged: {
                        if (pressed) {
                            userPressAndHoldAmiation.restart()
                        } else {
                            userPressAndHoldAmiation.stop()
                            userNameContainer.color = "#dddddd"
                        }
                    }

                    onPressAndHold: {
                        privateData.isEdited = true
                    }
                }
            }
        }

        SearchBar {
            id: searchBar
            z: 2
            anchors.bottomMargin: privateData.isEdited?(22 * applicationModel.settings.zoomFactor):0
            Behavior on anchors.bottomMargin {NumberAnimation {duration: 200 } }
            onSearch: {
                usersModel.filterString = text
            }
        }

        EditBar {
            id: editBar
            z: 1
            addButtonEnabled: false
            removeButtonEnabled: false
            editButtonEnabled: false
            canselButtonEnabled: true
            removeUserButtonEnabled: true
            selectAllButtonEnabled: true
            deselectAllButtonEnabled: true
            hideOnMissClick: false
            visible: anchors.bottomMargin > - (22 * applicationModel.settings.zoomFactor)
            anchors.bottomMargin: privateData.isEdited?0:-(22 * applicationModel.settings.zoomFactor)
            onCansel: {
                for (var i = 0; i < usersModel.count; i++) {
                    usersModel.get(i).additionalData.isSelect = false
                }
                privateData.isEdited = false
            }

            onButtonClicked: {
                if (buttonType === "selectAll") {
                    for (var i = 0; i < usersModel.count; i++) {
                        usersModel.get(i).additionalData.isSelect = true
                    }
                } else if (buttonType === "deselectAll") {
                    for (i = 0; i < usersModel.count; i++) {
                        usersModel.get(i).additionalData.isSelect = false
                    }
                }
            }

            onSubmit: {
                for (var i = usersModel.count - 1; i > -1; i--) {
                    var user = usersModel.get(i)
                    if(user.additionalData.isSelect) {
                        proxy.command("abondel",user.id)
                        usersModel.remove(i)
                    }
                }
                privateData.isEdited = false
                applicationModel.contactsModel.save()
            }
        }
    }

    ErrorPage {
        id: errorPage
        anchors.fill: parent
        blurItem: items
    }
}
