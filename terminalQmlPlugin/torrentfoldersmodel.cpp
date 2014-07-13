#include "torrentfoldersmodel.h"
#include "torrentfolder.h"
#include <QDebug>

TorrentFoldersModel::TorrentFoldersModel(QObject *parent) :
    FoldersModel(parent)
{
    _roles[Id] = "folder_id";
    _roles[Name] = "folder_name";
    _roles[SidsAvailability] = "folder_sids_availability";
    _roles[InProcess] = "folder_in_process";
    _roles[AdditionalData] = "folder_additional_data";
    _roles[FilesModel] = "folder_files_model";
    _roles[Size] = "folder_size";
    _roles[DownloadedSize] = "folder_downloaded_size";
}

AbstractXmlItemObject * TorrentFoldersModel::newItem()
{
    TorrentFolder *newItem = new TorrentFolder(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(sidsAvailabilityChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(inProcessChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(sizeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(downloadedSizeChanged()), this, SLOT(itemDataChanged()));
    newItem->filesModel()->setItemAdditionalData(fileAdditionalData());
    return newItem;
}

QVariant TorrentFoldersModel::data(const QModelIndex &index, int role) const
{
    TorrentFolder * item = qobject_cast<TorrentFolder *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role)
    {
    case Id:
        return item->id();
        break;
    case Name:
        return item->name();
        break;
    case SidsAvailability:
        return item->sidsAvailability();
        break;
    case InProcess:
        return item->inProcess();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    case FilesModel:
        return QVariant::fromValue(item->filesModel());
        break;
    case Size:
        return item->size();
        break;
    case DownloadedSize:
        return item->downloadedSize();
        break;
    }

    return QVariant();
}
