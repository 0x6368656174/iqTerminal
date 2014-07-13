#ifndef XMLINTERFACE_H
#define XMLINTERFACE_H

#include <QDomElement>
#include <QDomDocument>
#include <QUrl>

class XmlInterface
{
protected:
    QDomElement findElement(const QObject* object, const QUrl& source, const QString &element, QDomDocument &domDocument) const;

    QDomElement createElement(const QObject* object, const QUrl& source, const QString &element, QDomDocument &domDocument) const;
};

#endif // XMLINTERFACE_H
