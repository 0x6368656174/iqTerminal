#include "abstractxmlitemobject.h"

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
        _additionalData->setParent(this);

        emit additionalDataChanged();
    }
}
