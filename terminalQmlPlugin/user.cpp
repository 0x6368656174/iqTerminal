#include "user.h"

User::User(QObject *parent) :
    AbstractXmlItemObject(parent),
    _profile("")
{
}

void User::reset()
{
    AbstractXmlItemObject::reset();
    setProfile("");
}

QString User::tagName() const
{
    return "user";
}

void User::setProfile(const QString &profileFile)
{
    if (_profile != profileFile)
    {
        _profile = profileFile;

        emit profileChanged();
    }
}

bool User::loadFromDomElement(const QDomElement &domElement)
{
    if (AbstractXmlItemObject::loadFromDomElement(domElement))
    {
        QDomElement profileElement = domElement.firstChildElement("profile");
        if (!profileElement.isNull())
        {
            setProfile(profileElement.text());
        }
        else
        {
            setProfile("");
        }

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

    return rootElement;
}
