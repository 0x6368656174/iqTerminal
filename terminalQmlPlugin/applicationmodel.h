#ifndef APPLICATIONMODEL_H
#define APPLICATIONMODEL_H

#include <QObject>
#include "userinfo.h"
#include "usersmodel.h"
#include "torrents.h"

class ApplicationModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(UserInfo* aboutMe READ aboutMe CONSTANT)
    Q_PROPERTY(UsersModel* allModel READ allModel CONSTANT)
    Q_PROPERTY(UsersModel* contactsModel READ contactsModel CONSTANT)
    Q_PROPERTY(UsersModel* visitorsModel READ visitorsModel CONSTANT)
    Q_PROPERTY(Torrents* torrents READ torrents CONSTANT)
public:
    static ApplicationModel *instance();

public:
    UserInfo *aboutMe() const;

    UsersModel *allModel() const;

    UsersModel *contactsModel() const;

    UsersModel *visitorsModel() const;

    Torrents *torrents() const;

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
