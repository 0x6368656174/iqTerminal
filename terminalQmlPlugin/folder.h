#ifndef FOLDER_H
#define FOLDER_H

#include "abstractxmlitemobject.h"
#include "abstractxmlitemsmodel.h"
#include "filesmodel.h"

class Folder : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(FilesModel * filesModel READ filesModel CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit Folder(QObject *parent = 0);

public:
    virtual bool loadFromPath(const QUrl &path);

protected:
    virtual AbstractXmlItemsModel * newFilesModel() const;

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

protected:
    virtual void reset();

public:
    FilesModel *filesModel() const;

    QString name() const;
    void setName(const QString &name);

signals:
    void nameChanged();

private:
    mutable FilesModel *m_filesModel;
    QString m_name;

    void loadFromDir(const QUrl &path);
};

#endif // FOLDER_H
