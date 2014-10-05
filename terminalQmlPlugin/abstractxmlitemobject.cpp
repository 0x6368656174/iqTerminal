#include "abstractxmlitemobject.h"
#include <QDebug>

AbstractXmlItemObject::AbstractXmlItemObject(QObject *parent) :
    QObject(parent),
    m_id(-1),
    m_additionalData(nullptr)
{
}

qint64 AbstractXmlItemObject::id() const
{
    return m_id;
}

void AbstractXmlItemObject::setId(const qint64 id)
{
    if (m_id != id) {
        m_id = id;

        emit idChanged();
    }
}

QObject *AbstractXmlItemObject::additionalData() const
{
    return m_additionalData;
}

void AbstractXmlItemObject::setAdditionalData(QObject *additionalData)
{
    if (m_additionalData != additionalData) {
        if (m_additionalData)
            m_additionalData->deleteLater();

        if(m_additionalData)
            m_additionalData->setParent(this);
        m_additionalData = additionalData;

        emit additionalDataChanged();
    }
}

void AbstractXmlItemObject::reset()
{
    //НЕ СБРАСЫВАЙ id!!!
}

bool AbstractXmlItemObject::loadFromDomElement(const QDomElement &domElement)
{
    if (domElement.isNull()) {
        reset();
        return false;
    }

    if (domElement.tagName() != tagName()) {
        qWarning() << QString("Dom Element is not \"%0\"")
                   .arg(tagName());
        reset();
        return false;
    }

    if (domElement.hasAttribute("id")) {
        setId(domElement.attribute("id").toLongLong());
    } else {
        qWarning() << QString("In parsing \"%0\" element found element without id, skipped...")
                   .arg(tagName());
        reset();
        return false;
    }

    return true;
}

QDomElement AbstractXmlItemObject::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = domDocument.createElement(tagName());
    rootElement.setAttribute("id", id());

    return rootElement;
}
