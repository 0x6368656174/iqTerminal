#include "foldersmodel.h"
#include <QDebug>

FoldersModel::FoldersModel(QObject *parent) :
    AbstractXmlItemsModel(parent),
    _source(QUrl()),
    _parentElement(""),
    _fileAdditionalData(NULL)
{
    _roles[Id] = "folder_id";
    _roles[Name] = "folder_name";
    _roles[AdditionalData] = "folder_additional_data";
    _roles[FilesModel] = "folder_files_model";

    connect(this, SIGNAL(itemAdditionalDataChanged()), this, SIGNAL(folderAdditionalDataChanged()));
}

AbstractXmlItemObject * FoldersModel::newItem()
{
    Folder *newItem = new Folder(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));
    newItem->filesModel()->setItemAdditionalData(_fileAdditionalData);
    return newItem;
}

void FoldersModel::setSource(const QUrl &source)
{
    if(_source != source) {
        _source = source;

        emit sourceChanged();

        if (!parentElement().isEmpty())
            reload();
    }
}

void FoldersModel::setParentElement(const QString &parentElement)
{
    if (_parentElement != parentElement) {
        _parentElement = parentElement;

        emit parentElementChanged();

        if (source().isValid())
            reload();
    }
}

void FoldersModel::setFileAdditionalData(QQmlComponent *fileAdditionalData)
{
    if (_fileAdditionalData != fileAdditionalData) {
        _fileAdditionalData = fileAdditionalData;

        emit fileAdditionalDataChanged();
    }
}

bool FoldersModel::reload()
{
    return reloadModel(this, source(), parentElement());
}

bool FoldersModel::save() const
{
    return saveModel(this, source(), parentElement());
}

Folder * FoldersModel::appendNew(const QUrl &path)
{
    return insertNew(rowCount(), path);
}

Folder * FoldersModel::insertNew(int row, const QUrl &path)
{
    Folder *newItem = qobject_cast<Folder *>(AbstractXmlItemsModel::insertNew(row));
    newItem->loadFromPath(path);
    return newItem;
}

QVariant FoldersModel::data(const QModelIndex &index, int role) const
{
    Folder * item = qobject_cast<Folder *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role) {
    case Id:
        return item->id();
        break;
    case Name:
        return item->name();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    case FilesModel:
        return QVariant::fromValue(item->filesModel());
        break;
    }

    return QVariant();
}

