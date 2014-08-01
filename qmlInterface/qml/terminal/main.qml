import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtMultimedia 5.2

import "pages/welcome"
import "pages/menu"
import "pages/userInfo"
import "pages/chat"
import "elements"

Rectangle {
    id: main
    width: Core.dp(208)
    height: Core.dp(285)

    Image {
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        source: "images/1a.png"
    }

    Rectangle {
        anchors.fill: parent
        color: "#b0706a78"
    }

    QtObject {
        id: privateData
        property var pageHistory
        Component.onCompleted: {
            pageHistory = []
        }
    }

    function showRightPage(name) {
        for (var i = 0; i < pages.children.length; i++) {
            if (pages.children[i].visible) {
                privateData.pageHistory.push(pages.children[i].name)
                pages.children[i].hideLeft()
            }
            if (pages.children[i].name === name) {
                pages.children[i].showRight()
            }
        }
        console.log("Show page \"" + name + "\"")
    }

    function showLeftPage(name) {
        for (var i = 0; i < pages.children.length; i++) {
            if (pages.children[i].visible) {
                pages.children[i].hideRight()
            }
            if (pages.children[i].name === name) {
                pages.children[i].showLeft()
            }
        }
        console.log("Show page \"" + name + "\"")
    }

    function backPage() {
        var backPageName = privateData.pageHistory.pop()
        console.log("Back to page \"" + backPageName + "\"")
        showLeftPage(backPageName)
    }

    Item {
        id: pages
        anchors.fill: parent

        //Модели

        UsersModel {
            id: usersAllModel
            source: Core.dataDir + "/all.xml"
            parentElement: "all"
            itemAdditionalData: QtObject {
                property bool isSelect: false
            }
        }

        UsersModel {
            id: usersContactsModel
            source: Core.dataDir + "/contacts.xml"
            parentElement: "all"
            itemAdditionalData: QtObject {
                property bool isSelect: false
            }
        }

        UsersModel {
            id: usersVisitorsModel
            source: Core.dataDir + "/visitors.xml"
            parentElement: "all"
            itemAdditionalData: QtObject {
                property bool isSelect: false
            }
        }

        Welcome {
            onSignInCompleted: showRightPage("menu")
        }

        Menu {
            visible: false
            onPageLoaded: addMenuPage(page)
            onPageClicked: showRightPage(pageName)

            function addMenuPage(page) {
                page.visible = false
                page.parent = pages
            }
        }

        UserInfo {
            id: userInfo
            visible: false
            readOnly: true
        }

        Chat {
            id: chat
            visible: false
        }
    }

    MouseArea {
        id: disableMouse
        visible: {
            for (var i = 0; i < pages.children.length; i++) {
                if (pages.children[i].showHideAnimationRunning)
                    return true
            }
            return false
        }

        anchors.fill: parent
        onClicked: {
        }
        onPressAndHold: {
        }
    }

    Audio {
        id: audioPlayer
        property bool isPlay: false
        property bool isPause: false

        onPlaying: {
            isPlay = true
            isPause = false
        }
        onPaused:  {
            isPlay = false
            isPause = true
        }
        onStopped: {
            isPlay = false
            isPause = false
        }
    }
}
