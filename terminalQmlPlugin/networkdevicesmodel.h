#ifndef NETWORKDEVICESMODEL_H
#define NETWORKDEVICESMODEL_H

#include "devicesmodel.h"

class NetworkDevicesModel : public DevicesModel
{
    Q_OBJECT
public:
    enum Roles
    {
        Id = Qt::UserRole,
        AdditionalData,
        Ip,
        Port,
        Active
    };

    explicit NetworkDevicesModel(QObject *parent = 0);

public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

private:
    QHash<int, QByteArray> m_roles;
};

#endif // NETWORKDEVICESMODEL_H
