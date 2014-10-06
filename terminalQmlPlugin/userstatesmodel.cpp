#include "userstatesmodel.h"
#include "userstate.h"

UserStatesModel::UserStatesModel(QObject *parent) :
    AbstractXmlItemsModel(parent)
{
    m_roles[Id] = "state_id";
    m_roles[Name] = "state_name";
    m_roles[Text] = "state_text";
    m_roles[AdditionalData] = "state_additional_data";
}

QHash<int, QByteArray> UserStatesModel::roleNames() const
{
    return m_roles;
}

AbstractXmlItemObject * UserStatesModel::newItem()
{
    UserState *newItem = new UserState(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));

    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(textChanged()), this, SLOT(itemDataChanged()));
    return newItem;
}

QVariant UserStatesModel::data(const QModelIndex &index, int role) const
{
    UserState * item = qobject_cast<UserState *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role) {
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
