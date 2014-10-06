#ifndef PROXYSETTINGS_H
#define PROXYSETTINGS_H

#include "abstractxmlitemobject.h"

class ProxySettings : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

public:
    explicit ProxySettings(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

    virtual void reset();

public:
    QString ip() const;
    void setIp(const QString &ip);

    quint16 port() const;
    void setPort(const quint16 &port);

    QString user() const;
    void setUser(const QString &user);

    QString password() const;
    void setPassword(const QString &password);

signals:
    void ipChanged();
    void portChanged();
    void userChanged();
    void passwordChanged();

private:
    QString m_ip;
    quint16 m_port;
    QString m_user;
    QString m_password;
};

#endif // PROXYSETTINGS_H
