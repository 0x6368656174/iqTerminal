import QtQuick 2.0
import TerminalQmlPlugin 1.0

import ".."

Page {
    id: welcomePage

    signal signInCompleted()

    function signIn(login, password) {
//        return "Bad login or password"
        welcomePage.signInCompleted()
        return ""
    }

    function register (login, email, name) {

    }

    function remindPassword(email) {

    }

    function changePassword(oldPassword, newPassword) {

    }

    name: "welcome"
    state: "start"

    Rectangle {
        anchors.fill: parent
        color: "#da4504"
    }

    states: [
        State {
            name: "start"
        },
        State {
            name: "signInPage"
            StateChangeScript {
                script: {
                    hideStartRows.start()
                }
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
            StateChangeScript {
                script: {
                    hideStartRows.start()
                }
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
            StateChangeScript {
                script: {
                    hideStartRows.start()
                }
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
            StateChangeScript {
                script: {
                    hideStartRows.start()
                }
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

    SequentialAnimation {
        id: hideStartRows
        ScriptAction {
            script: {
                signInRow.state = "hiden"
                registerRow.state = "hiden"
                remindRow.state = "hiden"
                changeRow.state = "hiden"
            }
        }
        PauseAnimation { duration: 200 }
        ScriptAction {
            script: {
                startView.visible = false
                buttonsRow.visible = true
                pages.visible = true
                signInButton.hide = false
//                signInButton.widthAnimationEnabled = false
                registerButton.hide = false
//                registerButton.widthAnimationEnabled = false
                remindButton.hide = false
//                remindButton.widthAnimationEnabled = false
                changeButton.hide = false
//                changeButton.widthAnimationEnabled = false
            }
        }
    }

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

        Flickable {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: terminalLogo.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: Core.dp(11)
            contentWidth: parent.width
            contentHeight: Core.dp(200)
            interactive: contentHeight > height
            clip: true

            Column {
                id: startView
                anchors.top: parent.top
                anchors.topMargin: Core.dp(4)
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: Core.dp(150)
                spacing: Core.dp(4)
                StartViewRow {
                    id: signInRow
                    text: qsTr("Sign In")
                    iconSource: "../../images/5.png"
                    onClicked: welcomePage.state = "signInPage"
                }
                StartViewRow {
                    id: registerRow
                    text: qsTr("Register")
                    iconSource: "../../images/6.png"
                    onClicked: welcomePage.state = "registerPage"
                }
                StartViewRow {
                    id: remindRow
                    text: qsTr("Remind Password")
                    iconSource: "../../images/7.png"
                    onClicked: welcomePage.state = "remindPage"
                }
                StartViewRow {
                    id: changeRow
                    text: qsTr("Change Password")
                    iconSource: "../../images/8.png"
                    onClicked: welcomePage.state = "changePage"
                }
            }

            Row {
                id: buttonsRow
                visible: false
                anchors.top: parent.top
                anchors.topMargin: Core.dp(4)
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: Core.dp(4)

                Item {
                    width: Core.dp(28)
                    height: width
                    Button {
                        id: signInButton
                        anchors.centerIn: parent
                        source: "../../images/5.png"
                        hide: true
                        onClicked: welcomePage.state = "signInPage"
                    }
                }
                Item {
                    width: Core.dp(28)
                    height: width
                    Button {
                        id: registerButton
                        anchors.centerIn: parent
                        source: "../../images/6.png"
                        hide: true
                        onClicked: welcomePage.state = "registerPage"
                    }

                }Item {
                    width: Core.dp(28)
                    height: width
                    Button {
                        id: remindButton
                        anchors.centerIn: parent
                        source: "../../images/7.png"
                        hide: true
                        onClicked: welcomePage.state = "remindPage"
                    }
                }Item {
                    width: Core.dp(28)
                    height: width
                    Button {
                        id: changeButton
                        anchors.centerIn: parent
                        source: "../../images/8.png"
                        hide: true
                        onClicked: welcomePage.state = "changePage"
                    }
                }
            }

            Item {
                id: pages
                visible: false
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: buttonsRow.bottom
                anchors.bottom: parent.bottom
                anchors.topMargin: Core.dp(15)

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

                        Behavior on opacity {NumberAnimation {duration: 200;} }

                        LineEdit {
                            id: signInLogin
                            placeholderText: qsTr("LOGIN")
                            anchors.horizontalCenter: parent.horizontalCenter
                            KeyNavigation.tab: signInPassword
                            onSubmit: submitButton.click()
                        }
                        LineEdit {
                            id: signInPassword
                            placeholderText: qsTr("PASSWORD")
                            anchors.horizontalCenter: parent.horizontalCenter
                            password: true
                            KeyNavigation.tab: signInLogin
                            onSubmit: submitButton.click()
                        }
                    }

                    Column {
                        id: registerPage
                        opacity: 0
                        visible: opacity !== 0
                        spacing: Core.dp(12)
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Behavior on opacity {NumberAnimation {duration: 200;} }

                        LineEdit {
                            id: registerLogin
                            placeholderText: qsTr("LOGIN")
                            anchors.horizontalCenter: parent.horizontalCenter
                            KeyNavigation.tab: registerEmail
                            onSubmit: submitButton.click()
                        }
                        LineEdit {
                            id: registerEmail
                            placeholderText: qsTr("EMAIL")
                            anchors.horizontalCenter: parent.horizontalCenter
                            KeyNavigation.tab: registerName
                            onSubmit: submitButton.click()
                        }
                        LineEdit {
                            id: registerName
                            placeholderText: qsTr("YOUR NAME")
                            anchors.horizontalCenter: parent.horizontalCenter
                            KeyNavigation.tab: registerLogin
                            onSubmit: submitButton.click()
                        }
                    }

                    Column {
                        id: remindPage
                        opacity: 0
                        visible: opacity !== 0
                        spacing: Core.dp(12)
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Behavior on opacity {NumberAnimation {duration: 200;} }

                        LineEdit {
                            id: remindEmal
                            placeholderText: qsTr("EMAIL")
                            anchors.horizontalCenter: parent.horizontalCenter
                            onSubmit: submitButton.click()
                        }
                    }

                    Column {
                        id: changePage
                        opacity: 0
                        visible: opacity !== 0
                        spacing: Core.dp(12)
                        anchors.left: parent.left
                        anchors.right: parent.right

                        Behavior on opacity {NumberAnimation {duration: 200;} }

                        LineEdit {
                            id: changeOldPassword
                            placeholderText: qsTr("OLD PASSWORD")
                            anchors.horizontalCenter: parent.horizontalCenter
                            password: true
                            KeyNavigation.tab: changeNewPassword
                            onSubmit: submitButton.click()
                        }
                        LineEdit {
                            id: changeNewPassword
                            placeholderText: qsTr("NEW PASSWORD")
                            anchors.horizontalCenter: parent.horizontalCenter
                            password: true
                            KeyNavigation.tab: changeRepeatPassword
                            onSubmit: submitButton.click()
                        }
                        LineEdit {
                            id: changeRepeatPassword
                            placeholderText: qsTr("REPEAT PASSWORD")
                            anchors.horizontalCenter: parent.horizontalCenter
                            password: true
                            KeyNavigation.tab: changeOldPassword
                            onSubmit: submitButton.click()
                        }
                    }
                }

                function showError (error) {
                    errorText.text = error
                    errorMessage.opacity = 1
                    errorMessage.visible = true
                    showErrorAmination.restart()
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
                    Behavior on opacity {NumberAnimation {duration: 200} }

                    Rectangle {
                        id: errorBox
                        anchors.horizontalCenter: parent.horizontalCenter
                        border.color: "#FF0000"
                        color: "#FBEFEF"
                        width: Core.dp(125)
                        height: errorText.lineCount * Core.dp(16)

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

                Item {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.horizontalCenterOffset: Core.dp(66)
                    anchors.topMargin: Core.dp(130)

                    Button {
                        id: submitButton
                        anchors.centerIn: parent
                        baseWidth: Core.dp(18)
                        source: "../../images/10.png"

                        function processClicked() {
                            if (welcomePage.state === "signInPage") {
                                var signInResult = welcomePage.signIn(signInLogin.text, signInPassword.text)
                                if (signInResult !== "") {
                                    pages.showError(signInResult)
                                }
                            } else if (welcomePage.state === "registerPage") {
                                welcomePage.register(registerLogin.text, registerEmail.text, registerName.text);
                            } else if (welcomePage.state === "remingPage") {
                                welcomePage.remindPassword(remindEmal.text);
                            } else if (welcomePage.state === "changePage") {
                                if (changeNewPassword.text !== changeRepeatPassword.text) {
                                    pages.showError(qsTr("Passwords do not match"))
                                } else {
                                    welcomePage.changePassword(changeOldPassword.text, changeNewPassword.text)
                                }
                            }
                        }

                        onClicked: processClicked()
                    }
                }
            }
        }
    }
}
