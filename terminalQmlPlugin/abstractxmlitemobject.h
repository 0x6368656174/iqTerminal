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

    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const ;

    virtual QString tagName() const = 0;

protected:
    virtual void reset();

public:
    qint64 id() const;
    void setId(const qint64 id);

    QObject *additionalData() const;
    void setAdditionalData(QObject *additionalData);

signals:
    void idChanged();
    void additionalDataChanged();

private:
    qint64 m_id;
    QObject *m_additionalData;
};

#endif // ABSTRACTXMLITEMOBJECT_H
