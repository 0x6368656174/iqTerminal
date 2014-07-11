#ifndef FILESMODEL_H
#define FILESMODEL_H

#include "abstractxmlitemsmodel.h"
#include "file.h"

class FilesModel : public AbstractXmlItemsModel
{
    Q_OBJECT

public:
    enum Roles
    {
        Id = Qt::UserRole,
        Path,
        Name,
        Size,
        DowloadedSize,
        AdditionalData
    };

    explicit FilesModel(QObject *parent = 0);

    Q_INVOKABLE File * appendNew(const QUrl &path = QUrl());

    Q_INVOKABLE File * insertNew(int row, const QUrl &path = QUrl());

    qint64 filesSumSize() const;

    qint64 filesDownloadedSumSize() const;

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

    virtual QString itemTagName() const;

signals:
    void filesSumSizeChanged();
    void filesDownloadedSumSizeChanged();


private:
    QHash<int, QByteArray> _roles;
};

#endif // FILESMODEL_H
