#include "usersfiltermodel.h"
#include "user.h"
#include "userprofile.h"

UsersFilterModel::UsersFilterModel(QObject *parent) :
    AbstractXmlItemsFilterModel(parent)
{
}

bool UsersFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    if (!filterModel())
        return false;
    User * user = qobject_cast<User *>(filterModel()->get(source_row));
    if (!user)
        return false;

    if (!user->userInfo())
        return false;

    if (!user->userInfo()->userProfile())
        return false;

    if (user->userInfo()->userProfile()->name().contains(filterString(), filterCaseSensitivity())) {
        return true;
    }
    return false;
}
