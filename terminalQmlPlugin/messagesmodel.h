#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include "filexmlitemsmodel.h"

class MessagesModel : public FileXmlItemsModel
{
    Q_OBJECT
public:
    enum Roles
    {
        Id = Qt::UserRole,
        AdditionalData,
        Text,
        Type,
        Direction,
        WasRead,
        FilePath,
        SendDateTime
    };

    explicit MessagesModel(QObject *parent = 0);

public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();


private:
    QHash<int, QByteArray> m_roles;
};

#endif // MESSAGESMODEL_H
