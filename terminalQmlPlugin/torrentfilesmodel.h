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
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

signals:
    void filesSumSizeChanged();
    void filesDownloadedSumSizeChanged();

private:
    QHash<int, QByteArray> m_roles;
};

#endif // TORRENTFILESMODEL_H
