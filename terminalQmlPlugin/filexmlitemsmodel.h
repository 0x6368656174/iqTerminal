#ifndef FILEXMLITEMSMODEL_H
#define FILEXMLITEMSMODEL_H

#include "abstractxmlitemsmodel.h"
#include "xmlinterface.h"

/*!
 * \brief Модель позволяющая хранить данные в XML-файлах
 *
 * Данная модель реализует методы и функции, позволяющие хранить данные модели в XML-файлах. Для работы с
 * XML-файлами необходимо установить \ref source и \ref parentElement. После чего можно загружать модель
 * из XML-фала, используя \ref reload(), а так же сохранять данные модели в XML-файл, используя \ref save().
 */
class FileXmlItemsModel : public AbstractXmlItemsModel, public XmlInterface
{
    Q_OBJECT
    /*!
     * \brief XML-файл, в котором храняться данные модели
     *
     * Модель автоматически читает данные из указанного XML-файла (если до этого указан правильный
     * \ref parentElement), а так же сохраняет данные в указанный XML-файл при вызове метода \ref save().
     *
     * \accessors %source(), setSource(), sourceChanged()
     */
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    /*!
     * \brief Корненвой элемент, в котором храняться XML-ноды, описывающие экземпляры элементов модели.
     *
     * Строка содержащая названия нод XML-файла, разделенные косой чертой ("\\"). Например, если XML-файл имеет
     * структуру:
     * ~~~~~~~~~~~{.xml}
     *    <all>
     *       <contacts>
     *          <user></user>
     *          ....
     *          <user></user>
     *       </contacts>
     *    </all>
     * ~~~~~~~~~~~
     * То корневой элемент для модели, содержей пользователей, описанные в нодах `<user></user>`, является
     * `"all/contacts"`.
     *
     * При установки нового корневого элемента, если задан правильный путь к XML-файлу (\ref source),
     * модель автоматически перезагрузится.
     *
     * \accessors %parentElement(), setParentElement(), parentElementChanged()
     */
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)

public:
    explicit FileXmlItemsModel(QObject *parent = 0);

    /*!
     * \brief Перезагружает модель
     *
     * Перезагружает данные в модель из XML-файла. Для того, чтоб можно было загрузить данные в модель, необходимо
     * правильно установить \ref source, \ref parentElement.
     *
     * \return \a TRUE, в случае удачи, иначе \a FALSE
     */
    Q_INVOKABLE bool reload();

    /*!
     * \brief Сохраняет данные модели
     *
     * Сохраняет данные из модели в XML-файл. Для того, чтоб можно было сохранить данные из модели, необходимо
     * правильно установить \ref source, \ref parentElement.
     *
     * \return \a TRUE, в случае удачи, иначе \a FALSE
     */
    Q_INVOKABLE bool save();

#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
    QUrl source() const;
    void setSource(const QUrl &source);

    QString parentElement() const;
    void setParentElement(const QString &parentElement);

signals:
    void sourceChanged();
    void parentElementChanged();
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

private:
    QUrl m_source;
    QString m_parentElement;
};

#endif // FILEXMLITEMSMODEL_H
