#ifndef XMLINTERFACE_H
#define XMLINTERFACE_H

#include <QDomElement>
#include <QDomDocument>
#include <QUrl>
#include "abstractxmlitemsmodel.h"

/*!
 * \brief Класс, описывающий методы расширяющие классы для работы с XML-фалами
 *
 * Данный класс описывает методы, расщиряющие функционал классов для работы с XML-файлами. Это методы для работы
 * с XML-элементами (\ref findElement(), \ref createElement()), а так же методы, позволяющие \ref AbstractXmlItemsModel
 * загружать свои данные из XML-файлов (\ref reloadModel()) и сохранять свои данные в XML-файл (\ref saveModel()).
 */
class XmlInterface
{
protected:
    static QDomElement findElement(const QObject* object, const QUrl& source, const QString &element, QDomDocument &domDocument);

    static QDomElement createElement(const QObject* object, const QUrl& source, const QString &element, QDomDocument &domDocument);

    static bool reloadModel(AbstractXmlItemsModel* model, const QUrl& source, const QString &element);

    static bool saveModel(const AbstractXmlItemsModel *model, const QUrl& source, const QString &element);
};

#endif // XMLINTERFACE_H
