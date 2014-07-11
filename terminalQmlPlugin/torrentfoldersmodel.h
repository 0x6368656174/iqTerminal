#ifndef TORRENTFOLDERSMODEL_H
#define TORRENTFOLDERSMODEL_H

#include "torrentfolder.h"
#include "abstractxmlitemsmodel.h"

class TorrentFoldersModel : public AbstractXmlItemsModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)
    Q_PROPERTY(QQmlComponent* folderAdditionalData READ itemAdditionalData WRITE setItemAdditionalData NOTIFY folderAdditionalDataChanged)
    Q_PROPERTY(QQmlComponent* fileAdditionalData READ fileAdditionalData WRITE setFileAdditionalData NOTIFY fileAdditionalDataChanged)

public:
    enum Roles
    {
        Id = Qt::UserRole,
        Name,
        SidsAvailability,
        InProcess,
        AdditionalData,
        FilesModel,
        Size,
        DownloadedSize
    };

    explicit TorrentFoldersModel(QObject *parent = 0);

    Q_INVOKABLE bool reload();

    Q_INVOKABLE bool save();

    Q_INVOKABLE TorrentFolder * appendNew(const QUrl &path = QUrl());

    Q_INVOKABLE TorrentFolder * insertNew(int row, const QUrl &path = QUrl());

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

    virtual QString itemTagName() const;

public:
    inline QUrl source() const {return _source;}
    void setSource(const QUrl &source);

    inline QString parentElement() const {return _parentElement;}
    void setParentElement(const QString &parentElement);

    inline QQmlComponent *fileAdditionalData() const {return _fileAdditionalData;}
    void setFileAdditionalData(QQmlComponent *fileAdditionalData);

signals:
    void sourceChanged();
    void parentElementChanged();
    void folderAdditionalDataChanged();
    void fileAdditionalDataChanged();

private:
    QHash<int, QByteArray> _roles;

    QUrl _source;
    QString _parentElement;
    QQmlComponent *_fileAdditionalData;
};

#endif // TORRENTFOLDERSMODEL_H
