#include "torrentfoldersmodel.h"
#include <QDebug>
#include <QFile>
#include <QQmlContext>
#include <QQmlEngine>

TorrentFoldersModel::TorrentFoldersModel(QObject *parent) :
    AbstractXmlItemsModel(parent),
    _source(QUrl()),
    _parentElement(""),
    _fileAdditionalData(NULL)
{
    _roles[Id] = "folder_id";
    _roles[Name] = "folder_name";
    _roles[SidsAvailability] = "folder_sids_availability";
    _roles[InProcess] = "folder_in_process";
    _roles[AdditionalData] = "folder_additional_data";
    _roles[FilesModel] = "folder_files_model";
    _roles[Size] = "folder_size";
    _roles[DownloadedSize] = "folder_downloaded_size";

    connect(this, SIGNAL(itemAdditionalDataChanged()), this, SIGNAL(folderAdditionalDataChanged()));
}

QString TorrentFoldersModel::itemTagName() const
{
    return "folder";
}

AbstractXmlItemObject * TorrentFoldersModel::newItem()
{
    TorrentFolder *newItem = new TorrentFolder(this);
    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(sidsAvailabilityChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(inProcessChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(sizeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(downloadedSizeChanged()), this, SLOT(itemDataChanged()));
    newItem->filesModel()->setItemAdditionalData(_fileAdditionalData);
    return newItem;
}

void TorrentFoldersModel::setSource(const QUrl &source)
{
    if(_source != source)
    {
        _source = source;

        emit sourceChanged();

        reload();
    }
}

void TorrentFoldersModel::setParentElement(const QString &parentElement)
{
    if (_parentElement != parentElement)
    {
        _parentElement = parentElement;

        emit parentElementChanged();

        reload();
    }
}

void TorrentFoldersModel::setFileAdditionalData(QQmlComponent *fileAdditionalData)
{
    if (_fileAdditionalData != fileAdditionalData)
    {
        _fileAdditionalData = fileAdditionalData;

        emit fileAdditionalDataChanged();
    }
}

bool TorrentFoldersModel::reload()
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

bool TorrentFoldersModel::save()
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

TorrentFolder * TorrentFoldersModel::appendNew(const QUrl &path)
{
    return insertNew(rowCount(), path);
}

TorrentFolder * TorrentFoldersModel::insertNew(int row, const QUrl &path)
{
    TorrentFolder *newItem = qobject_cast<TorrentFolder *>(AbstractXmlItemsModel::insertNew(row));
    newItem->loadFromPath(path);
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
