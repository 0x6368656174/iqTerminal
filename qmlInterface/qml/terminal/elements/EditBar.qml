import QtQuick 2.0
import TerminalQmlPlugin 1.0

Item {
    id: bar
    property bool isEditedSate: false
    property bool addButtonEnabled: true
    property bool editButtonEnabled: true
    property bool removeButtonEnabled: true
    property bool selectAllButtonEnabled: false
    property bool deselectAllButtonEnabled: false
    property bool canselButtonEnabled: false
    property bool addUserButtonEnabled: false
    property bool removeUserButtonEnabled: false

    property bool hideOnMissClick: true

    property string editRole: ""
    onEditRoleChanged: {
        if (editRole !=="") {
            isEditedSate = true
        } else {
            isEditedSate = false
        }
    }

    onVisibleChanged: {
        if (!visible)
            editRole = ""
    }

    onButtonClicked: {
        if (buttonType === "edit") {
            editRole = "edit"
        } else if (buttonType === "remove") {
            editRole = "remove"
        } else if (buttonType === "add") {
            editRole = "add"
        } else if (buttonType === "addUser") {
            editRole = "addUser"
        } else if (buttonType === "removeUser") {
            editRole = "removeUser"
        } else if (buttonType === "submit") {
            bar.submit()
        } else if (buttonType === "cancel") {
            bar.cansel()
        }
    }

    onHidenClicked: {
        cansel()
    }

    Behavior on opacity {NumberAnimation { duration: 200} }
    Behavior on anchors.bottomMargin {NumberAnimation {duration: 200} }

    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.left: parent.left

    signal buttonClicked(var buttonType)
    signal hidenClicked()
    signal submit()
    signal cansel()

    function getButtonWidth() {
        var visibleButtonCount = 0
        for(var i = 0; i < editButtons.children.length; i++){
            if (editButtons.children[i].enabled){
                visibleButtonCount++
            }
        }

        return bar.width / visibleButtonCount
    }

    TerminalMouseArea {
        anchors.top: parent.top
        anchors.bottom: barRow.top
        anchors.left: parent.left
        anchors.right: parent.right
        enabled: hideOnMissClick
        visible: !isEditedSate
        onClicked: {
            bar.hidenClicked()
        }
    }

    Item {
        id: barRow
        height: 22 * applicationModel.settings.zoomFactor
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Row {
            id: editButtons
            anchors.fill: parent
            visible: !isEditedSate
            EditButton {
                type: "cancel"
                enabled: canselButtonEnabled
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "deselectAll"
                enabled: deselectAllButtonEnabled
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "selectAll"
                enabled: selectAllButtonEnabled
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "add"
                enabled: addButtonEnabled
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "edit"
                enabled: editButtonEnabled
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "remove"
                enabled: removeButtonEnabled
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "addUser"
                enabled: addUserButtonEnabled
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "removeUser"
                enabled: removeUserButtonEnabled
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }
        }

        Row {
            id: saveButtons
            anchors.fill: parent
            visible: isEditedSate
            EditButton {
                type: "submit"
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: parent.width / 2
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "cancel"
                mouseAreaActive: bar.anchors.bottomMargin === 0
                width: parent.width / 2
                onClicked: bar.buttonClicked(type)
            }
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: barRow.top
        height: 1
        color: "#c6c1c7"
    }
}
