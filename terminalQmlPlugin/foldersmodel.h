#ifndef FOLDERSMODEL_H
#define FOLDERSMODEL_H

#include "abstractxmlitemsmodel.h"
#include "folder.h"
#include "xmlinterface.h"

class FoldersModel : public AbstractXmlItemsModel, public XmlInterface
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)
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

    Q_INVOKABLE bool reload();

    Q_INVOKABLE bool save() const;

    Q_INVOKABLE Folder * appendNew(const QUrl &path = QUrl());

    Q_INVOKABLE Folder * insertNew(int row, const QUrl &path = QUrl());

public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

public:
    QUrl source() const;
    void setSource(const QUrl &source);

    QString parentElement() const;
    void setParentElement(const QString &parentElement);

    QQmlComponent *fileAdditionalData() const;
    void setFileAdditionalData(QQmlComponent *fileAdditionalData);

signals:
    void sourceChanged();
    void parentElementChanged();
    void folderAdditionalDataChanged();
    void fileAdditionalDataChanged();

private:
    QHash<int, QByteArray> m_roles;

    QUrl m_source;
    QString m_parentElement;
    QQmlComponent *m_fileAdditionalData;
};

#endif // FOLDERSMODEL_H
