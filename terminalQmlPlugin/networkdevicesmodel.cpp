#include "networkdevicesmodel.h"
#include "networkdevice.h"

NetworkDevicesModel::NetworkDevicesModel(QObject *parent) :
    DevicesModel(parent)
{
    m_roles[Id] = "network_device_id";
    m_roles[AdditionalData] = "network_device_additional_data";
    m_roles[Ip] = "network_device_ip";
    m_roles[Port] = "network_device_port";
    m_roles[Active] = "network_device_active";
}

QHash<int, QByteArray> NetworkDevicesModel::roleNames() const
{
    return m_roles;
}

AbstractXmlItemObject * NetworkDevicesModel::newItem()
{
    NetworkDevice *newItem = new NetworkDevice(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));

    connect(newItem, SIGNAL(ipChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(portChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(activeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(activeChanged()), this, SLOT(updateChildActive()));
    return newItem;
}

QVariant NetworkDevicesModel::data(const QModelIndex &index, int role) const
{
    NetworkDevice * item = qobject_cast<NetworkDevice *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role) {
    case Id:
        return item->id();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    case Ip:
        return item->ip();
        break;
    case Port:
        return item->port();
        break;
    case Active:
        return item->active();
        break;
    }

    return QVariant();
}
