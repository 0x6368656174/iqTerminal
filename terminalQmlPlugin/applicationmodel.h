#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>
#include "userinfo.h"
#include "usersmodel.h"
#include "torrents.h"

/*!
 * \defgroup AppModel Основная модель
 *
 * \brief Основная модель приложения
 *
 * В этой группе содержаться классы, описывающие работу основной модели приложения. Основной класс из этой
 * группы - это \ref ApplicationModel. Именно этот класс необходимо использовать в С++ для взаимодействия с
 * QML-интерфейсом.
 *
 * \ingroup TerminalQmlPlugin
 */

/*!
 * \brief Основаная модель приложения
 *
 * Данный класс описывает основную модель приложения. Класс является синглтоном, поэтому, чтоб получить
 * доступ к экземпляру объекта класса необходимо использовать метод \ref instance().
 *
 * \ingroup AppModel
 */
class ApplicationModel : public QObject
{
    Q_OBJECT
    /*!
     * \brief Указатель на информацию о себе
     *
     * \accessors %aboutMe()
     */
    Q_PROPERTY(UserInfo* aboutMe READ aboutMe CONSTANT)
    /*!
     * \brief Указатель на модель, содержащую всех пользователей
     *
     * \accessors %allModel()
     */
    Q_PROPERTY(UsersModel* allModel READ allModel CONSTANT)
    /*!
     * \brief Указатель на модель, содержащую список контактов
     *
     * \accessors %contactsModel()
     */
    Q_PROPERTY(UsersModel* contactsModel READ contactsModel CONSTANT)
    /*!
     * \brief Указатель на модель, содержащую список посетителей
     *
     * \accessors %visitorsModel()
     */
    Q_PROPERTY(UsersModel* visitorsModel READ visitorsModel CONSTANT)
    /*!
     * \brief Указатель на торренты
     *
     * \accessors %torrents()
     */
    Q_PROPERTY(Torrents* torrents READ torrents CONSTANT)

public:
    /*!
     * \brief Метод для получения экземпляра объекта
     *
     * Основная модель приложения - синглтон, поэтому для того, чтобы получить доступ к ее екземпляру,
     * необходимо использовать данный метод.
     *
     * \return Указатель на экземпляр объекта
     */
    static ApplicationModel *instance();

#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
    UserInfo *aboutMe() const;

    UsersModel *allModel() const;

    UsersModel *contactsModel() const;

    UsersModel *visitorsModel() const;

    Torrents *torrents() const;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

private:
    explicit ApplicationModel(QObject *parent = 0);
    static ApplicationModel *m_instance;

    UserInfo *m_aboutMe;
    UsersModel *m_allModel;
    UsersModel *m_contactsModel;
    UsersModel *m_visitorsModel;
    Torrents *m_torrents;
};

#endif // APPLICATIONMODEL_H
