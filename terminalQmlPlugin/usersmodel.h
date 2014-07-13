#ifndef USERSMODEL_H
#define USERSMODEL_H

#include "abstractxmlitemsmodel.h"

class UsersModel : public AbstractXmlItemsModel
{
    Q_OBJECT
public:
    enum Roles
    {
        Id = Qt::UserRole,
        AdditionalData
    };

    explicit UsersModel(QObject *parent = 0);

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

private:
    QHash<int, QByteArray> _roles;
};

#endif // USERSMODEL_H
