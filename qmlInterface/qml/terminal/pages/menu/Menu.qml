import QtQuick 2.0
import TerminalQmlPlugin 1.0
import QtQuick.XmlListModel 2.0
import QtGraphicalEffects 1.0
import ".."
import "../../elements"

Page {
    enabled: !progressBar.runing
    id: menuPage

    signal pageLoaded(var page)

    signal pageClicked(var pageName)

    function showError(errorText) {
        errorPage.text = errorText
        errorPage.visible = true
    }

    name: "menu"

    Connections {
        target: proxy
        onAbonBookLoaded: {
            usersContactsModel.reload()
            progressBar.runing=false
            menuPage.pageClicked("contacts")
        }
        onAbonWaitLoaded: {
            usersVisitorsModel.reload()
            progressBar.runing=false
            menuPage.pageClicked("visitors")
        }
        onAbonListLoaded: {
            usersAllModel.reload()
            progressBar.runing=false
            menuPage.pageClicked("all")
        }
        onAbonBookNotLoaded: {
            progressBar.runing=false
            showError("Контакты не загружены")
        }
        onAbonWaitNotLoaded: {
            progressBar.runing=false
            showError("Посетители не загружены")
        }
        onAbonListNotLoaded: {
            progressBar.runing=false
            showError("Список не загружен")
        }
    }

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
        id: items
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

        ProgressBar {
            id: progressBar
            anchors.horizontalCenter: parent.horizontalCenter
            width: terminalLogo.paintedWidth
            anchors.top: terminalLogo.bottom
            anchors.topMargin: Core.dp(8)
            height: runing?Core.dp(2):0
            Behavior on height {NumberAnimation {duration: 200 } }
            onFinished: showError(qsTr("No connection"))
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

                FileWatcher {
                    id: fileWatcher
                    file: Core.dataDir + expected_file
                    onCreated: {
                        if (progressBar.runing) {
                            menuPage.pageClicked(page_name)
                            progressBar.runing = false
                        }
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

                    onClicked: if (expected_file === "") {
                                   menuPage.pageClicked(page_name)
                               } else {
                                   if (fileWatcher.exist()) {
                                       menuPage.pageClicked(page_name)
                                   } else {
                                       progressBar.runing = true
                                   }
                               }
                }

                Loader {
                    source: "../" + qml_page
                    onLoaded: {
                        console.log("loader "+page_name)
                        item.name = page_name
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
                name: "page_name"
                query: "name/string()"
            }
            XmlRole {
                name: "expected_file"
                query: "expected_file/string()"
            }
        }

        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            width: Core.dp(125)
            anchors.top: progressBar.bottom
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

    ErrorPage {
        anchors.fill: parent
        id: errorPage
        visible: false
        blurItem: items
        blurLeftMargin: Core.dp(27)
    }
}
