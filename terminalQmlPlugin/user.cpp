#include "user.h"

User::User(QObject *parent) :
    AbstractXmlItemObject(parent),
    m_profile(""),
    m_online(false),
    m_friendshipAccepted(false),
    m_userInfo(new UserInfo(this))
{
}

void User::reset()
{
    AbstractXmlItemObject::reset();
    setProfile("");
    setOnline(false);
    setFriendshipAccepted(false);
}

QString User::tagName() const
{
    return "user";
}

void User::setProfile(const QString &profileFile)
{
    if (m_profile != profileFile) {
        m_profile = profileFile;

        m_userInfo->setUserProfileFile(profileFile);

        emit profileChanged();
    }
}
bool User::online() const
{
    return m_online;
}

void User::setOnline(bool online)
{
    if (m_online != online) {
        m_online = online;
        emit onlineChanged();
    }
}
bool User::friendshipAccepted() const
{
    return m_friendshipAccepted;
}

void User::setFriendshipAccepted(bool friendshipAccepted)
{
    if (m_friendshipAccepted != friendshipAccepted) {
        m_friendshipAccepted = friendshipAccepted;
        emit friendshipAcceptedChanged();
    }
}

UserInfo *User::userInfo() const
{
    return m_userInfo;
}

bool User::loadFromDomElement(const QDomElement &domElement)
{
    if (AbstractXmlItemObject::loadFromDomElement(domElement)) {
        QDomElement profileElement = domElement.firstChildElement("profile");
        if (!profileElement.isNull())
            setProfile(profileElement.text());
        else
            setProfile("");

        QDomElement onlineElement = domElement.firstChildElement("online");
        if (!onlineElement.isNull())
            setOnline(onlineElement.text().compare("true", Qt::CaseInsensitive) == 0?true:false);
        else
            setOnline(false);

        QDomElement friendshipAcceptedElement = domElement.firstChildElement("friendship_accepted");
        if (!friendshipAcceptedElement.isNull())
            setFriendshipAccepted(friendshipAcceptedElement.text().compare("true", Qt::CaseInsensitive) == 0?true:false);
        else
            setFriendshipAccepted(false);

        return true;
    }
    return false;
}

QDomElement User::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = AbstractXmlItemObject::toDomElement(domDocument);

    QDomElement profileElement = domDocument.createElement("profile");
    rootElement.appendChild(profileElement);
    QDomText profileText = domDocument.createTextNode(profile());
    profileElement.appendChild(profileText);

    QDomElement onlineElement = domDocument.createElement("online");
    rootElement.appendChild(onlineElement);
    QDomText onlineText = domDocument.createTextNode(online()?"true":"false");
    onlineElement.appendChild(onlineText);

    QDomElement friednshipAcceptedElement = domDocument.createElement("friendship_accepted");
    rootElement.appendChild(friednshipAcceptedElement);
    QDomText friendshipAcceptedText = domDocument.createTextNode(friendshipAccepted()?"true":"false");
    friednshipAcceptedElement.appendChild(friendshipAcceptedText);

    return rootElement;
}
