#include "applicationmodel.h"

ApplicationModel * ApplicationModel::m_instance = nullptr;

ApplicationModel::ApplicationModel(QObject *parent) :
    QObject(parent),
    m_aboutMe(new UserInfo(this)),
    m_allModel(new UsersModel(this)),
    m_contactsModel(new UsersModel(this)),
    m_visitorsModel(new UsersModel(this)),
    m_torrents(new Torrents(this))
{
    m_aboutMe->setUserProfileFile("../i.xml");

}

ApplicationModel* ApplicationModel::instance()
{
    if (m_instance)
        return m_instance;

    m_instance = new ApplicationModel();
    return m_instance;
}

UserInfo *ApplicationModel::aboutMe() const
{
    return m_aboutMe;
}

UsersModel *ApplicationModel::allModel() const
{
    return m_allModel;
}

UsersModel *ApplicationModel::contactsModel() const
{
    return m_contactsModel;
}

UsersModel *ApplicationModel::visitorsModel() const
{
    return m_visitorsModel;
}

Torrents *ApplicationModel::torrents() const
{
    return m_torrents;
}
