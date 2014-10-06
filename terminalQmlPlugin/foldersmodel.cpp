#include "foldersmodel.h"
#include <QDebug>

FoldersModel::FoldersModel(QObject *parent) :
    FileXmlItemsModel(parent),
    m_fileAdditionalData(nullptr)
{
    m_roles[Id] = "folder_id";
    m_roles[Name] = "folder_name";
    m_roles[AdditionalData] = "folder_additional_data";
    m_roles[FilesModel] = "folder_files_model";

    connect(this, SIGNAL(itemAdditionalDataChanged()), this, SIGNAL(folderAdditionalDataChanged()));
}

AbstractXmlItemObject * FoldersModel::newItem()
{
    Folder *newItem = new Folder(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));

    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    newItem->filesModel()->setItemAdditionalData(m_fileAdditionalData);
    return newItem;
}

QHash<int, QByteArray> FoldersModel::roleNames() const
{
    return m_roles;
}

QQmlComponent* FoldersModel::fileAdditionalData() const
{
    return m_fileAdditionalData;
}

void FoldersModel::setFileAdditionalData(QQmlComponent *fileAdditionalData)
{
    if (m_fileAdditionalData != fileAdditionalData) {
        m_fileAdditionalData = fileAdditionalData;

        //Установим новые дополнительные данные для всех существующих объектов
        for (int i = 0; i < rowCount(); i++) {
            Folder *folder = qobject_cast<Folder* >(get(i));
            if (folder)
                folder->filesModel()->setItemAdditionalData(fileAdditionalData);
        }

        emit fileAdditionalDataChanged();
    }
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

