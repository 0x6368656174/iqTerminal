#ifndef SIMPLEDEVICE_H
#define SIMPLEDEVICE_H

#include "device.h"

class SimpleDevice : public Device
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit SimpleDevice(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

protected:
    virtual void reset();

public:
    QString name() const;
    void setName(const QString &name);

signals:
    void nameChanged();

private:
    QString m_name;
};

#endif // SIMPLEDEVICE_H
