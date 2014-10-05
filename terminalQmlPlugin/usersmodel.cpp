#include "usersmodel.h"

UsersModel::UsersModel(QObject *parent) :
    AbstractXmlItemsModel(parent),
    m_source(QUrl()),
    m_parentElement("")
{
    m_roles[Id] = "user_id";
    m_roles[AdditionalData] = "user_additional_data";
    m_roles[Profile] = "user_profile";
    m_roles[Online] = "user_online";
    m_roles[FriendshipAccepted] = "user_friendship_accepted";
    m_roles[UserInfo] = "user_info";
}

QHash<int, QByteArray> UsersModel::roleNames() const
{
    return m_roles;
}

QUrl UsersModel::source() const
{
    return m_source;
}

void UsersModel::setSource(const QUrl &source)
{
    if(m_source != source) {
        m_source = source;

        emit sourceChanged();

        if (!parentElement().isEmpty())
            reload();
    }
}

QString UsersModel::parentElement() const
{
    return m_parentElement;
}

void UsersModel::setParentElement(const QString &parentElement)
{
    if (m_parentElement != parentElement) {
        m_parentElement = parentElement;

        emit parentElementChanged();

        if (source().isValid())
            reload();
    }
}

AbstractXmlItemObject * UsersModel::newItem()
{
    User *newItem = new User(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(profileChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(onlineChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(friendshipAcceptedChanged()), this, SLOT(itemDataChanged()));
    return newItem;
}

QVariant UsersModel::data(const QModelIndex &index, int role) const
{
    User * item = qobject_cast<User *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role) {
    case Id:
        return item->id();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    case Profile:
        return item->profile();
        break;
    case Online:
        return item->online();
        break;
    case FriendshipAccepted:
        return item->friendshipAccepted();
        break;
    case UserInfo:
        return QVariant::fromValue(item->userInfo());
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
