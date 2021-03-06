#ifndef USER_H
#define USER_H

#include "abstractxmlitemobject.h"
#include "userinfo.h"
#include "messagesmodel.h"

class User : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(QString profile READ profile WRITE setProfile NOTIFY profileChanged)
    Q_PROPERTY(bool online READ online WRITE setOnline NOTIFY onlineChanged)
    Q_PROPERTY(bool friendshipAccepted READ friendshipAccepted WRITE setFriendshipAccepted NOTIFY friendshipAcceptedChanged)
    Q_PROPERTY(UserInfo* userInfo READ userInfo CONSTANT)
    Q_PROPERTY(MessagesModel* messagesModel READ messagesModel CONSTANT)
public:
    explicit User(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

protected:
    virtual void reset();

public:
    QString profile() const {return m_profile;}
    void setProfile(const QString &profile);
    void resetProfile();

    bool online() const;
    void setOnline(bool online);

    bool friendshipAccepted() const;
    void setFriendshipAccepted(bool friendshipAccepted);

    UserInfo *userInfo() const;

    MessagesModel *messagesModel() const;

signals:
    void profileChanged();
    void onlineChanged();
    void friendshipAcceptedChanged();

private:
    QString m_profile;
    bool m_online;
    bool m_friendshipAccepted;
    UserInfo *m_userInfo;
    MessagesModel *m_messagesModel;
};

#endif // USER_H
