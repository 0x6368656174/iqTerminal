#ifndef TORRENTFILESMODEL_H
#define TORRENTFILESMODEL_H

#include "filesmodel.h"

class TorrentFilesModel : public FilesModel
{
    Q_OBJECT
public:
    enum Roles
    {
        Id = Qt::UserRole,
        Path,
        Name,
        Size,
        DowloadedSize,
        AdditionalData
    };

    explicit TorrentFilesModel(QObject *parent = 0);

    qint64 filesSumSize() const;

    qint64 filesDownloadedSumSize() const;

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

signals:
    void filesSumSizeChanged();
    void filesDownloadedSumSizeChanged();

private:
    QHash<int, QByteArray> _roles;
};

#endif // TORRENTFILESMODEL_H
