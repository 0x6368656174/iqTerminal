#ifndef SIMLEDEVICESMODEL_H
#define SIMLEDEVICESMODEL_H

#include "devicesmodel.h"

class SimpleDevicesModel : public DevicesModel
{
    Q_OBJECT
public:
    enum Roles
    {
        Id = Qt::UserRole,
        AdditionalData,
        Name,
        Active
    };

    explicit SimpleDevicesModel(QObject *parent = 0);

public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

private:
    QHash<int, QByteArray> m_roles;
};

#endif // SIMLEDEVICESMODEL_H
