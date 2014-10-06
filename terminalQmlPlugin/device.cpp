#include "device.h"

Device::Device(QObject *parent) :
    AbstractXmlItemObject(parent),
    m_active(false)
{
}

void Device::reset()
{
    AbstractXmlItemObject::reset();
    setActive(false);
}

bool Device::active() const
{
    return m_active;
}

void Device::setActive(bool active)
{
    if (m_active != active) {
        m_active = active;
        emit activeChanged();
    }
}

bool Device::loadFromDomElement(const QDomElement &domElement)
{
    if (AbstractXmlItemObject::loadFromDomElement(domElement)) {
        QDomElement activeElement = domElement.firstChildElement("active");
        if (!activeElement.isNull()) {
            setActive(activeElement.text().compare("true", Qt::CaseInsensitive) == 0?true:false);
        } else {
            setActive(false);
        }

        return true;
    }
    return false;
}

QDomElement Device::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = AbstractXmlItemObject::toDomElement(domDocument);

    QDomElement activeElement = domDocument.createElement("active");
    rootElement.appendChild(activeElement);
    QDomText activeText = domDocument.createTextNode(active()?"true":"false");
    activeElement.appendChild(activeText);

    return rootElement;
}
