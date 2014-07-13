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
        AdditionalData
    };

    explicit FilesModel(QObject *parent = 0);

    Q_INVOKABLE File * appendNew(const QUrl &path = QUrl());

    Q_INVOKABLE File * insertNew(int row, const QUrl &path = QUrl());

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

private:
    QHash<int, QByteArray> _roles;
};

#endif // FILESMODEL_H
