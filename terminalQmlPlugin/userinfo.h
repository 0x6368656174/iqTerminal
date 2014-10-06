#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include "userprofile.h"
#include "foldersmodel.h"

/*!
 * \brief Класс описывающий информацию о пользователе
 *
 * Данный класс содержит модели и данные, используемые для отображения странички информации о пользователе.
 *
 * \ingroup AppModel
 */
class UserInfo : public QObject
{
    Q_OBJECT
    /*!
     * \brief Имя файла пользователя
     *
     * Информация о пользователе грузится из XML-файла, указаного в данном параметре. При изменении имени
     * файла о пользователе, информация автоматически перезагрузится.
     *
     * \accessors %userProfileFile(), setUserProfileFile(), userProfileFileChanged()
     */
    Q_PROPERTY(QString userProfileFile READ userProfileFile WRITE setUserProfileFile NOTIFY userProfileFileChanged)
    /*!
     * \brief Указатель на профиль пользователя
     *
     * \accessors %userProfile()
     */
    Q_PROPERTY(UserProfile* userProfile READ userProfile CONSTANT)
    /*!
     * \brief Указатель на модель фотографий пользователя
     *
     * \accessors %photosModel()
     */
    Q_PROPERTY(FoldersModel* photosModel READ photosModel CONSTANT)
    /*!
     * \brief Указатель на модель видео пользователя
     *
     * \accessors %videosModel()
     */
    Q_PROPERTY(FoldersModel* videosModel READ videosModel CONSTANT)
    /*!
     * \brief Указатель на модель музыки пользователя
     *
     * \accessors %musicsModel()
     */
    Q_PROPERTY(FoldersModel* musicsModel READ musicsModel CONSTANT)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
public:
    /*!
     * \brief Конструктор
     *
     * Создает экземпляр объекта с указанным родителем.
     *
     * \param parent Укзатель на родитель
     */
    explicit UserInfo(QObject *parent = 0);

public:
    UserProfile *userProfile() const;

    FoldersModel *photosModel() const;

    FoldersModel *videosModel() const;

    FoldersModel *musicsModel() const;

    QString userProfileFile() const;
    void setUserProfileFile(const QString &userProfileFile);

signals:
    void userProfileFileChanged();
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

private:
    QString m_userProfileFile;
    UserProfile *m_userProfile;
    FoldersModel *m_photosModel;
    FoldersModel *m_videosModel;
    FoldersModel *m_musicsModel;
};

#endif // USERINFO_H
