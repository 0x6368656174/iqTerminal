#include "simpledevicesmodel.h"
#include "simpledevice.h"

SimpleDevicesModel::SimpleDevicesModel(QObject *parent) :
    DevicesModel(parent)
{
    m_roles[Id] = "device_id";
    m_roles[AdditionalData] = "device_additional_data";
    m_roles[Name] = "device_name";
    m_roles[Active] = "device_active";
}


QHash<int, QByteArray> SimpleDevicesModel::roleNames() const
{
    return m_roles;
}

AbstractXmlItemObject * SimpleDevicesModel::newItem()
{
    SimpleDevice *newItem = new SimpleDevice(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));

    connect(newItem, SIGNAL(nameChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(activeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(activeChanged()), this, SLOT(updateChildActive()));
    return newItem;
}

QVariant SimpleDevicesModel::data(const QModelIndex &index, int role) const
{
    SimpleDevice * item = qobject_cast<SimpleDevice *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role) {
    case Id:
        return item->id();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    case Name:
        return item->name();
        break;
    case Active:
        return item->active();
        break;
    }

    return QVariant();
}
