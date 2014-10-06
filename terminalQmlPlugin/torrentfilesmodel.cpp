#include "torrentfilesmodel.h"
#include "torrentfile.h"

TorrentFilesModel::TorrentFilesModel(QObject *parent) :
    FilesModel(parent)
{
    m_roles[Id] = "file_id";
    m_roles[Path] = "file_path";
    m_roles[Name] = "file_name";
    m_roles[Size] = "file_size";
    m_roles[DowloadedSize] = "file_downloaded_size";
    m_roles[AdditionalData] = "file_additional_data";

    connect(this, SIGNAL(countChanged()), this, SIGNAL(filesDownloadedSumSizeChanged()));
    connect(this, SIGNAL(countChanged()), this, SIGNAL(filesSumSizeChanged()));
}

QHash<int, QByteArray> TorrentFilesModel::roleNames() const
{
    return m_roles;
}

AbstractXmlItemObject * TorrentFilesModel::newItem()
{
    TorrentFile *newItem = new TorrentFile(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));

    connect(newItem, SIGNAL(pathChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(sizeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(sizeChanged()), this, SIGNAL(filesSumSizeChanged()));
    connect(newItem, SIGNAL(downloadedSizeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(downloadedSizeChanged()), this, SIGNAL(filesDownloadedSumSizeChanged()));
    return newItem;
}

qint64 TorrentFilesModel::filesSumSize() const
{
    qint64 result = 0;
    for (int i = 0; i < rowCount(); i++) {
        TorrentFile *file = qobject_cast<TorrentFile *>(get(i));
        result += file->size();
    }

    return result;
}

qint64 TorrentFilesModel::filesDownloadedSumSize() const
{
    qint64 result = 0;
    for (int i = 0; i < rowCount(); i++) {
        TorrentFile *file = qobject_cast<TorrentFile *>(get(i));
        result += file->downloadedSize();
    }

    return result;
}

QVariant TorrentFilesModel::data(const QModelIndex &index, int role) const
{
    TorrentFile * item = qobject_cast<TorrentFile *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role) {
    case Id:
        return item->id();
        break;
    case Path:
        return item->path();
        break;
    case Name:
        return item->name();
        break;
    case Size:
        return item->size();
        break;
    case DowloadedSize:
        return item->downloadedSize();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    }

    return QVariant();
}
