#include "proxysettings.h"
#include <QDebug>

ProxySettings::ProxySettings(QObject *parent) :
    AbstractXmlItemObject(parent),
    m_ip("127.0.0.1"),
    m_port(8888),
    m_user(""),
    m_password("")
{
}

QString ProxySettings::tagName() const
{
    return "proxy_settings";
}

void ProxySettings::reset()
{
    AbstractXmlItemObject::reset();
    setIp("127.0.0.1");
    setPort(8888);
    setUser("");
    setPassword("");
}

QString ProxySettings::ip() const
{
    return m_ip;
}

void ProxySettings::setIp(const QString &ip)
{
    if (m_ip != ip) {
        m_ip = ip;
        emit ipChanged();
    }
}

quint16 ProxySettings::port() const
{
    return m_port;
}

void ProxySettings::setPort(const quint16 &port)
{
    if (m_port != port) {
        m_port = port;
        emit portChanged();
    }
}

QString ProxySettings::user() const
{
    return m_user;
}

void ProxySettings::setUser(const QString &user)
{
    if (m_user != user) {
        m_user = user;
        emit userChanged();
    }
}

QString ProxySettings::password() const
{
    return m_password;
}

void ProxySettings::setPassword(const QString &password)
{
    if (m_password != password) {
        m_password = password;
        emit passwordChanged();
    }
}

bool ProxySettings::loadFromDomElement(const QDomElement &domElement)
{
    if (domElement.isNull()) {
        reset();
        return false;
    }

    if (domElement.tagName() != tagName()) {
        qWarning() << tr("Dom Element is not \"%0\"")
                      .arg(tagName());
        reset();
        return false;
    }


    QDomElement ipElement = domElement.firstChildElement("ip");
    if (!ipElement.isNull()) {
        setIp(ipElement.text());
    } else {
        setIp("127.0.0.1");
    }

    QDomElement portElement = domElement.firstChildElement("port");
    if (!portElement.isNull()) {
        setPort(portElement.text().toUShort());
    } else {
        setPort(8888);
    }

    QDomElement userElement = domElement.firstChildElement("user");
    if (!userElement.isNull()) {
        setUser(userElement.text());
    } else {
        setUser("");
    }

    QDomElement passwordElement = domElement.firstChildElement("password");
    if (!passwordElement.isNull()) {
        setPassword(passwordElement.text());
    } else {
        setPassword("");
    }

    return true;
}

QDomElement ProxySettings::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = domDocument.createElement(tagName());

    QDomElement ipElement = domDocument.createElement("ip");
    rootElement.appendChild(ipElement);
    QDomText ipText = domDocument.createTextNode(ip());
    ipElement.appendChild(ipText);

    QDomElement portElement = domDocument.createElement("port");
    rootElement.appendChild(portElement);
    QDomText portText = domDocument.createTextNode(QString::number(port()));
    portElement.appendChild(portText);

    QDomElement userElement = domDocument.createElement("user");
    rootElement.appendChild(userElement);
    QDomText userText = domDocument.createTextNode(user());
    userElement.appendChild(userText);

    QDomElement passwordElement = domDocument.createElement("password");
    rootElement.appendChild(passwordElement);
    QDomText passwordText = domDocument.createTextNode(password());
    passwordElement.appendChild(passwordText);

    return rootElement;
}
