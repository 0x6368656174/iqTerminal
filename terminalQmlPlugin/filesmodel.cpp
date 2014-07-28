#include "filesmodel.h"

FilesModel::FilesModel(QObject *parent) :
    AbstractXmlItemsModel(parent)
{
    _roles[Id] = "file_id";
    _roles[Path] = "file_path";
    _roles[Name] = "file_name";
    _roles[AdditionalData] = "file_additional_data";
}

AbstractXmlItemObject * FilesModel::newItem()
{
    File *newItem = new File(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(pathChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    return newItem;
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
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    }

    return QVariant();
}
