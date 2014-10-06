#include "networkdevice.h"

NetworkDevice::NetworkDevice(QObject *parent) :
    Device(parent),
    m_ip(""),
    m_port("")
{
}

void NetworkDevice::reset()
{
    Device::reset();
    setIp("");
    setPort("");
}

QString NetworkDevice::tagName() const
{
    return "network_device";
}

QString NetworkDevice::ip() const
{
    return m_ip;
}

void NetworkDevice::setIp(const QString &ip)
{
    if (m_ip != ip) {
        m_ip = ip;
        emit ipChanged();
    }
}
QString NetworkDevice::port() const
{
    return m_port;
}

void NetworkDevice::setPort(const QString &port)
{
    if (m_port != port) {
        m_port = port;
        emit portChanged();
    }
}

bool NetworkDevice::loadFromDomElement(const QDomElement &domElement)
{
    if (Device::loadFromDomElement(domElement)) {
        QDomElement ipElement = domElement.firstChildElement("ip");
        if (!ipElement.isNull()) {
            setIp(ipElement.text());
        } else {
            setIp("");
        }

        QDomElement portElement = domElement.firstChildElement("port");
        if (!portElement.isNull()) {
            setPort(portElement.text());
        } else {
            setPort("");
        }

        return true;
    }
    return false;
}

QDomElement NetworkDevice::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = Device::toDomElement(domDocument);

    QDomElement ipElement = domDocument.createElement("ip");
    rootElement.appendChild(ipElement);
    QDomText ipText = domDocument.createTextNode(ip());
    ipElement.appendChild(ipText);

    QDomElement portElement = domDocument.createElement("port");
    rootElement.appendChild(portElement);
    QDomText portText = domDocument.createTextNode(port());
    portElement.appendChild(portText);

    return rootElement;
}
