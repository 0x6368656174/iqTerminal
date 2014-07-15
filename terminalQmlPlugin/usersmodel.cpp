#include "usersmodel.h"

UsersModel::UsersModel(QObject *parent) :
    AbstractXmlItemsModel(parent),
    _source(QUrl()),
    _parentElement("")
{
    _roles[Id] = "user_id";
    _roles[AdditionalData] = "user_additional_data";
    _roles[Profile] = "user_profile";
}

void UsersModel::setSource(const QUrl &source)
{
    if(_source != source)
    {
        _source = source;

        emit sourceChanged();

        reload();
    }
}

void UsersModel::setParentElement(const QString &parentElement)
{
    if (_parentElement != parentElement)
    {
        _parentElement = parentElement;

        emit parentElementChanged();

        reload();
    }
}

AbstractXmlItemObject * UsersModel::newItem()
{
    User *newItem = new User(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(profileChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));
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
    case Profile:
        return item->profile();
        break;
    }

    return QVariant();
}

bool UsersModel::reload()
{
    return reloadModel(this, source(), parentElement());
}

bool UsersModel::save()
{
    return saveModel(this, source(), parentElement());
}
