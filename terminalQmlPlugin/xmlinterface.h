#ifndef XMLINTERFACE_H
#define XMLINTERFACE_H

#include <QDomElement>
#include <QDomDocument>
#include <QUrl>
#include "abstractxmlitemsmodel.h"

class XmlInterface
{
protected:
    QDomElement findElement(const QObject* object, const QUrl& source, const QString &element, QDomDocument &domDocument) const;

    QDomElement createElement(const QObject* object, const QUrl& source, const QString &element, QDomDocument &domDocument) const;

    bool reloadModel(AbstractXmlItemsModel* model, const QUrl& source, const QString &element);

    bool saveModel(const AbstractXmlItemsModel *model, const QUrl& source, const QString &element) const;
};

#endif // XMLINTERFACE_H
