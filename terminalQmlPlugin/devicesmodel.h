#ifndef DEVICESMODEL_H
#define DEVICESMODEL_H

#include "filexmlitemsmodel.h"

class DevicesModel : public FileXmlItemsModel
{
    Q_OBJECT
public:
    explicit DevicesModel(QObject *parent = 0);

protected slots:
    void updateChildActive();

private:
    bool m_updateChildActiveInProcess;
};

#endif // DEVICESMODEL_H
