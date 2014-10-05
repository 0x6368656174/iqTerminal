import QtQuick 2.0
import TerminalQmlPlugin 1.0
import "../userInfo"

UserInfoPage {
    id:iPage
    name: "i"
//    userProfile: "../i.xml"
    userInfo: applicationModel.aboutMe
}
