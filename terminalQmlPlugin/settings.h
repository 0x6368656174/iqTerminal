#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QUrl>
#include <QTranslator>
#include "simpledevicesmodel.h"
#include "networkdevicesmodel.h"
#include "proxysettings.h"
#include "xmlinterface.h"

/*!
 * \brief Класс, описывающий настройки приложения
 *
 * Настройки из данного класса доступны пользователю через графический интерфейс QML.
 *
 * \ingroup AppModel
 */
class Settings : public QObject, public XmlInterface
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
     * \brief Корненвой элемент, в котором храняться XML-ноды, описывающие экземпляры элементов объекта.
     *
     * Строка содержащая названия нод XML-файла, разделенные косой чертой ("\\"). Например, если XML-файл имеет
     * структуру:
     * ~~~~~~~~~~~{.xml}
     *    <all>
     *       <settings>
     *          ....
     *       </settings>
     *    </all>
     * ~~~~~~~~~~~
     * То корневой элемент для объекта, содержего настройки, описанные в нодах `<settings></settings>`, является
     * `"all/settings"`.
     *
     * При установки нового корневого элемента, если задан правильный путь к XML-файлу (\ref source),
     * объект автоматически перезагрузится.
     *
     * \accessors %parentElement(), setParentElement(), parentElementChanged()
     */
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)
    /*!
     * \brief Перевод
     *
     * При изменении данного параметра, интерфейс автоматически переводится на указаный перевод.
     *
     * По умолчанию \a English.
     *
     * \accessors %translation(), setTranslation(), translationChanged()
     */
    Q_PROPERTY(Translations translation READ translation WRITE setTranslation NOTIFY translationChanged)
    /*!
     * \brief Домашняя дирректория приложения
     *
     * По умолчанию равен \ref Core::homeDir().
     *
     * \accessors %homeDirectory(), setHomeDirectory(), homeDirectoryChanged()
     */
    Q_PROPERTY(QUrl homeDirectory READ homeDirectory WRITE setHomeDirectory NOTIFY homeDirectoryChanged)
    /*!
     * \brief Указатель на модель, содержащую доступные аудио-устройства
     *
     * \accessors %audioDevicesModel()
     */
    Q_PROPERTY(SimpleDevicesModel* audioDevicesModel READ audioDevicesModel CONSTANT)
    /*!
     * \brief Указатель на модель, содержащую доступные видео-устройства
     *
     * \accessors %videoDevicesModel()
     */
    Q_PROPERTY(SimpleDevicesModel* videoDevicesModel READ videoDevicesModel CONSTANT)
    /*!
     * \brief Укзатель на модель, содержащую доступные WAN-интерфейсы
     *
     * \accessors %wanDevicesModel()
     */
    Q_PROPERTY(NetworkDevicesModel* wanDevicesModel READ wanDevicesModel CONSTANT)
    /*!
     * \brief Признак того, что NAT-PNP активирован
     *
     * По-умолчанию \a FALSE.
     *
     * \accessors %natEnabled(), setNatEnabled(), natEnabledChanged()
     */
    Q_PROPERTY(bool natEnabled READ natEnabled WRITE setNatEnabled NOTIFY natEnabledChanged)
    /*!
     * \brief Признак того, что UPnP активирован
     *
     * По-умолчанию \a FALSE.
     *
     * \accessors %upnpEnabled(), setUpnpEnabled(), upnpEnabledChanged()
     */
    Q_PROPERTY(bool upnpEnabled READ upnpEnabled WRITE setUpnpEnabled NOTIFY upnpEnabledChanged)
    /*!
     * \brief Указатель на настройки прокси
     *
     * \accessors %proxySettings()
     */
    Q_PROPERTY(ProxySettings* proxySettings READ proxySettings CONSTANT)
    /*!
     * \brief Коофициент маштабирования приложения
     *
     * При изменении данного параметра, интерфейс автоматически маштабируется в соответствии с ним.
     *
     * По умолчанию \a 1.0.
     *
     * \accessors %zoomFactor(), setZoomFactor(), zoomFactorChanged()
     */
    Q_PROPERTY(qreal zoomFactor READ zoomFactor WRITE setZoomFactor NOTIFY zoomFactorChanged)
    /*!
     * \brief Вспомогательное свойство для обеспечения динамического перевода QML
     *
     * При помощи данного свойства, обеспечивается динамический перевод QML. Каждая строка в QML, которая
     * должна быть переведена должна заканчиваться конкатенацией с данным свойством.
     *
     * ~~~~~~~~~~~{.qml}
     * Text {
     *    text: qsTr("Name") + applicationModel.settings.translatorStringEnd
     * }
     * ~~~~~~~~~~~
     *
     * \accessors emptyString(), translationChanged()
     */
    Q_PROPERTY(QString translatorStringEnd READ emptyString NOTIFY translationChanged)
    Q_ENUMS(Translations)
