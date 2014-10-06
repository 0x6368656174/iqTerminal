#ifndef FOLDERSMODEL_H
#define FOLDERSMODEL_H

#include "filexmlitemsmodel.h"
#include "folder.h"

class FoldersModel : public FileXmlItemsModel
{
    Q_OBJECT
    Q_PROPERTY(QQmlComponent* folderAdditionalData READ itemAdditionalData WRITE setItemAdditionalData NOTIFY folderAdditionalDataChanged)
    Q_PROPERTY(QQmlComponent* fileAdditionalData READ fileAdditionalData WRITE setFileAdditionalData NOTIFY fileAdditionalDataChanged)

public:
    enum Roles
    {
        Id = Qt::UserRole,
        Name,
        AdditionalData,
        FilesModel
    };
    explicit FoldersModel(QObject *parent = 0);

    Q_INVOKABLE Folder * appendNew(const QUrl &path = QUrl());

    Q_INVOKABLE Folder * insertNew(int row, const QUrl &path = QUrl());

public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

public:
    QQmlComponent *fileAdditionalData() const;
    void setFileAdditionalData(QQmlComponent *fileAdditionalData);

signals:
    void folderAdditionalDataChanged();
    void fileAdditionalDataChanged();

private:
    QHash<int, QByteArray> m_roles;
    QQmlComponent *m_fileAdditionalData;
};

#endif // FOLDERSMODEL_H
