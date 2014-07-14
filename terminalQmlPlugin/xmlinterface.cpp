#include "xmlinterface.h"
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QStringList>

QDomElement XmlInterface::findElement(const QObject *object, const QUrl &source, const QString &element, QDomDocument &domDocument) const
{
    if (!object)
    {
        qWarning() << "In XmlInterface::findElement() object can not be NULL.";
        return QDomElement();
    }

    QString className = QString(object->metaObject()->className());

    if (!source.isValid())
    {
//        qWarning() << QString("Find element \"%0\" in \"%1\" in %2 failed. Source must be set first.")
//                      .arg(element)
//                      .arg(source.toLocalFile())
//                      .arg(className);
        return QDomElement();
    }

    if (element.isEmpty())
    {
//        qWarning() << QString("Find element \"%0\" in \"%1\" in %2 failed. Element must be set first.")
//                      .arg(element)
//                      .arg(source.toLocalFile())
//                      .arg(className);
        return QDomElement();
    }

    QFile file (source.toLocalFile());
    if (!file.exists())
    {
        qWarning() << QString("Find element \"%0\" in \"%1\" in %2 failed. File \"%1\" not exists.")
                      .arg(element)
                      .arg(source.toLocalFile())
                      .arg(className);
        return QDomElement();
    }
    if (!file.open(QFile::ReadOnly))
    {
        qWarning() << QString("Find element \"%0\" in \"%1\" in %2 failed. Can not open file \"%1\".")
                      .arg(element)
                      .arg(source.toLocalFile())
                      .arg(className);
        return QDomElement();
    }
    QString error;
    if (!domDocument.setContent(&file, &error))
    {
        qWarning() << QString("Find element \"%0\" in \"%1\" in %2 failed. Parse file \"%1\" error. Error: \"%3\".")
                      .arg(element)
                      .arg(source.toLocalFile())
                      .arg(className)
                      .arg(error);
        return QDomElement();
    }

    QDomElement rootElement = domDocument.documentElement();
    QStringList domPath = element.split("/", QString::SkipEmptyParts);

    if (rootElement.tagName() != domPath.first())
    {
        qWarning() << QString("Find element \"%0\" in \"%1\" in %2 failed. In file \"%1\" not found \"%3\".")
                      .arg(element)
                      .arg(source.toLocalFile())
                      .arg(className)
                      .arg(domPath.first());
        return QDomElement();
    }
    domPath.removeFirst();

    foreach (QString domPathString, domPath)
    {
        rootElement = rootElement.firstChildElement(domPathString);
        if (rootElement.isNull())
        {
            qWarning() << QString("Find element \"%0\" in \"%1\" in %2 failed. In file \"%1\" not found \"%3\".")
                          .arg(element)
                          .arg(source.toLocalFile())
                          .arg(className)
                          .arg(domPathString);
            return QDomElement();
        }
    }

    return rootElement;
}

QDomElement XmlInterface::createElement(const QObject* object, const QUrl &source, const QString &element, QDomDocument &domDocument) const
{
    if (!object)
    {
        qWarning() << "In XmlInterface::createElement() object can not be NULL.";
        return QDomElement();
    }

    QString className = QString(object->metaObject()->className());

    if (!source.isValid())
    {
        qWarning() << QString("Create element \"%0\" in \"%1\" in %2 failed. Source must be set first.")
                      .arg(element)
                      .arg(source.toLocalFile())
                      .arg(className);
        return QDomElement();
    }

    if (element.isEmpty())
    {
        qWarning() << QString("Create element \"%0\" in \"%1\" in %2 failed. ParentElement must be set first.")
                      .arg(element)
                      .arg(source.toLocalFile())
                      .arg(className);
        return QDomElement();
    }

    QDomElement rootElement;
    QFile file (source.toLocalFile());
    if (file.exists())
    {
        if (!file.open(QFile::ReadOnly))
        {
            qWarning() << QString("Create element \"%0\" in \"%1\" in %2 failed. Can not open file \"%0\".")
                          .arg(element)
                          .arg(source.toLocalFile())
                          .arg(className);
            return QDomElement();
        }
        QString error;
        if (!domDocument.setContent(&file, &error))
        {
            qWarning() << QString("Create element \"%0\" in \"%1\" in %2 failed. Parse file \"%0\" error. Error: \"%3\".")
                          .arg(element)
                          .arg(source.toLocalFile())
                          .arg(className)
                          .arg(error);
            return QDomElement();
        }
        file.close();

        rootElement = domDocument.documentElement();
        QStringList domPath = element.split("/", QString::SkipEmptyParts);

        if (rootElement.tagName() != domPath.first())
        {
            qWarning() << QString("Create element \"%0\" in \"%1\" in %2 failed. In file \"%0\" not found \"%3\".")
                          .arg(element)
                          .arg(source.toLocalFile())
                          .arg(className)
                          .arg(domPath.first());
            return QDomElement();
        }
        domPath.removeFirst();

        foreach (QString domPathString, domPath)
        {
            QDomElement parentElement = rootElement;
            rootElement = parentElement.firstChildElement(domPathString);
            if (rootElement.isNull())
            {
                rootElement = domDocument.createElement(domPathString);
                parentElement.appendChild(rootElement);
            }
        }
    }
    else
    {
        QStringList domPath = element.split("/", QString::SkipEmptyParts);

        rootElement = domDocument.createElement(domPath.first());
        domDocument.appendChild(rootElement);

        foreach (QString domPathString, domPath)
        {
            QDomElement parentElement = rootElement;
            rootElement = domDocument.createElement(domPathString);
            parentElement.appendChild(rootElement);
        }
    }

    return rootElement;
}

bool XmlInterface::reloadModel(AbstractXmlItemsModel *model, const QUrl &source, const QString &element)
{
    if (!model)
    {
        qWarning() << "In XmlInterface::reoladModel() model can not be NULL.";
        return false;
    }

    QDomDocument domDoc;
    QDomElement rootElement = findElement(model, source, element, domDoc);
    if (!rootElement.isNull())
    {
        return model->loadFromDomElement(rootElement);
    }

    return false;
}

bool XmlInterface::saveModel(AbstractXmlItemsModel *model, const QUrl &source, const QString &element)
{
    if (!model)
    {
        qWarning() << "In XmlInterface::saveModel() model can not be NULL.";
        return false;
    }

    QDomDocument domDoc;
    QDomElement rootElement = createElement(model, source, element, domDoc);
    if (!rootElement.isNull())
    {
        //Пересоздадим основной элемент
        QDomElement oldRootElement = rootElement;
        rootElement = domDoc.createElement(rootElement.tagName());
        oldRootElement.parentNode().appendChild(rootElement);
        oldRootElement.parentNode().removeChild(oldRootElement);

        model->appendItemsToDomElement(rootElement, domDoc);

        QFile file (source.toLocalFile());
        if(file.open(QFile::WriteOnly))
        {
            QTextStream ts(&file);
            ts << domDoc.toString();
            file.close();

            return true;
        }
    }

    return false;
}
