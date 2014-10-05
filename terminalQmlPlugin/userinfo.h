#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include "userprofile.h"
#include "foldersmodel.h"

class UserInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userProfileFile READ userProfileFile WRITE setUserProfileFile NOTIFY userProfileFileChanged)
    Q_PROPERTY(UserProfile* userProfile READ userProfile CONSTANT)
    Q_PROPERTY(FoldersModel* photosModel READ photosModel CONSTANT)
    Q_PROPERTY(FoldersModel* videosModel READ videosModel CONSTANT)
    Q_PROPERTY(FoldersModel* musicsModel READ musicsModel CONSTANT)
public:
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

private:
    QString m_userProfileFile;
    UserProfile *m_userProfile;
    FoldersModel *m_photosModel;
    FoldersModel *m_videosModel;
    FoldersModel *m_musicsModel;
};

#endif // USERINFO_H
