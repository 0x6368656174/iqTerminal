#ifndef USERSTATESMODEL_H
#define USERSTATESMODEL_H

#include "abstractxmlitemsmodel.h"

class UserStatesModel : public AbstractXmlItemsModel
{
    Q_OBJECT
public:
    enum Roles
    {
        Id = Qt::UserRole,
        Name,
        Text,
        AdditionalData
    };

    explicit UserStatesModel(QObject *parent = 0);

public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

private:
    QHash<int, QByteArray> m_roles;
};

#endif // USERSTATESMODEL_H
