import QtQuick 2.0
import TerminalQmlPlugin 1.0
import Qt.labs.settings 1.0

import ".."
import "." as Welcome
import "../../elements" as Elements

Page {
    id: welcomePage

    function showError (error) {
        progressBar.runing = false
        errorText.text = error
        errorMessage.opacity = 1
        errorMessage.visible = true
        flickableItem.contentY = flickableItem.contentHeight - flickableItem.height
        showErrorAmination.restart()
    }

    function signIn(login, password) {
//        return "Bad login or password"
        main.autorized = false
        progressBar.runing = true
        main.autorized = true
        return ""
    }

    function register (login, email, name) {
        progressBar.runing = true
    }

    function remindPassword(email) {
        progressBar.runing = true
    }

    function changePassword(oldPassword, newPassword) {
        progressBar.runing = true
    }

    Connections {
        target: main
        onAutorizedChanged: {
            progressBar.runing = false
            errorText.text = ""
            errorMessage.opacity = 0
            errorMessage.visible = false
        }
    }

    name: "welcome"
    state: "signInPage"

    Rectangle {
        anchors.fill: parent
        color: "#da4504"
    }

    states: [
        State {
            name: "signInPage"
            PropertyChanges {
                target: buttonTitleText
                text: qsTr("Sign In")
            }
            PropertyChanges {
                target: signInPage
                opacity: 1
            }
            PropertyChanges {
                target: registerPage
                opacity: 0
            }
            PropertyChanges {
                target: remindPage
                opacity: 0
            }
            PropertyChanges {
                target: changePage
                opacity: 0
            }
            PropertyChanges {
                target: signInButton
                source: "../../images/5a.png"
            }
            PropertyChanges {
                target: registerButton
                source: "../../images/6.png"
            }
            PropertyChanges {
                target: remindButton
                source: "../../images/7.png"
            }
            PropertyChanges {
                target: changeButton
                source: "../../images/8.png"
            }
            PropertyChanges {
                target: errorMessage
                opacity: 0
                visible: false
            }
        },
        State {
            name: "registerPage"
            PropertyChanges {
                target: buttonTitleText
                text: qsTr("Register")
            }
            PropertyChanges {
                target: signInPage
                opacity: 0
            }
            PropertyChanges {
                target: registerPage
                opacity: 1
            }
            PropertyChanges {
                target: remindPage
                opacity: 0
            }
            PropertyChanges {
                target: changePage
                opacity: 0
            }
            PropertyChanges {
                target: signInButton
                source: "../../images/5.png"
            }
            PropertyChanges {
                target: registerButton
                source: "../../images/6a.png"
            }
            PropertyChanges {
                target: remindButton
                source: "../../images/7.png"
            }
            PropertyChanges {
                target: changeButton
                source: "../../images/8.png"
            }
            PropertyChanges {
                target: errorMessage
                opacity: 0
                visible: false
            }
        },
        State {
            name: "remindPage"
            PropertyChanges {
                target: buttonTitleText
                text: qsTr("Remind Password")
            }
            PropertyChanges {
                target: signInPage
                opacity: 0
            }
            PropertyChanges {
                target: registerPage
                opacity: 0
            }
            PropertyChanges {
                target: remindPage
                opacity: 1
            }
            PropertyChanges {
                target: changePage
                opacity: 0
            }
            PropertyChanges {
                target: signInButton
                source: "../../images/5.png"
            }
            PropertyChanges {
                target: registerButton
                source: "../../images/6.png"
            }
            PropertyChanges {
                target: remindButton
                source: "../../images/7a.png"
            }
            PropertyChanges {
                target: changeButton
                source: "../../images/8.png"
            }
            PropertyChanges {
                target: errorMessage
                opacity: 0
                visible: false
            }
        },
        State {
            name: "changePage"
            PropertyChanges {
                target: buttonTitleText
                text: qsTr("Change Password")
            }
            PropertyChanges {
                target: signInPage
                opacity: 0
            }
            PropertyChanges {
                target: registerPage
                opacity: 0
            }
            PropertyChanges {
                target: remindPage
                opacity: 0
            }
            PropertyChanges {
                target: changePage
                opacity: 1
            }
            PropertyChanges {
                target: signInButton
                source: "../../images/5.png"
            }
            PropertyChanges {
                target: registerButton
                source: "../../images/6.png"
            }
            PropertyChanges {
                target: remindButton
                source: "../../images/7.png"
            }
            PropertyChanges {
                target: changeButton
                source: "../../images/8a.png"
            }
            PropertyChanges {
                target: errorMessage
                opacity: 0
                visible: false
            }
        }
    ]

    Item {
        anchors.fill: parent
        enabled: !progressBar.runing

        Image {
            id: bakgrounImage
            anchors.centerIn: parent
            fillMode: Image.PreserveAspectFit
            width: parent.width / 5 * 4
            Behavior on opacity {NumberAnimation {duration: 200} }
            opacity: welcomePage.state !== "start"?0.6:0
            source: "../../images/4.png"
        }

        Image {
            id: terminalLogo
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: Core.dp(35)
            source: "../../images/2.png"
            width: parent.width / 3 * 2
            fillMode: Image.PreserveAspectFit
        }

        Elements.ProgressBar {
            id: progressBar
            anchors.horizontalCenter: parent.horizontalCenter
            width: terminalLogo.paintedWidth
            anchors.top: terminalLogo.bottom
            anchors.topMargin: Core.dp(8)
            height: runing?Core.dp(2):0
            Behavior on height {NumberAnimation {duration: 200 } }
            onFinished: showError(qsTr("No connection"))
        }

        Item {
            id: buttonTitle
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: progressBar.bottom
            anchors.topMargin: progressBar.runing?Core.dp(8):0
            Behavior on anchors.topMargin {NumberAnimation {duration: 200 } }
            height: Core.dp(12)

            Text {
                id: buttonTitleText
                anchors.centerIn: parent
                color: "white"
                font.pixelSize: Core.dp(8)
            }
            Elements.TerminalMouseArea {
                id: buttonTitleMA
                anchors.fill: parent
                platformIndependentHoverEnabled: true
                onContainsMouseChanged:  {
                    if (containsMouse) {
                        buttonTitleText.font.pixelSize = Core.dp(10)
                    } else {
                        buttonTitleText.font.pixelSize = Core.dp(8)
                    }
                }

                function click() {
                    if (welcomePage.state === "signInPage") {
                        var signInResult = welcomePage.signIn(signInLogin.text, signInPassword.text)
                        if (signInResult !== "") {
                            showError(signInResult)
                        }
                    } else if (welcomePage.state === "registerPage") {
                        welcomePage.register(registerLogin.text, registerEmail.text, registerName.text);
                    } else if (welcomePage.state === "remindPage") {
                        welcomePage.remindPassword(remindEmal.text);
                    } else if (welcomePage.state === "changePage") {
                        if (changeNewPassword.text !== changeRepeatPassword.text) {
                            showError(qsTr("Passwords do not match"))
                        } else {
                            welcomePage.changePassword(changeOldPassword.text, changeNewPassword.text)
                        }
                    }
                }

                onClicked: click()
            }
        }

        Row {
            id: buttonsRow
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: buttonTitle.bottom
            anchors.topMargin: Core.dp(4)
            height: Core.dp(28)
            width: Core.dp(28)*4+Core.dp(4)*3
            spacing: Core.dp(4)

            Item {
                width: Core.dp(28)
                height: width
                Welcome.Button {
                    id: signInButton
                    anchors.centerIn: parent
                    source: "../../images/5.png"
                    onClicked: welcomePage.state = "signInPage"
                }
            }
            Item {
                width: Core.dp(28)
                height: width
                Welcome.Button {
                    id: registerButton
                    anchors.centerIn: parent
                    source: "../../images/6.png"
                    onClicked: welcomePage.state = "registerPage"
                }

            }Item {
                width: Core.dp(28)
                height: width
                Welcome.Button {
                    id: remindButton
                    anchors.centerIn: parent
                    source: "../../images/7.png"
                    onClicked: welcomePage.state = "remindPage"
                }
            }Item {
                width: Core.dp(28)
                height: width
                enabled: main.autorized
                Welcome.Button {
                    id: changeButton
                    anchors.centerIn: parent
                    source: "../../images/8.png"
                    onClicked: welcomePage.state = "changePage"
                }
            }
        }

        Flickable {
            id: flickableItem
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: buttonsRow.bottom
            anchors.topMargin: Core.dp(8)
            anchors.bottom: saveUser.top
            anchors.bottomMargin: Core.dp(4)
            contentWidth: parent.width
            contentHeight: form.height + errorMessage.height*errorMessage.visible + errorMessage.anchors.topMargin*errorMessage.visible
            interactive: contentHeight > height
            clip: true

            Item {
                id: pages
                anchors.fill: parent

                Item {
                    id: form
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: {
                        for (var i = 0; i < form.children.length; i++) {
                            if (form.children[i].opacity === 1)
                                return form.children[i].height
                        }
                    }

                    Column {
                        id: signInPage
                        opacity: 0
                        visible: opacity !== 0
                        spacing: Core.dp(12)
                        anchors.left: parent.left
                        anchors.right: parent.right
                        onVisibleChanged: {
                            if (!saveUser.passwordSaved) {
                                signInLogin.text = ""
                                signInPassword.text = ""
                            }
                        }

                        Behavior on opacity {NumberAnimation {duration: 200;} }

                        Welcome.LineEdit {
                            id: signInLogin
                            placeholderText: qsTr("LOGIN")
                            anchors.horizontalCenter: parent.horizontalCenter
                            KeyNavigation.tab: signInPassword
                            onSubmit: buttonTitleMA.click()
                        }
                        Welcome.LineEdit {
                            id: signInPassword
                            placeholderText: qsTr("PASSWORD")
                            anchors.horizontalCenter: parent.horizontalCenter
                            password: true
                            KeyNavigation.tab: signInLogin
                            onSubmit: buttonTitleMA.click()
                        }
                    }

                    Column {
                        id: registerPage
                        opacity: 0
                        visible: opacity !== 0
                        spacing: Core.dp(12)
                        anchors.left: parent.left
                        anchors.right: parent.right
                        onVisibleChanged: {
                            registerLogin.text = ""
                            registerEmail.text = ""
                            registerName.text = ""
                        }

                        Behavior on opacity {NumberAnimation {duration: 200;} }

                        Welcome.LineEdit {
                            id: registerLogin
                            placeholderText: qsTr("LOGIN")
                            anchors.horizontalCenter: parent.horizontalCenter
                            KeyNavigation.tab: registerEmail
                            onSubmit: buttonTitleMA.click()
                        }
                        Welcome.LineEdit {
                            id: registerEmail
                            placeholderText: qsTr("EMAIL")
                            anchors.horizontalCenter: parent.horizontalCenter
                            KeyNavigation.tab: registerName
                            onSubmit: buttonTitleMA.click()
                        }
                        Welcome.LineEdit {
                            id: registerName
                            placeholderText: qsTr("YOUR NAME")
                            anchors.horizontalCenter: parent.horizontalCenter
                            KeyNavigation.tab: registerLogin
                            onSubmit: buttonTitleMA.click()
                        }
                    }

                    Column {
                        id: remindPage
                        opacity: 0
                        visible: opacity !== 0
                        spacing: Core.dp(12)
                        anchors.left: parent.left
                        anchors.right: parent.right
                        onVisibleChanged: {
                            remindEmal.text = ""
                        }

                        Behavior on opacity {NumberAnimation {duration: 200;} }

                        Welcome.LineEdit {
                            id: remindEmal
                            placeholderText: qsTr("EMAIL")
                            anchors.horizontalCenter: parent.horizontalCenter
                            onSubmit: buttonTitleMA.click()
                        }
                    }

                    Column {
                        id: changePage
                        opacity: 0
                        visible: opacity !== 0
                        spacing: Core.dp(12)
                        anchors.left: parent.left
                        anchors.right: parent.right
                        onVisibleChanged: {
                            changeOldPassword.text = ""
                            changeNewPassword.text = ""
                            changeRepeatPassword.text = ""
                        }

                        Behavior on opacity {NumberAnimation {duration: 200;} }

                        Welcome.LineEdit {
                            id: changeOldPassword
                            placeholderText: qsTr("OLD PASSWORD")
                            anchors.horizontalCenter: parent.horizontalCenter
                            password: true
                            KeyNavigation.tab: changeNewPassword
                            onSubmit: buttonTitleMA.click()
                        }
                        Welcome.LineEdit {
                            id: changeNewPassword
                            placeholderText: qsTr("NEW PASSWORD")
                            anchors.horizontalCenter: parent.horizontalCenter
                            password: true
                            KeyNavigation.tab: changeRepeatPassword
                            onSubmit: buttonTitleMA.click()
                        }
                        Welcome.LineEdit {
                            id: changeRepeatPassword
                            placeholderText: qsTr("REPEAT PASSWORD")
                            anchors.horizontalCenter: parent.horizontalCenter
                            password: true
                            KeyNavigation.tab: changeOldPassword
                            onSubmit: buttonTitleMA.click()
                        }
                    }
                }

                PropertyAnimation {
                    id: showErrorAmination
                    target: errorBox
                    property: "anchors.horizontalCenterOffset"
                    from: Core.dp(12)
                    to: 0
                    easing.type: Easing.OutElastic
                    easing.amplitude: Core.dp(2)
                    duration: 800
                }


                Item {
                    id: errorMessage
                    opacity: 0
                    visible: false
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: form.bottom
                    anchors.topMargin: Core.dp(12)
                    height: errorText.lineCount * Core.dp(16)
                    Behavior on opacity {NumberAnimation {duration: 200} }

                    onVisibleChanged: {
                        if (!visible) {
                            errorText.text = ""
                        }
                    }

                    Rectangle {
                        id: errorBox
                        anchors.horizontalCenter: parent.horizontalCenter
                        height: parent.height
                        border.color: "#FF0000"
                        color: "#FBEFEF"
                        width: Core.dp(125)

                        Text {
                            id: errorText
                            color: "#FF0000"
                            font.pixelSize: Core.dp(8)
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.margins: Core.dp(8)
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }

        Item {
            id: saveUser
            property bool passwordSaved: false
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Core.dp(10)
            Behavior on anchors.topMargin {NumberAnimation {duration: 200 } }
            height: Core.dp(12)

            Settings {
                property alias saveUserAndPassword: saveUser.passwordSaved
                property alias userName: signInLogin.text
                property alias userPassword: signInPassword.text
            }

            Text {
                id: saveUserText
                anchors.centerIn: parent
                color: "white"
                font.pixelSize: Core.dp(8)
                text: saveUser.passwordSaved?qsTr("Not save User & Password"):qsTr("Save User & Password")
            }
            Elements.TerminalMouseArea {
                id: saveUserMA
                anchors.fill: parent
                platformIndependentHoverEnabled: true
                onContainsMouseChanged:  {
                    if (containsMouse) {
                        saveUserText.font.pixelSize = Core.dp(10)
                    } else {
                        saveUserText.font.pixelSize = Core.dp(8)
                    }
                }
                onClicked: saveUser.passwordSaved = !saveUser.passwordSaved
            }
        }
    }
}
