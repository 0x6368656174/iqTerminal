#include "userinfo.h"
#include "core.h"
#include <QDebug>

UserInfo::UserInfo(QObject *parent) :
    QObject(parent),
    m_userProfileFile(""),
    m_userProfile(new UserProfile(this)),
    m_photosModel(new FoldersModel(this)),
    m_videosModel(new FoldersModel(this)),
    m_musicsModel(new FoldersModel(this))
{
    m_userProfile->setParentElement("user/info");
    m_userProfile->setPreferredSize(QSize(800, 600));

    m_photosModel->setParentElement("user/photos");

    m_videosModel->setParentElement("user/videos");

    m_musicsModel->setParentElement("user/music");
}

UserProfile *UserInfo::userProfile() const
{
    return m_userProfile;
}

FoldersModel *UserInfo::photosModel() const
{
    return m_photosModel;
}

FoldersModel *UserInfo::videosModel() const
{
    return m_videosModel;
}

FoldersModel *UserInfo::musicsModel() const
{
    return m_musicsModel;
}
QString UserInfo::userProfileFile() const
{
    return m_userProfileFile;
}

void UserInfo::setUserProfileFile(const QString &userProfileFile)
{
    if (m_userProfileFile != userProfileFile) {
        m_userProfileFile = userProfileFile;

        QUrl userProfileFileUrl;
        if (!userProfileFile.isEmpty()) {
            userProfileFileUrl = QUrl::fromLocalFile(Core::dataDir().toLocalFile() + "users/" + userProfileFile);
        }
        m_userProfile->setSource(userProfileFileUrl);
        m_photosModel->setSource(userProfileFileUrl);
        m_videosModel->setSource(userProfileFileUrl);
        m_musicsModel->setSource(userProfileFileUrl);

        emit userProfileFileChanged();
    }
}

