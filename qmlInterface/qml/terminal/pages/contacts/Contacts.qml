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
            anchors.topMargin: Core.dp(70)
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
            property int columnCount: width / Core.dp(60) < 1?1:width / Core.dp(60)
            interactive: contentHeight > height
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: -Core.dp(2)
            anchors.top: parent.top
            anchors.bottom: searchBar.top
            cellHeight: Core.dp(76)
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
                width: usersView.cellWidth - Core.dp(2)
                height: usersView.cellHeight - Core.dp(2)
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
                        height: photoImage.sourceSize.height > Core.dp(60)?Core.dp(60):photoImage.sourceSize.height
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
                    height: Core.dp(14)
                    anchors.bottom: parent.bottom
                    color: "#dddddd"

                    //ВЫБОР
                    Item {
                        id: selectItem
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: Core.dp(3)
                        width: Core.dp(10)
                        height: width

                        Button {
                            id: userCheckButton
                            anchors.centerIn: parent
                            width: privateData.isEdited?Core.dp(10):0
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
                        anchors.leftMargin: Core.dp(3)
                        width: Core.dp(10)
                        height: width
                        source: {
                            if (!user_friendship_accepted)
                                return "../../images/204c.png"
                            else if (user_online)
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
                        anchors.leftMargin: Core.dp(16)
                        anchors.rightMargin: Core.dp(6)
                        font.pixelSize: Core.dp(6)
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
                            chat.userProfile = user_profile
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
            anchors.bottomMargin: privateData.isEdited?Core.dp(22):0
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
            visible: anchors.bottomMargin > - Core.dp(22)
            anchors.bottomMargin: privateData.isEdited?0:-Core.dp(22)
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
                usersContactsModel.save()
            }
        }
    }

    ErrorPage {
        id: errorPage
        anchors.fill: parent
        blurItem: items
    }
}
