#include "filesmodel.h"
#include <QQmlContext>
#include <QQmlEngine>

FilesModel::FilesModel(QObject *parent) :
    QAbstractListModel(parent),
    _itemAdditionalData(NULL)
{
    _roles[Id] = "file_id";
    _roles[Path] = "file_path";
    _roles[Name] = "file_name";
    _roles[Size] = "file_size";
    _roles[DowloadedSize] = "file_downloaded_size";
    _roles[AdditionalData] = "file_additional_data";
}

void FilesModel::setItemAdditionalData(QQmlComponent *folderAdditionalData)
{
    if (_itemAdditionalData != folderAdditionalData)
    {
        _itemAdditionalData = folderAdditionalData;

        emit itemAdditionalDataChanged();
    }
}

bool FilesModel::loadFromDomElement(const QDomElement &domElement)
{
    if (domElement.isNull())
        return false;

    QDomElement itemElement = domElement.firstChildElement("file");

    //Удалим все итемы, которых нет в новом XML-файле
    QList<qint64> itemIds;
    while (!itemElement.isNull())
    {
        qint64 itemId = itemElement.attribute("id", "-1").toLongLong();
        if (itemId != -1)
        {
            itemIds << itemId;
        }
        itemElement = itemElement.nextSiblingElement("file");
    }
    for (int i = rowCount() - 1; i > -1; i--)
    {
        File * file = get(i);
        if (itemIds.contains(file->id()))
            continue;

        removeRow(i);
    }

    //Пройдемся по новому XML-файлу
    itemElement = domElement.firstChildElement("file");
    int row = 0;
    while (!itemElement.isNull())
    {
        qint64 itemId = itemElement.attribute("id", "-1").toLongLong();
        if (itemId != -1)
        {
            File *oldItem = find(itemId);
            if (!oldItem)
            {
                //Создадим новый файл
                insertRow(row);
                File *newItem = get(row);
                newItem->loadFromDomElement(itemElement);
            }
            else
            {
                //Перезагрузим старый файл
                oldItem->loadFromDomElement(itemElement);

                //Переместим его, если он вдруг передвинулся
                int oldItemRow = _items.indexOf(oldItem);
                if (oldItemRow != row)
                {
                    emit beginMoveRows(QModelIndex(), oldItemRow, oldItemRow, QModelIndex(), row);
                    _items.move(oldItemRow, row);
                    emit endMoveRows();
                }
            }
        }
        itemElement = itemElement.nextSiblingElement("file");
        row++;
    }

    return true;
}

void FilesModel::itemDataChanged()
{
    File *item = qobject_cast<File *>(sender());
    if (item)
    {
        int fileRow = _items.indexOf(item);

        emit dataChanged(index(fileRow, 0), index(fileRow, 0));
    }
}

File * FilesModel::find(const qint64 id) const
{
    foreach (File * item, _items)
    {
        if (item && item->id() == id)
            return item;
    }

    return NULL;
}

void FilesModel::toDomElement(QDomElement &rootElement, QDomDocument &domDocument) const
{
    foreach (File *item, _items)
    {
        rootElement.appendChild(item->toDomElement(domDocument));
    }
}

qint64 FilesModel::filesSumSize() const
{
    qint64 result = 0;
    foreach (File *file, _items)
    {
        result += file->size();
    }

    return result;
}

qint64 FilesModel::filesDownloadedSumSize() const
{
    qint64 result = 0;
    foreach (File *file, _items)
    {
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
    insertRow(row);
    File *newItem = get(row);
    newItem->loadFromPath(path);
    return newItem;
}

bool FilesModel::remove(int row)
{
    return removeRow(row);
}

bool FilesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count-1);
    for (int i = 0; i < count; i++)
    {
        File *newItem = new File(this);
        newItem->setId(newId());
        connect(newItem, SIGNAL(pathChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(sizeChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(sizeChanged()), this, SIGNAL(filesSumSizeChanged()));
        connect(newItem, SIGNAL(downloadedSizeChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(downloadedSizeChanged()), this, SIGNAL(filesDownloadedSumSizeChanged()));

        if (_itemAdditionalData)
        {
            QQmlContext *qmlContext = QQmlEngine::contextForObject(this);

            QObject *newItemAdditionalData = _itemAdditionalData->create(qmlContext);
            newItem->setAdditionalData(newItemAdditionalData);
        }

        if (_itemAdditionalData)
        {
            QQmlContext *qmlContext = QQmlEngine::contextForObject(this);

            QObject *newFolderAdditionalData = _itemAdditionalData->create(qmlContext);
            newItem->setAdditionalData(newFolderAdditionalData);
        }

        _items.insert(row + i, newItem);
    }
    endInsertRows();
    emit filesSumSizeChanged();
    emit filesDownloadedSumSizeChanged();
    emit countChanged();
    return true;
}

bool FilesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    for (int i = row + count - 1; i > row - 1; i--)
    {
        File *item = get(i);
        item->deleteLater();
        _items.removeAt(i);
    }
    endRemoveRows();
    emit filesSumSizeChanged();
    emit filesDownloadedSumSizeChanged();
    emit countChanged();
    return true;
}

qint64 FilesModel::newId() const
{
    qint64 result = -1;
    foreach (File *item, _items)
    {
        if (item->id() > result)
            result = item->id();
    }
    return result + 1;
}

QVariant FilesModel::data(const QModelIndex &index, int role) const
{
    File * item = get(index.row());
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

File * FilesModel::get(const int row) const
{
    if (row < 0 || row > rowCount() - 1)
        return NULL;

    return _items.at(row);
}
