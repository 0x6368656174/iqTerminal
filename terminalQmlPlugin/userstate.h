#ifndef USERSTATE_H
#define USERSTATE_H

#include "abstractxmlitemobject.h"

class UserState : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
    explicit UserState(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

protected:
    virtual void reset();

public:
    inline QString name() const {return _name;}
    void setName(const QString &name);

    inline QString text() const {return _text;}
    void setText(const QString &text);

signals:
    void nameChanged();
    void textChanged();

private:
    QString _name;
    QString _text;
};

#endif // USERSTATE_H
