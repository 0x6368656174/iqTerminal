import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtMultimedia 5.2
import QtQuick.Window 2.1

import "pages/welcome"
import "pages/menu"
import "pages/userInfo"
import "pages/chat"
import "elements"

Window {
    id: main
    visible: true
    title: "iqTerminal"
    visibility: Core.operatingSystem === Core.Android?Window.FullScreen:Window.Windowed
    width: 600
    height: 600


    property bool autorized: false

    onAutorizedChanged: {
        if (autorized) {
            showRightPage("menu")
        } else {
            showLeftPage("welcome")
        }
    }

    Image {
        anchors.fill: parent
        fillMode: Image.Tile
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
                if (pages.children[i].name === name)
                    return
                privateData.pageHistory.push(pages.children[i].name)
                pages.children[i].hideLeft()
            }
            if (pages.children[i].name === name) {
                pages.children[i].showRight()
            }
        }
        console.log("Show page \"" + name + "\"")
        proxy.command("show",name);
    }

    function showLeftPage(name) {
        for (var i = 0; i < pages.children.length; i++) {
            if (pages.children[i].visible) {
                if (pages.children[i].name === name)
                    return
                pages.children[i].hideRight()
            }
            if (pages.children[i].name === name) {
                pages.children[i].showLeft()
            }
        }
        console.log("Show page \"" + name + "\"")
        proxy.command("show",name);
    }

    function backPage() {

        var backPageName = privateData.pageHistory.pop()
        console.log("Back to page \"" + backPageName + "\"")
        showLeftPage(backPageName)
    }

    Item {
        id: pages
        anchors.fill: parent

        Welcome {
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

        UserInfoPage {
            id: userInfoPage
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

    AudioPlayer {
        id: audioPlayer
    }
}
