#include "usersfiltermodel.h"
#include "user.h"
#include "userprofile.h"

UsersFilterModel::UsersFilterModel(QObject *parent) :
    AbstractXmlItemsFilterModel(parent)
{
}

void UsersFilterModel::setUserProfilesDir(const QUrl &dir)
{
    if (_userProfilesDir != dir)
    {
        _userProfilesDir = dir;

        emit userProfilesDirChanged();
    }
}

bool UsersFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    if (!filterModel())
        return false;
    User * user = qobject_cast<User *>(filterModel()->get(source_row));
    if (!user)
        return false;

    UserProfile userProfile;
    userProfile.setParentElement("user/info");
    QUrl userProfileFile = QUrl::fromLocalFile(userProfilesDir().toLocalFile() + user->profile());
    userProfile.setSource(userProfileFile);

    if (userProfile.name().contains(filterString(), filterCaseSensitivity()))
    {
        return true;
    }
    return false;
}
