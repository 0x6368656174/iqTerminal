import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import ".."
import "../../elements"

Page {
    id: menuPage

    signal pageLoaded(var page)

    signal pageClicked(var pageName)

    name: "menu"

    BackButton {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: Core.dp(70)
    }

    Rectangle {
        anchors.fill: parent
        color: "#da4504"
    }

    Item {
        anchors.fill: parent
        anchors.leftMargin: Core.dp(27)
        Image {
            id: terminalLogo
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: Core.dp(35)
            source: "../../images/2.png"
            width: parent.width / 3 * 2
            fillMode: Image.PreserveAspectFit
        }

        Component {
            id: menuDelegate
            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: Core.dp(15)
                height: Core.dp(28)
                Row {
                    anchors.fill: parent
                    spacing: Core.dp(8)

                    Item {
                        height: parent.height
                        width: height
                        Image {
                            id: icon
                            width: Core.dp(28)
                            height: width
                            anchors.centerIn: parent
                            source: "../../images/" + image

                            Behavior on width {NumberAnimation {duration: 200} }
                        }
                    }

                    Text {
                        color: "white"
                        font.pixelSize: Core.dp(8)
                        anchors.verticalCenter: parent.verticalCenter
                        width: Core.dp(596)
                        clip: true
                        elide: Text.ElideRight
                        text: title
                    }
                }

                TerminalMouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onContainsMouseChanged: {
                        if (containsMouse) {
                            icon.width = Core.dp(34)
                        } else {
                            icon.width = Core.dp(28)
                        }
                    }

                    onClicked: menuPage.pageClicked(pageName)
                }

                Loader {
                    source: "../" + qml_page
                    onLoaded: {
                        item.name = pageName
                        menuPage.pageLoaded(item)
                    }
                }
            }
        }

        XmlListModel {
            id: menuModel
            source: Core.dataDir + "menu.xml"
            query: "/menu/item"

            XmlRole {
                name: "image"
                query: "image/string()"
            }

            XmlRole {
                name: "title"
                query: "title/string()"
            }

            XmlRole {
                name: "qml_page"
                query: "qml_page/string()"
            }

            XmlRole {
                name: "pageName"
                query: "name/string()"
            }
        }

        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            width: Core.dp(125)
            anchors.top: terminalLogo.bottom
            anchors.bottom: parent.bottom
            anchors.margins: Core.dp(5)
            anchors.horizontalCenterOffset: Core.dp(-25)
            clip: true

            ListView {
                interactive: contentHeight > height
                delegate: menuDelegate
                spacing: Core.dp(5)
                model: menuModel
                anchors.fill: parent
                anchors.topMargin: Core.dp(4)
                anchors.bottomMargin: Core.dp(4)
            }
        }
    }
}
