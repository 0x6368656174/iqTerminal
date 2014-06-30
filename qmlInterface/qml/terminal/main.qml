import QtQuick 2.0
import TerminalQmlPlugin 1.0

import "pages/welcome"
import "pages/menu"

Rectangle {
    width: Core.dp(208)
    height: Core.dp(285)

    function showRightPage(name) {
        for (var i = 0; i < pages.children.length; i++) {
            pages.children[i].visible = false
            if (pages.children[i].name === name) {
                console.log("Show page \"" + name + "\"")
                pages.children[i].visible = true
            }
        }
    }

    Item {
        id: pages
        anchors.fill: parent

        Welcome {
            onSignInCompleted: showRightPage("menu")
        }

        Menu {
            visible: false

            onPageLoaded: addMenuPage(page)
            onPageClicked: showRightPage(pageName)

            function addMenuPage(page) {
                page.parent = pages
                page.visible = false
            }
        }
    }
}
