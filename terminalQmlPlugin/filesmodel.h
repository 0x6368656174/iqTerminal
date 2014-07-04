#ifndef FILESMODEL_H
#define FILESMODEL_H

#include <QAbstractListModel>
#include <QDomElement>
#include "file.h"

class FilesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FilesModel(QObject *parent = 0);

    Q_INVOKABLE File * get(const int row) const;

    Q_INVOKABLE File * find(const qint64 id) const;

    Q_INVOKABLE File * appendNew(const QString &path);

    Q_INVOKABLE File * insertNew(int row, const QString &path);

    bool loadFromDomElement(const QDomElement &domElement);

    QDomElement toDomElement(QDomElement &rootElement, QDomDocument &domDocument) const;

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual inline int rowCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent); return _items.count();}

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private slots:
    void itemDataChanged();

private:
    enum Roles
    {
        Id = Qt::UserRole,
        Path,
        Size,
        DowloadedSize
    };

    QList<File *> _items;
    QHash<int, QByteArray> _roles;

    qint64 newId() const;
};

#endif // FILESMODEL_H
