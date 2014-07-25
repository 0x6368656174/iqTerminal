#ifndef XMLINTERFACE_H
#define XMLINTERFACE_H

#include <QDomElement>
#include <QDomDocument>
#include <QUrl>
#include "abstractxmlitemsmodel.h"

class XmlInterface
{
protected:
    static QDomElement findElement(const QObject* object, const QUrl& source, const QString &element, QDomDocument &domDocument);

    static QDomElement createElement(const QObject* object, const QUrl& source, const QString &element, QDomDocument &domDocument);

    static bool reloadModel(AbstractXmlItemsModel* model, const QUrl& source, const QString &element);

    static bool saveModel(const AbstractXmlItemsModel *model, const QUrl& source, const QString &element);
};

#endif // XMLINTERFACE_H
