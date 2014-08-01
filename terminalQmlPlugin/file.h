#ifndef FILE_H
#define FILE_H

#include "abstractxmlitemobject.h"

class File : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit File(QObject *parent = 0);

public:
    virtual bool loadFromPath(const QUrl &path);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

protected:
    virtual void reset();

public:
    inline QString path() const {return _path;}
    void setPath(const QString &path);

    inline QString name() const {return _name;}
    void setName(const QString &name);

signals:
    void pathChanged();
    void nameChanged();

private:
    QString _path;
    QString _name;
};

#endif // FILE_H
