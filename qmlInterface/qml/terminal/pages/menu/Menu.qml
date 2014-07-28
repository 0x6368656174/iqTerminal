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

    Rectangle {
        anchors.fill: parent
        color: "#da4504"
    }

//    Rectangle {
//        id: leftBackground
//        anchors.top: parent.top
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//        width: Core.dp(150)
//        clip: true

//        Column {
//            anchors.fill: parent
//            spacing: Core.dp(60)
//            Repeater {
//                model: parent.height / Core.dp(260) + 1

//                Rectangle {
//                    width: Core.dp(150)
//                    height: Core.dp(200)
//                    color: "#6e6c77"
//                }
//            }
//        }
//    }

    Image {
        id: border1
        anchors.left: parent.left
        anchors.leftMargin: Core.dp(27)
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: Core.dp(5)
        source: "../../images/3.png"
        fillMode: Image.TileVertically
    }
    Image {
        id: border2
        rotation: 180
        anchors.left: border1.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: border1.width
        source: "../../images/3.png"
        fillMode: Image.TileVertically
    }

    Item {
        anchors.top: parent.top
        anchors.left: border2.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
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
