#ifndef USERSMODEL_H
#define USERSMODEL_H

#include "filexmlitemsmodel.h"
#include "user.h"

/*!
 * \brief Модель, содержащая список пользователей
 *
 * Данная модель содержит список пользователей. Каждый пользователь в модедели - это объект типа \ref User.
 *
 * \ingroup AppModel
 */
class UsersModel : public FileXmlItemsModel
{
    Q_OBJECT
public:
    /*!
     * \brief Роли модели
     */
    enum Roles
    {
        Id = Qt::UserRole,  //!< Уникальный идентификатор
        AdditionalData,     //!< Дополнительные данные, используемые QML
        Profile,            //!< Профиль пользователя (\ref User::profile)
        Online,             //!< Пользователь онлайт
        FriendshipAccepted, //!< Пользователь принял предложение дружбы
        UserInfo            //!< Указатель на информацию о пользователе
    };

    /*!
     * \brief Конструктор
     *
     * Создает экземпляр объекта с указанным родителем.
     *
     * \param parent Укзатель на родитель
     */
    explicit UsersModel(QObject *parent = 0);

#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

private:
    QHash<int, QByteArray> m_roles;
    QUrl m_source;
    QString m_parentElement;
};

#endif // USERSMODEL_H
