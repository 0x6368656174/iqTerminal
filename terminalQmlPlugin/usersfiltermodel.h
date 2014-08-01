#ifndef USERSFILTERMODEL_H
#define USERSFILTERMODEL_H

#include "abstractxmlitemsfiltermodel.h"

class UsersFilterModel : public AbstractXmlItemsFilterModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl userProfilesDir READ userProfilesDir WRITE setUserProfilesDir NOTIFY userProfilesDirChanged)
public:
    explicit UsersFilterModel(QObject *parent = 0);

public:
    inline QUrl userProfilesDir() const {return _userProfilesDir;}
    void setUserProfilesDir(const QUrl &dir);

signals:
    void userProfilesDirChanged();

protected:
    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;

private:
    QUrl _userProfilesDir;
};

#endif // USERSFILTERMODEL_H
