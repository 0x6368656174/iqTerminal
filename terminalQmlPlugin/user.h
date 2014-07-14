#ifndef USER_H
#define USER_H

#include "abstractxmlitemobject.h"

class User : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(QString profile READ profile WRITE setProfile NOTIFY profileChanged)
public:
    explicit User(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

protected:
    virtual void reset();

public:
    QString profile() const {return _profile;}
    void setProfile(const QString &profile);

signals:
    void profileChanged();

private:
    QString _profile;
};

#endif // USER_H