public:
    /*!
     * \brief Доступные переводы
     */
    enum Translations {
        English,    //!< Английский
        Chinese,    //!< Китайский
        Spanish,    //!< Испанский
        Japanese,   //!< Японский
        Portuguese, //!< Португальский
        German,     //!< Немецкий
        Arabic,     //!< Арабский
        French,     //!< Французкий
        Russian,    //!< Русский
        Korean      //!< Корейский
    };

    /*!
     * \brief Конструктор
     *
     * Создает экземпляр объекта с указанным родителем.
     *
     * \param parent Укзатель на родитель
     */
    explicit Settings(QObject *parent = 0);

    /*!
     * \brief Перезагружает объект
     *
     * Перезагружает данные в объект из XML-файла. Для того, чтоб можно было загрузить данные в объект, необходимо
     * правильно установить \ref source, \ref parentElement.
     *
     * \return \a TRUE, в случае удачи, иначе \a FALSE
     */
    Q_INVOKABLE bool reload();

    /*!
     * \brief Сохраняет данные объекта
     *
     * Сохраняет данные из объекта в XML-файл. Для того, чтоб можно было сохранить данные из объекта, необходимо
     * правильно установить \ref source, \ref parentElement.
     *
     * \return \a TRUE, в случае удачи, иначе \a FALSE
     */
    Q_INVOKABLE bool save() const;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
    QUrl source() const;
    void setSource(const QUrl &source);

    QString parentElement() const;
    void setParentElement(const QString &parentElement);

    Translations translation() const;
    void setTranslation(const Translations &translation);

    QUrl homeDirectory() const;
    void setHomeDirectory(const QUrl &homeDirectory);

    SimpleDevicesModel *audioDevicesModel() const;

    SimpleDevicesModel *videoDevicesModel() const;

    NetworkDevicesModel *wanDevicesModel() const;

    bool natEnabled() const;
    void setNatEnabled(bool natEnabled);

    bool upnpEnabled() const;
    void setUpnpEnabled(bool upnpEnabled);

    ProxySettings *proxySettings() const;

    qreal zoomFactor() const;
    void setZoomFactor(const qreal &zoomFactor);

    QString emptyString() const;

signals:
    void sourceChanged();
    void parentElementChanged();
    void translationChanged();
    void homeDirectoryChanged();
    void natEnabledChanged();
    void upnpEnabledChanged();
    void zoomFactorChanged();
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

private:
    QUrl m_source;
    QString m_parentElement;
    Translations m_translation;
    QUrl m_homeDirectory;
    SimpleDevicesModel *m_audioDevicesModel;
    SimpleDevicesModel *m_videoDevicesModel;
    NetworkDevicesModel *m_wanDevicesModel;
    bool m_natEnabled;
    bool m_upnpEnabled;
    ProxySettings *m_proxySettings;
    qreal m_zoomFactor;
    QTranslator *m_applicatonTranslator;
};

#endif // SETTINGS_H
