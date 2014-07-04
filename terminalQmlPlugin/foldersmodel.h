#ifndef FOLDERSMODEL_H
#define FOLDERSMODEL_H

#include <QAbstractListModel>
#include <QUrl>
#include "folder.h"

class FoldersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)

public:
    explicit FoldersModel(QObject *parent = 0);

    Q_INVOKABLE bool reload();

    Q_INVOKABLE bool save();

    Q_INVOKABLE Folder * get(const int row) const;

    Q_INVOKABLE Folder * find(const qint64 id) const;

    Q_INVOKABLE Folder * appendNew(const QString &path);

    Q_INVOKABLE Folder * insertNew(int row, const QString &path);

    bool loadFromDomElement(const QDomElement &domElement);

    QDomElement toDomElement(QDomElement &rootElement, QDomDocument &domDocument) const;

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual inline int rowCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent); return _items.count();}

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public:
    inline QUrl source() const {return _source;}
    void setSource(const QUrl &source);

    inline QString parentElement() const {return _parentElement;}
    void setParentElement(const QString &parentElement);

signals:
    void sourceChanged();
    void parentElementChanged();

private slots:
    void itemDataChanged();

private:
    enum Roles
    {
        Id = Qt::UserRole,
        Name,
        SidsAvailability,
        LoadingInProcess
    };

    QList<Folder *> _items;
    QHash<int, QByteArray> _roles;

    qint64 newId() const;

    QUrl _source;
    QString _parentElement;
};

#endif // FOLDERSMODEL_H
