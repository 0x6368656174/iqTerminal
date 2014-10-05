#include "userstate.h"
#include <QDebug>

UserState::UserState(QObject *parent) :
    AbstractXmlItemObject(parent),
    m_name(tr("New State")),
    m_text("")
{
}

void UserState::reset()
{
    AbstractXmlItemObject::reset();
    setName("");
    setText("");
}

QString UserState::tagName() const
{
    return "state";
}

QString UserState::name() const
{
    return m_name;
}

void UserState::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;

        emit nameChanged();
    }
}

QString UserState::text() const
{
    return m_text;
}

void UserState::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;

        emit textChanged();
    }
}

bool UserState::loadFromDomElement(const QDomElement &domElement)
{
    if (AbstractXmlItemObject::loadFromDomElement(domElement)) {
        QDomElement nameElement = domElement.firstChildElement("name");
        if (!nameElement.isNull()) {
            setName(nameElement.text());
        } else {
            setName("");
        }

        QDomElement textElement = domElement.firstChildElement("text");
        if (!textElement.isNull()) {
            setText(textElement.text());
        } else {
            setText("");
        }

        return true;
    }
    return false;
}

QDomElement UserState::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = AbstractXmlItemObject::toDomElement(domDocument);

    QDomElement nameElement = domDocument.createElement("name");
    rootElement.appendChild(nameElement);
    QDomText nameText = domDocument.createTextNode(name());
    nameElement.appendChild(nameText);

    QDomElement textElement = domDocument.createElement("text");
    rootElement.appendChild(textElement);
    QDomText textText = domDocument.createTextNode(text());
    textElement.appendChild(textText);

    return rootElement;
}
