#include "simpledevice.h"

SimpleDevice::SimpleDevice(QObject *parent) :
    Device(parent),
    m_name("")
{
}

void SimpleDevice::reset()
{
    Device::reset();
    setName("");
}

QString SimpleDevice::tagName() const
{
    return "device";
}

QString SimpleDevice::name() const
{
    return m_name;
}

void SimpleDevice::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

bool SimpleDevice::loadFromDomElement(const QDomElement &domElement)
{
    if (Device::loadFromDomElement(domElement)) {
        QDomElement nameElement = domElement.firstChildElement("name");
        if (!nameElement.isNull()) {
            setName(nameElement.text());
        } else {
            setName("");
        }

        return true;
    }
    return false;
}

QDomElement SimpleDevice::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = Device::toDomElement(domDocument);

    QDomElement nameElement = domDocument.createElement("name");
    rootElement.appendChild(nameElement);
    QDomText nameText = domDocument.createTextNode(name());
    nameElement.appendChild(nameText);

    return rootElement;
}
