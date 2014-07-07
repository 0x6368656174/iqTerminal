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
            editRole = ""
    }

    onButtonClicked: {
        if (buttonType === "edit") {
            editRole = "edit"
        } else if (buttonType === "remove") {
            editRole = "remove"
        } else if (buttonType === "add") {
            editRole = "add"
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

    anchors.fill: parent

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
        height: Core.dp(22)
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Row {
            id: editButtons
            anchors.fill: parent
            visible: !isEditedSate

            EditButton {
                type: "selectAll"
                enabled: selectAllButtonEnabled
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "deselectAll"
                enabled: deselectAllButtonEnabled
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "add"
                enabled: addButtonEnabled
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "edit"
                enabled: editButtonEnabled
                width: getButtonWidth()
                onClicked: bar.buttonClicked(type)
            }

            EditButton {
                type: "remove"
                enabled: removeButtonEnabled
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
                width: parent.width / 2
                onClicked: bar.buttonClicked("submit")
            }

            EditButton {
                type: "back"
                width: parent.width / 2
                onClicked: bar.buttonClicked("cancel")
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
