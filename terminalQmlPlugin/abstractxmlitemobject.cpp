#include "abstractxmlitemobject.h"
#include <QDebug>

AbstractXmlItemObject::AbstractXmlItemObject(QObject *parent) :
    QObject(parent),
    _id(-1),
    _additionalData(NULL)
{
}

void AbstractXmlItemObject::setId(const qint64 id)
{
    if (_id != id)
    {
        _id = id;

        emit idChanged();
    }
}

void AbstractXmlItemObject::setAdditionalData(QObject *additionalData)
{
    if (_additionalData != additionalData)
    {
        _additionalData = additionalData;
        if(_additionalData)
            _additionalData->setParent(this);

        emit additionalDataChanged();
    }
}

void AbstractXmlItemObject::reset()
{
    //НЕ СБРАСЫВАЙ id!!!
}

bool AbstractXmlItemObject::loadFromDomElement(const QDomElement &domElement)
{
    if (domElement.isNull())
    {
        reset();
        return false;
    }

    if (domElement.tagName() != tagName())
    {
        qWarning() << QString("Dom Element is not \"%0\"")
                      .arg(tagName());
        reset();
        return false;
    }

    if (domElement.hasAttribute("id"))
    {
        setId(domElement.attribute("id").toLongLong());
    }
    else
    {
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
