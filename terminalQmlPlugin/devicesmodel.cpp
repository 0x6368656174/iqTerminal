#include "devicesmodel.h"
#include "device.h"

DevicesModel::DevicesModel(QObject *parent) :
    FileXmlItemsModel(parent),
    m_updateChildActiveInProcess(false)
{
}

void DevicesModel::updateChildActive()
{
    if (!m_updateChildActiveInProcess) {
        m_updateChildActiveInProcess = true;
        Device * item = qobject_cast<Device *>(sender());
        if (item) {
            for (int i = 0; i < rowCount(); i++) {
                Device *nextItem = qobject_cast<Device *>(get(i));
                if (nextItem && nextItem != item) {
                    nextItem->setActive(false);
                }
            }
        }
        m_updateChildActiveInProcess = false;
    }
}
