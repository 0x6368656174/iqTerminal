#include "usersmodel.h"
#include "user.h"

UsersModel::UsersModel(QObject *parent) :
    AbstractXmlItemsModel(parent)
{
    _roles[Id] = "user_id";
    _roles[AdditionalData] = "user_additional_data";
}

AbstractXmlItemObject * UsersModel::newItem()
{
    User *newItem = new User(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    return newItem;
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    User * item = qobject_cast<User *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role)
    {
    case Id:
        return item->id();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    }

    return QVariant();
}

