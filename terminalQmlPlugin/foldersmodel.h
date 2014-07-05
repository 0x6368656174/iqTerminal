#ifndef FOLDERSMODEL_H
#define FOLDERSMODEL_H

#include <QAbstractListModel>
#include <QUrl>
#include <QQmlComponent>
#include "folder.h"

class FoldersModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)
    Q_PROPERTY(QQmlComponent* folderAdditionalData READ folderAdditionalData WRITE setFolderAdditionalData NOTIFY folderAdditionalDataChanged)

public:
    explicit FoldersModel(QObject *parent = 0);

    Q_INVOKABLE bool reload();

    Q_INVOKABLE bool save();

    Q_INVOKABLE Folder * get(const int row) const;

    Q_INVOKABLE Folder * find(const qint64 id) const;

    Q_INVOKABLE Folder * appendNew(const QString &path);

    Q_INVOKABLE Folder * insertNew(int row, const QString &path);

    Q_INVOKABLE bool remove(int row);

    bool loadFromDomElement(const QDomElement &domElement);

    void toDomElement(QDomElement &rootElement, QDomDocument &domDocument) const;

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual inline int rowCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent); return _items.count();}

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public:
    int count() const {return rowCount();}

    inline QUrl source() const {return _source;}
    void setSource(const QUrl &source);

    inline QString parentElement() const {return _parentElement;}
    void setParentElement(const QString &parentElement);

    inline QQmlComponent *folderAdditionalData() const {return _folderAdditionalData;}
    void setFolderAdditionalData(QQmlComponent *folderAdditionalData);

signals:
    void countChanged();
    void sourceChanged();
    void parentElementChanged();
    void folderAdditionalDataChanged();

private slots:
    void itemDataChanged();

private:
    enum Roles
    {
        Id = Qt::UserRole,
        Name,
        SidsAvailability,
        LoadingInProcess,
        AdditionalData,
        FilesModel,
        Size,
        DownloadedSize
    };

    QList<Folder *> _items;
    QHash<int, QByteArray> _roles;

    qint64 newId() const;

    QUrl _source;
    QString _parentElement;
    QQmlComponent *_folderAdditionalData;
};

#endif // FOLDERSMODEL_H
