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
            progressBar.runing=false
            menuPage.pageClicked("contacts")
        }
        onAbonWaitLoaded: {
            progressBar.runing=false
            menuPage.pageClicked("visitors")
        }
        onAbonListLoaded: {
            progressBar.runing=false
            menuPage.pageClicked("all")
        }
        onAbonBookNotLoaded: {
            progressBar.runing=false
//            menuPage.pageClicked("contacts")
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
        anchors.topMargin: (70 * applicationModel.settings.zoomFactor)
    }

    Rectangle {
        anchors.fill: parent
        color: "#da4504"
    }

    Item {
        id: items
        anchors.fill: parent
        anchors.leftMargin: (27 * applicationModel.settings.zoomFactor)
        Image {
            id: terminalLogo
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: (35 * applicationModel.settings.zoomFactor)
            source: "../../images/2.png"
            width: parent.width / 3 * 2
            fillMode: Image.PreserveAspectFit
        }

        ProgressBar {
            id: progressBar
            anchors.horizontalCenter: parent.horizontalCenter
            width: terminalLogo.paintedWidth
            anchors.top: terminalLogo.bottom
            anchors.topMargin: (8 * applicationModel.settings.zoomFactor)
            height: runing?(2 * applicationModel.settings.zoomFactor):0
            Behavior on height {NumberAnimation {duration: 200 } }
            onFinished: showError(qsTr("No connection"))
        }

        Component {
            id: menuDelegate
            Item {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: (15 * applicationModel.settings.zoomFactor)
                height: (28 * applicationModel.settings.zoomFactor)
                Row {
                    anchors.fill: parent
                    spacing: (8 * applicationModel.settings.zoomFactor)

                    Item {
                        height: parent.height
                        width: height
                        Image {
                            id: icon
                            width: ma.containsMouse?(34 * applicationModel.settings.zoomFactor):(28 * applicationModel.settings.zoomFactor)
                            height: width
                            anchors.centerIn: parent
                            source: "../../images/" + image

                            Behavior on width {NumberAnimation {duration: 200} }
                        }
                    }

                    Text {
                        color: "white"
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        anchors.verticalCenter: parent.verticalCenter
                        width: (596 * applicationModel.settings.zoomFactor)
                        clip: true
                        elide: Text.ElideRight
                        text: qsTranslate("MenuContext", title) + applicationModel.settings.translatorStringEnd
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
                    id: ma
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: if (expected_file === "") {
                                   menuPage.pageClicked(page_name)
                               } else {
                                   if(page_name==="contacts") {
                                       //progressBar.runing=true
                                       proxy.command("abonbook","")
                                       menuPage.pageClicked("contacts")
                                   }
                                   if(page_name==="visitors")  {
                                       progressBar.runing=true
                                       proxy.command("abonwait","")
                                   }
                                   if(page_name==="all") {
                                       progressBar.runing=true
                                       proxy.command("abonlist","%%")
                                   }
                               }
                }

                Loader {
                    source: "../" + qml_page
                    onLoaded: {
                        item.name = page_name
                        menuPage.pageLoaded(item)
                    }
                }
            }
        }

        ListModel {
            id: menuModel
            ListElement {
                page_name: "i"
                title: QT_TRANSLATE_NOOP("MenuContext", "I")
                image: "13.png"
                qml_page: "i/I.qml"
                expected_file: ""
            }
            ListElement {
                page_name: "all"
                title: QT_TRANSLATE_NOOP("MenuContext", "ALL")
                image: "15.png"
                qml_page: "all/All.qml"
                expected_file: "all.xml"
            }
            ListElement {
                page_name: "contacts"
                title: QT_TRANSLATE_NOOP("MenuContext", "CONTACTS")
                image: "16.png"
                qml_page: "contacts/Contacts.qml"
                expected_file: "contacts.xml"
            }
            ListElement {
                page_name: "visitors"
                title: QT_TRANSLATE_NOOP("MenuContext", "VISITORS")
                image: "17.png"
                qml_page: "visitors/Visitors.qml"
                expected_file: "visitors.xml"
            }
            ListElement {
                page_name: "torrent"
                title: QT_TRANSLATE_NOOP("MenuContext", "TORRENT")
                image: "14.png"
                qml_page: "torrent/Torrent.qml"
                expected_file: ""
            }
            ListElement {
                page_name: "settings"
                title: QT_TRANSLATE_NOOP("MenuContext", "SETTINGS")
                image: "18.png"
                qml_page: "settings/Settings.qml"
                expected_file: ""
            }
        }

        Item {
            anchors.horizontalCenter: parent.horizontalCenter
            width: (125 * applicationModel.settings.zoomFactor)
            anchors.top: progressBar.bottom
            anchors.bottom: parent.bottom
            anchors.margins: (5 * applicationModel.settings.zoomFactor)
            anchors.horizontalCenterOffset: (-25 * applicationModel.settings.zoomFactor)
            clip: true

            ListView {
                interactive: contentHeight > height
                delegate: menuDelegate
                spacing: (5 * applicationModel.settings.zoomFactor)
                model: menuModel
                anchors.fill: parent
                anchors.topMargin: (4 * applicationModel.settings.zoomFactor)
                anchors.bottomMargin: (4 * applicationModel.settings.zoomFactor)
            }
        }
    }

    ErrorPage {
        anchors.fill: parent
        id: errorPage
        visible: false
        blurItem: items
        blurLeftMargin: (27 * applicationModel.settings.zoomFactor)
    }
}
