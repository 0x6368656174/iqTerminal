#ifndef TORRENTFOLDERSMODEL_H
#define TORRENTFOLDERSMODEL_H

#include "foldersmodel.h"

class TorrentFoldersModel : public FoldersModel
{
    Q_OBJECT
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

public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

private:
    QHash<int, QByteArray> m_roles;
};

#endif // TORRENTFOLDERSMODEL_H
