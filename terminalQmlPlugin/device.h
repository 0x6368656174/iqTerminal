#ifndef DEVICE_H
#define DEVICE_H

#include "abstractxmlitemobject.h"

class Device : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
public:
    explicit Device(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

protected:
    virtual void reset();

public:
    bool active() const;
    void setActive(bool active);

signals:
    void activeChanged();

private:
    bool m_active;
};

#endif // DEVICE_H
