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
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

private:
    QHash<int, QByteArray> _roles;
};

#endif // USERSTATESMODEL_H
