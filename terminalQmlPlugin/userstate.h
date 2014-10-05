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
    QString name() const;
    void setName(const QString &name);

    QString text() const;
    void setText(const QString &text);

signals:
    void nameChanged();
    void textChanged();

private:
    QString m_name;
    QString m_text;
};

#endif // USERSTATE_H
