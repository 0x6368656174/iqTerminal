#ifndef USERSFILTERMODEL_H
#define USERSFILTERMODEL_H

#include "abstractxmlitemsfiltermodel.h"

class UsersFilterModel : public AbstractXmlItemsFilterModel
{
    Q_OBJECT
public:
    explicit UsersFilterModel(QObject *parent = 0);

protected:
    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;
};

#endif // USERSFILTERMODEL_H
