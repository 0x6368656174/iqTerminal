#include "userstatesmodel.h"
#include "userstate.h"

UserStatesModel::UserStatesModel(QObject *parent) :
    AbstractXmlItemsModel(parent)
{
    _roles[Id] = "state_id";
    _roles[Name] = "state_name";
    _roles[Text] = "state_text";
    _roles[AdditionalData] = "state_additional_data";
}

AbstractXmlItemObject * UserStatesModel::newItem()
{
    UserState *newItem = new UserState(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(textChanged()), this, SLOT(itemDataChanged()));
    return newItem;
}

QVariant UserStatesModel::data(const QModelIndex &index, int role) const
{
    UserState * item = qobject_cast<UserState *>(get(index.row()));
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
    case Text:
        return item->text();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    }

    return QVariant();
}
