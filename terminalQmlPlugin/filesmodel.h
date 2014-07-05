#ifndef FILESMODEL_H
#define FILESMODEL_H

#include <QAbstractListModel>
#include <QDomElement>
#include "file.h"

class FilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit FilesModel(QObject *parent = 0);

    Q_INVOKABLE File * get(const int row) const;

    Q_INVOKABLE File * find(const qint64 id) const;

    Q_INVOKABLE File * appendNew(const QUrl &path);

    Q_INVOKABLE File * insertNew(int row, const QUrl &path);

    Q_INVOKABLE bool remove(int row);

    bool loadFromDomElement(const QDomElement &domElement);

    void toDomElement(QDomElement &rootElement, QDomDocument &domDocument) const;

    qint64 filesSumSize() const;

    qint64 filesDownloadedSumSize() const;

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual inline int rowCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent); return _items.count();}

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public:
    int count() const {return rowCount();}

signals:
    void countChanged();
    void filesSumSizeChanged();
    void filesDownloadedSumSizeChanged();

private slots:
    void itemDataChanged();

private:
    enum Roles
    {
        Id = Qt::UserRole,
        Path,
        Name,
        Size,
        DowloadedSize
    };

    QList<File *> _items;
    QHash<int, QByteArray> _roles;

    qint64 newId() const;
};

#endif // FILESMODEL_H
