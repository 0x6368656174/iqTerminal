#include "userinfo.h"
#include "core.h"
#include <QDebug>

UserInfo::UserInfo(QObject *parent) :
    QObject(parent),
    m_userProfileFile(""),
    m_userProfile(new UserProfile(this)),
    m_photosModel(new FoldersModel(this)),
    m_videosModel(new FoldersModel(this)),
    m_musicsModel(new FoldersModel(this)),
    m_sms(false),
    m_onlinec(false),
    m_onlinea(false),
    m_onlinev(false)
{
    m_userProfile->setParentElement("user/info");
    m_userProfile->setPreferredSize(QSize(160, 160));

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

void UserInfo::resetUserProfileFile()
{
    QUrl userProfileFileUrl;
    if (!m_userProfileFile.isEmpty()) {
        userProfileFileUrl = QUrl::fromLocalFile(Core::dataDir().toLocalFile() + "users/" + m_userProfileFile);
    }
    m_userProfile->resetSource();
    m_photosModel->resetSource();
    m_videosModel->resetSource();
    m_musicsModel->resetSource();

    emit userProfileFileChanged();
}

bool UserInfo::sms() const
{
    return m_sms;
}

void UserInfo::setSms(bool value)
{
    if (m_sms != value) {
        m_sms = value;
        emit smsChanged();
    }
}

bool UserInfo::onlinec() const
{
    return m_onlinec;
}

void UserInfo::setOnlinec(bool value)
{
    if (m_onlinec != value) {
        m_onlinec = value;
        emit onlinecChanged();
    }
}

bool UserInfo::onlinea() const
{
    return m_onlinea;
}

void UserInfo::setOnlinea(bool value)
{
    if (m_onlinea != value) {
        m_onlinea = value;
        emit onlineaChanged();
    }
}

bool UserInfo::onlinev() const
{
    return m_onlinev;
}

void UserInfo::setOnlinev(bool value)
{
    if (m_onlinev != value) {
        m_onlinev = value;
        emit onlinevChanged();
    }
}
