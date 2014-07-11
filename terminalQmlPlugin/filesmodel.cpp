#include "filesmodel.h"

FilesModel::FilesModel(QObject *parent) :
    AbstractXmlItemsModel(parent)
{
    _roles[Id] = "file_id";
    _roles[Path] = "file_path";
    _roles[Name] = "file_name";
    _roles[Size] = "file_size";
    _roles[DowloadedSize] = "file_downloaded_size";
    _roles[AdditionalData] = "file_additional_data";

    connect(this, SIGNAL(countChanged()), this, SIGNAL(filesDownloadedSumSizeChanged()));
    connect(this, SIGNAL(countChanged()), this, SIGNAL(filesSumSizeChanged()));
}

QString FilesModel::itemTagName() const
{
    return "file";
}

AbstractXmlItemObject * FilesModel::newItem()
{
    File *newItem = new File(this);
    connect(newItem, SIGNAL(pathChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(sizeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(sizeChanged()), this, SIGNAL(filesSumSizeChanged()));
    connect(newItem, SIGNAL(downloadedSizeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(downloadedSizeChanged()), this, SIGNAL(filesDownloadedSumSizeChanged()));
    return newItem;
}

qint64 FilesModel::filesSumSize() const
{
    qint64 result = 0;
    for (int i = 0; i < rowCount(); i++)
    {
        File *file = qobject_cast<File *>(get(i));
        result += file->size();
    }

    return result;
}

qint64 FilesModel::filesDownloadedSumSize() const
{
    qint64 result = 0;
    for (int i = 0; i < rowCount(); i++)
    {
        File *file = qobject_cast<File *>(get(i));
        result += file->downloadedSize();
    }

    return result;
}

File * FilesModel::appendNew(const QUrl &path)
{
    return insertNew(rowCount(), path);
}

File * FilesModel::insertNew(int row, const QUrl &path)
{
    File *newItem = qobject_cast<File *>(AbstractXmlItemsModel::insertNew(row));
    newItem->loadFromPath(path);
    return newItem;
}

QVariant FilesModel::data(const QModelIndex &index, int role) const
{
    File * item = qobject_cast<File *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role)
    {
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
