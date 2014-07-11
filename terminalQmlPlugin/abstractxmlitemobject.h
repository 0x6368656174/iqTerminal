#ifndef ABSTRACTXMLITEMOBJECT_H
#define ABSTRACTXMLITEMOBJECT_H

#include <QObject>
#include <QDomElement>

class AbstractXmlItemObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QObject* additionalData READ additionalData WRITE setAdditionalData NOTIFY additionalDataChanged)
public:
    explicit AbstractXmlItemObject(QObject *parent = 0);

    virtual bool loadFromDomElement(const QDomElement &domElement) = 0;

    virtual QDomElement toDomElement(QDomDocument &domDocument) const = 0;

public:
    inline qint64 id() const {return _id;}
    void setId(const qint64 id);

    inline QObject *additionalData() const {return _additionalData;}
    void setAdditionalData(QObject *additionalData);

signals:
    void idChanged();
    void additionalDataChanged();

private:
    qint64 _id;
    QObject *_additionalData;
};

#endif // ABSTRACTXMLITEMOBJECT_H
