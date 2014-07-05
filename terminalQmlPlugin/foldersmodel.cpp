#include "foldersmodel.h"
#include "folder.h"
#include <QDebug>
#include <QFile>
#include <QQmlContext>
#include <QQmlEngine>

FoldersModel::FoldersModel(QObject *parent) :
    QAbstractListModel(parent),
    _source(QUrl()),
    _parentElement(""),
    _folderAdditionalData(NULL)
{
    _roles[Id] = "folder_id";
    _roles[Name] = "folder_name";
    _roles[SidsAvailability] = "folder_sids_availability";
    _roles[LoadingInProcess] = "folder_loading_in_process";
    _roles[AdditionalData] = "folder_additional_data";
    _roles[FilesModel] = "folder_files_model";
    _roles[Size] = "folder_size";
    _roles[DownloadedSize] = "folder_downloaded_size";
}

void FoldersModel::setSource(const QUrl &source)
{
    if(_source != source)
    {
        _source = source;

        emit sourceChanged();

        reload();
    }
}

void FoldersModel::setParentElement(const QString &parentElement)
{
    if (_parentElement != parentElement)
    {
        _parentElement = parentElement;

        emit parentElementChanged();

        reload();
    }
}

void FoldersModel::setFolderAdditionalData(QQmlComponent *folderAdditionalData)
{
    if (_folderAdditionalData != folderAdditionalData)
    {
        _folderAdditionalData = folderAdditionalData;

        emit folderAdditionalDataChanged();
    }
}

bool FoldersModel::reload()
{
    if (!_source.isValid())
    {
        qWarning() << "Reload FoldersModel failed. Source must be set first.";
        return false;
    }

    if (_parentElement.isEmpty())
    {
        qWarning() << "Reload FoldersModel failed. ParentElement must be set first.";
        return false;
    }

    QFile file (source().toLocalFile());
    if (!file.exists())
    {
        qWarning() << QString("Reload FoldersModel failde. File \"%0\" not exists.").arg(source().toLocalFile());
        return false;
    }
    if (!file.open(QFile::ReadOnly))
    {
        qWarning() << QString("Reload FoldersModel failde. Can not open file \"%0\".").arg(source().toLocalFile());
        return false;
    }
    QString error;
    QDomDocument domDoc;
    if (!domDoc.setContent(&file, &error))
    {
        qWarning() << QString("Reload FoldersModel failde. Parse file \"%0\" error. Error: \"%1\".")
                      .arg(source().toLocalFile())
                      .arg(error);
        return false;
    }

    QDomElement rootElement = domDoc.documentElement();
    QStringList domPath = _parentElement.split("/", QString::SkipEmptyParts);

    if (rootElement.tagName() != domPath.first())
    {
        qWarning() << QString("Reload FoldersModel failde. In file \"%0\" not found \"%1\".")
                      .arg(source().toLocalFile())
                      .arg(domPath.first());
        return false;
    }
    domPath.removeFirst();

    foreach (QString domPathString, domPath)
    {
        rootElement = rootElement.firstChildElement(domPathString);
        if (rootElement.isNull())
        {
            qWarning() << QString("Reload FoldersModel failde. In file \"%0\" not found \"%1\".")
                          .arg(source().toLocalFile())
                          .arg(domPathString);
            return false;
        }
    }

    return loadFromDomElement(rootElement);
}

bool FoldersModel::save()
{
    if (!_source.isValid())
    {
        qWarning() << "Save FoldersModel failed. Source must be set first.";
        return false;
    }

    if (_parentElement.isEmpty())
    {
        qWarning() << "Save FoldersModel failed. ParentElement must be set first.";
        return false;
    }

    QDomDocument domDoc;
    QDomElement rootElement;
    QFile file (source().toLocalFile());
    if (file.exists())
    {
        if (!file.open(QFile::ReadOnly))
        {
            qWarning() << QString("Save FoldersModel failde. Can not open file \"%0\".").arg(source().toLocalFile());
            return false;
        }
        QString error;
        if (!domDoc.setContent(&file, &error))
        {
            qWarning() << QString("Save FoldersModel failde. Parse file \"%0\" error. Error: \"%1\".")
                          .arg(source().toLocalFile())
                          .arg(error);
            return false;
        }
        file.close();

        rootElement = domDoc.documentElement();
        QStringList domPath = _parentElement.split("/", QString::SkipEmptyParts);

        if (rootElement.tagName() != domPath.first())
        {
            qWarning() << QString("Reload FoldersModel failde. In file \"%0\" not found \"%1\".")
                          .arg(source().toLocalFile())
                          .arg(domPath.first());
            return false;
        }
        domPath.removeFirst();

        foreach (QString domPathString, domPath)
        {
            QDomElement parentElement = rootElement;
            rootElement = parentElement.firstChildElement(domPathString);
            if (rootElement.isNull())
            {
                rootElement = domDoc.createElement(domPathString);
                parentElement.appendChild(rootElement);
            }
        }
    }
    else
    {
        QStringList domPath = _parentElement.split("/", QString::SkipEmptyParts);

        rootElement = domDoc.createElement(domPath.first());
        domDoc.appendChild(rootElement);

        foreach (QString domPathString, domPath)
        {
            QDomElement parentElement = rootElement;
            rootElement = domDoc.createElement(domPathString);
            parentElement.appendChild(rootElement);
        }
    }

    //Пересоздадим основной элемент
    QDomElement oldRootElement = rootElement;
    rootElement = domDoc.createElement(rootElement.tagName());
    oldRootElement.parentNode().appendChild(rootElement);
    oldRootElement.parentNode().removeChild(oldRootElement);

    toDomElement(rootElement, domDoc);

    file.open(QFile::WriteOnly);
    QTextStream ts(&file);
    ts << domDoc.toString();
    file.close();

    return true;
}

