#ifndef NETWORKDEVICE_H
#define NETWORKDEVICE_H

#include "device.h"

class NetworkDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(QString ip READ ip WRITE setIp NOTIFY ipChanged)
    Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged)

public:
    explicit NetworkDevice(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

protected:
    virtual void reset();

public:
    QString ip() const;
    void setIp(const QString &ip);

    QString port() const;
    void setPort(const QString &port);

signals:
    void ipChanged();
    void portChanged();

private:
    QString m_ip;
    QString m_port;
};

#endif // NETWORKDEVICE_H
