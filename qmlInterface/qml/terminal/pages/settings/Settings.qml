import QtQuick 2.0
import TerminalQmlPlugin 1.0
import ".."
import "../../elements"


Page {
    id: torrentPage
    name: "torrent"

    BackButton {
        id: backButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: (70 * applicationModel.settings.zoomFactor)
    }

    Rectangle {
        id: title
        anchors.left: parent.left
        anchors.right: parent.right
        height: (24 * applicationModel.settings.zoomFactor)
        color: "#c6c1c7"

        Text {
            anchors.fill: parent
            anchors.leftMargin: font.pixelSize
            anchors.rightMargin: font.pixelSize
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: (8 * applicationModel.settings.zoomFactor)
            text: qsTr("Settings") + applicationModel.settings.translatorStringEnd
            elide: Text.ElideRight
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: title.bottom
        anchors.bottom: parent.bottom
        clip: true
        Flickable {
            anchors.fill: parent
            contentHeight: itemColumn.height
            contentWidth: width

            Column {
                id: itemColumn
                anchors.left: parent.left
                anchors.right: parent.right
                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: font.pixelSize
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("Language") + applicationModel.settings.translatorStringEnd
                }

                Separator {
                }

                ListModel {
                    id: translationsModel
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "English")
                        translation: Settings.English
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "Chinese")
                        translation: Settings.Chinese
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "Spanish")
                        translation: Settings.Spanish
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "Japanese")
                        translation: Settings.Japanese
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "Portuguese")
                        translation: Settings.Portuguese
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "German")
                        translation: Settings.German
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "Arabic")
                        translation: Settings.Arabic
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "French")
                        translation: Settings.French
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "Russian")
                        translation: Settings.Russian
                    }
                    ListElement {
                        translation_name: QT_TRANSLATE_NOOP("SettingsContext", "Korean")
                        translation: Settings.Korean
                    }
                }

                ListView {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: translationsModel.count*(24 * applicationModel.settings.zoomFactor)
                    model: translationsModel
                    interactive: false
                    delegate: Item {
                        height: (24 * applicationModel.settings.zoomFactor)
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)

                        ClickedText {
                            anchors.left: parent.left
                            anchors.right: selectImage.left
                            anchors.rightMargin: font.pixelSize
                            text: qsTranslate("SettingsContext", translation_name) + applicationModel.settings.translatorStringEnd
                            onClicked: {
                                applicationModel.settings.translation = translation
                                applicationModel.settings.save()
                            }
                        }
                        Image {
                            id: selectImage
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            height: (10 * applicationModel.settings.zoomFactor)
                            width: height
                            anchors.rightMargin: (20 * applicationModel.settings.zoomFactor)
                            source: "../../images/33.png"
                            visible: applicationModel.settings.translation === translation
                        }
                        Separator {
                            anchors.bottom: parent.bottom
                        }
                    }
                }


                //ФАЙЛОВАЯ СИСТЕМА
                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: font.pixelSize
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("File System") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                }

                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("Home Directory") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                }

                ClickedText {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)
                    anchors.rightMargin: font.pixelSize
                    text: applicationModel.settings.homeDirectory

                    onClicked: fileDialog.open()
                }
                Separator {
                    anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)
                }

                //МУЛЬТИМЕДИ ДЕВАЙС
                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: font.pixelSize
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("Multimedia Device") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                }
                //АУДИО
                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("Audio In") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                }

                ListView {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: applicationModel.settings.audioDevicesModel.count*(24 * applicationModel.settings.zoomFactor)
                    model: applicationModel.settings.audioDevicesModel
                    interactive: false
                    delegate: Item {
                        height: (24 * applicationModel.settings.zoomFactor)
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)

                        ClickedText {
                            anchors.left: parent.left
                            anchors.right: selectAudionImage.left
                            anchors.rightMargin: font.pixelSize
                            text: device_name
                            onClicked: {
                                applicationModel.settings.audioDevicesModel.get(index).active = true
                                applicationModel.settings.save()
                            }
                        }
                        Image {
                            id: selectAudionImage
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            height: (10 * applicationModel.settings.zoomFactor)
                            width: height
                            anchors.rightMargin: (20 * applicationModel.settings.zoomFactor)
                            source: "../../images/33.png"
                            visible: device_active
                        }
                        Separator {
                            anchors.bottom: parent.bottom
                        }
                    }
                }

                //ВИДЕО
                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("Video In") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                }

                ListView {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: applicationModel.settings.videoDevicesModel.count*(24 * applicationModel.settings.zoomFactor)
                    model: applicationModel.settings.videoDevicesModel
                    interactive: false
                    delegate: Item {
                        height: (24 * applicationModel.settings.zoomFactor)
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)

                        ClickedText {
                            anchors.left: parent.left
                            anchors.right: selectVideoImage.left
                            anchors.rightMargin: font.pixelSize
                            text: device_name
                            onClicked: {
                                applicationModel.settings.videoDevicesModel.get(index).active = true
                                applicationModel.settings.save()
                            }
                        }
                        Image {
                            id: selectVideoImage
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            height: (10 * applicationModel.settings.zoomFactor)
                            width: height
                            anchors.rightMargin: (20 * applicationModel.settings.zoomFactor)
                            source: "../../images/33.png"
                            visible: device_active
                        }
                        Separator {
                            anchors.bottom: parent.bottom
                        }
                    }
                }

                //СЕТЬ
                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: font.pixelSize
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("Net") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                }
                //WAN
                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("WAN") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                }

                ListView {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: applicationModel.settings.wanDevicesModel.count*(24 * applicationModel.settings.zoomFactor)
                    model: applicationModel.settings.wanDevicesModel
                    interactive: false
                    delegate: Item {
                        height: (24 * applicationModel.settings.zoomFactor)
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)

                        ClickedText {
                            anchors.left: parent.left
                            anchors.right: selectWanImage.left
                            anchors.rightMargin: font.pixelSize
                            text: qsTr("IP: ") + network_device_ip + "\n" + qsTr("Port: ") + network_device_port + applicationModel.settings.translatorStringEnd
                            onClicked: {
                                applicationModel.settings.wanDevicesModel.get(index).active = true
                                applicationModel.settings.save()
                            }
                        }
                        Image {
                            id: selectWanImage
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            height: (10 * applicationModel.settings.zoomFactor)
                            width: height
                            anchors.rightMargin: (20 * applicationModel.settings.zoomFactor)
                            source: "../../images/33.png"
                            visible: network_device_active
                        }
                        Separator {
                            anchors.bottom: parent.bottom
                        }
                    }
                }
                //NAT
                Text {
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("NAT") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                }
                //NAT-PMP
                Item {
                    height: (24 * applicationModel.settings.zoomFactor)
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)

                    ClickedText {
                        anchors.left: parent.left
                        anchors.right: selectNatImage.left
                        anchors.rightMargin: font.pixelSize
                        text: qsTr("NAT-PMP on/off") + applicationModel.settings.translatorStringEnd
                    }
                    Image {
                        id: selectNatImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        height: (16 * applicationModel.settings.zoomFactor)
                        width: height*3
                        fillMode: Image.PreserveAspectFit
                        anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                        source: applicationModel.settings.natEnabled?"../../images/206a.png":"../../images/206.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            applicationModel.settings.natEnabled = !applicationModel.settings.natEnabled
                            applicationModel.settings.save()
                        }
                    }
                    Separator {
                        anchors.bottom: parent.bottom
                    }
                }
                //UPnP
                Item {
                    height: (24 * applicationModel.settings.zoomFactor)
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)

                    ClickedText {
                        anchors.left: parent.left
                        anchors.right: selectUpnpImage.left
                        anchors.rightMargin: font.pixelSize
                        text: qsTr("NAT-PMP on/off") + applicationModel.settings.translatorStringEnd
                    }
                    Image {
                        id: selectUpnpImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        height: (16 * applicationModel.settings.zoomFactor)
                        width: height*3
                        fillMode: Image.PreserveAspectFit
                        anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                        source: applicationModel.settings.upnpEnabled?"../../images/206a.png":"../../images/206.png"
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            applicationModel.settings.upnpEnabled = !applicationModel.settings.upnpEnabled
                            applicationModel.settings.save()
                        }
                    }
                    Separator {
                        anchors.bottom: parent.bottom
                    }
                }

                //Proxy
                Text {
                    id: proxy
                    function maxTitleWidth() {
                        var max = ipTitle.width
                        if (portTitle.width > max)
                            max = portTitle.width
                        if (userTitle.width > max)
                            max = userTitle.width
                        if (passwordTitle.width > max)
                            max = passwordTitle.width
                        return max
                    }
                    height: (24 * applicationModel.settings.zoomFactor)
                    verticalAlignment: Text.AlignVCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                    anchors.rightMargin: font.pixelSize
                    elide: Text.ElideRight
                    text: qsTr("Proxy") + applicationModel.settings.translatorStringEnd
                }
                Separator {
                    anchors.leftMargin: (20 * applicationModel.settings.zoomFactor)
                }

                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)
                    height: (24 * applicationModel.settings.zoomFactor)
                    Text {
                        id: ipTitle
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: qsTr("IP") + applicationModel.settings.translatorStringEnd
                    }
                    TextInput {
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: proxy.maxTitleWidth() + font.pixelSize
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        text: applicationModel.settings.proxySettings.ip
                        onTextChanged: {
                            applicationModel.settings.proxySettings.ip = text
                            applicationModel.settings.save()
                        }
                    }

                    Separator {
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)
                    height: (24 * applicationModel.settings.zoomFactor)
                    Text {
                        id: portTitle
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: qsTr("Port") + applicationModel.settings.translatorStringEnd
                    }
                    TextInput {
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: proxy.maxTitleWidth() + font.pixelSize
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        text: applicationModel.settings.proxySettings.port
                        onTextChanged: {
                            applicationModel.settings.proxySettings.port = text
                            applicationModel.settings.save()
                        }
                    }

                    Separator {
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)
                    height: (24 * applicationModel.settings.zoomFactor)
                    Text {
                        id: userTitle
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: qsTr("User") + applicationModel.settings.translatorStringEnd
                    }
                    TextInput {
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: proxy.maxTitleWidth() + font.pixelSize
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        text: applicationModel.settings.proxySettings.user
                        onTextChanged: {
                            applicationModel.settings.proxySettings.user = text
                            applicationModel.settings.save()
                        }
                    }

                    Separator {
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (40 * applicationModel.settings.zoomFactor)
                    height: (24 * applicationModel.settings.zoomFactor)
                    Text {
                        id: passwordTitle
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: qsTr("Password") + applicationModel.settings.translatorStringEnd
                    }
                    TextInput {
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: proxy.maxTitleWidth() + font.pixelSize
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        selectByMouse: true
                        selectionColor: "#c00d0d"
                        text: applicationModel.settings.proxySettings.password
                        onTextChanged: {
                            applicationModel.settings.proxySettings.password = text
                            applicationModel.settings.save()
                        }
                    }

                    Separator {
                        anchors.bottom: parent.bottom
                    }
                }
                //ЗООМ
                Item {
                    height: (24 * applicationModel.settings.zoomFactor)
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)

                    Text {
                        id: zoomTitle
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: qsTr("Zoom") + applicationModel.settings.translatorStringEnd
                    }

                    Text {
                        id: zoomText
                        anchors.left: zoomTitle.right
                        anchors.leftMargin: (8 * applicationModel.settings.zoomFactor)
                        anchors.top: parent.top
                        anchors.bottom: parent.bottom
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: (8 * applicationModel.settings.zoomFactor)
                        text: Math.round(applicationModel.settings.zoomFactor*100) + "%"
                    }

                    Image {
                        id: zoomInImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: zoomOutImage.left
                        anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                        source: "../../images/207.png"
                        height: (16 * applicationModel.settings.zoomFactor)
                        width: (16 * applicationModel.settings.zoomFactor)
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                applicationModel.settings.zoomFactor = applicationModel.settings.zoomFactor*1.25
                                applicationModel.settings.save()
                            }
                        }
                    }

                    Image {
                        id: zoomOutImage
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: (8 * applicationModel.settings.zoomFactor)
                        source: "../../images/208.png"
                        height: (16 * applicationModel.settings.zoomFactor)
                        width: (16 * applicationModel.settings.zoomFactor)
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                applicationModel.settings.zoomFactor = applicationModel.settings.zoomFactor/1.25
                                applicationModel.settings.save()
                            }
                        }
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        selectMultiple: false
        selectFile: false
        selectFolder: true
        title: qsTr("Select folder") + applicationModel.settings.translatorStringEnd
        height: userInfoPage.height
        onVisibleChanged: backButton.visible = !visible
        onAccepted: {
            applicationModel.settings.homeDirectory = fileUrl
            applicationModel.settings.save()
        }
    }
}