bool FoldersModel::loadFromDomElement(const QDomElement &domElement)
{
    if (domElement.isNull())
        return false;

    QDomElement itemElement = domElement.firstChildElement("folder");

    //Удалим все итемы, которых нет в новом XML-файле
    QList<qint64> itemIds;
    while (!itemElement.isNull())
    {
        qint64 itemId = itemElement.attribute("id", "-1").toLongLong();
        if (itemId != -1)
        {
            itemIds << itemId;
        }
        itemElement = itemElement.nextSiblingElement("folder");
    }
    for (int i = rowCount() - 1; i > -1; i--)
    {
        Folder * file = get(i);
        if (itemIds.contains(file->id()))
            continue;

        removeRow(i);
    }

    //Пройдемся по новому XML-файлу
    itemElement = domElement.firstChildElement("folder");
    int row = 0;
    while (!itemElement.isNull())
    {
        qint64 itemId = itemElement.attribute("id", "-1").toLongLong();
        if (itemId != -1)
        {
            Folder *oldItem = find(itemId);
            if (!oldItem)
            {
                //Создадим новый файл
                insertRow(row);
                Folder *newItem = get(row);
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
        itemElement = itemElement.nextSiblingElement("folder");
        row++;
    }

    return true;
}

void FoldersModel::itemDataChanged()
{
    Folder *item = qobject_cast<Folder *>(sender());
    if (item)
    {
        int fileRow = _items.indexOf(item);

        emit dataChanged(index(fileRow, 0), index(fileRow, 0));
    }
}

Folder * FoldersModel::find(const qint64 id) const
{
    foreach (Folder * item, _items)
    {
        if (item && item->id() == id)
            return item;
    }

    return NULL;
}

void FoldersModel::toDomElement(QDomElement &rootElement, QDomDocument &domDocument) const
{
    foreach (Folder *item, _items)
    {
        rootElement.appendChild(item->toDomElement(domDocument));
    }
}

Folder * FoldersModel::appendNew(const QString &path)
{
    return insertNew(rowCount(), path);
}

Folder * FoldersModel::insertNew(int row, const QString &path)
{
    insertRow(row);
    Folder *newItem = get(row);
    newItem->loadFromPath(path);
    return newItem;
}

bool FoldersModel::remove(int row)
{
    return removeRow(row);
}

bool FoldersModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count-1);
    for (int i = 0; i < count; i++)
    {
        Folder *newItem = new Folder(this);
        newItem->setId(newId());
        connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(sidsAvailabilityChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(loadingInProcessChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(sizeChanged()), this, SLOT(itemDataChanged()));
        connect(newItem, SIGNAL(downloadedSizeChanged()), this, SLOT(itemDataChanged()));

        if (_folderAdditionalData)
        {
            QQmlContext *qmlContext = QQmlEngine::contextForObject(this);

            QObject *newFolderAdditionalData = _folderAdditionalData->create(qmlContext);
            newItem->setAdditionalData(newFolderAdditionalData);
        }
        _items.insert(row + i, newItem);
    }
    endInsertRows();
    emit countChanged();
    return true;
}

bool FoldersModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    for (int i = row + count - 1; i > row - 1; i--)
    {
        Folder *item = get(i);
        item->deleteLater();
        _items.removeAt(i);
    }
    endRemoveRows();
    emit countChanged();
    return true;
}

qint64 FoldersModel::newId() const
{
    qint64 result = -1;
    foreach (Folder *item, _items)
    {
        if (item->id() > result)
            result = item->id();
    }
    return result + 1;
}

QVariant FoldersModel::data(const QModelIndex &index, int role) const
{
    Folder * item = get(index.row());
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
    case LoadingInProcess:
        return item->loadingInProcess();
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

Folder * FoldersModel::get(const int row) const
{
    if (row < 0 || row > rowCount() - 1)
        return NULL;

    return _items.at(row);
}
