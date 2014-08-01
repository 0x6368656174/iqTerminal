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
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

private:
    QHash<int, QByteArray> _roles;
};

#endif // TORRENTFOLDERSMODEL_H
