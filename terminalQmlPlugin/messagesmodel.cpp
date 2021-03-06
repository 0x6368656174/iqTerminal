#include "messagesmodel.h"
#include "message.h"

MessagesModel::MessagesModel(QObject *parent) :
    FileXmlItemsModel(parent)
{
    m_roles[Id] = "message_id";
    m_roles[AdditionalData] = "message_additional_data";
    m_roles[Text] = "message_text";
    m_roles[Type] = "message_type";
    m_roles[Direction] = "message_direction";
    m_roles[WasRead] = "message_was_read";
    m_roles[FilePath] = "message_file_path";
    m_roles[SendDateTime] = "message_send_date_time";
}

QHash<int, QByteArray> MessagesModel::roleNames() const
{
    return m_roles;
}

AbstractXmlItemObject * MessagesModel::newItem()
{
    Message *newItem = new Message(this);
    connect(newItem, SIGNAL(idChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(additionalDataChanged()), this, SLOT(itemDataChanged()));

    connect(newItem, SIGNAL(textChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(typeChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(directionChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(wasReadChanged()), this, SLOT(itemDataChanged()));
    connect(newItem, SIGNAL(filePathChanged()), this, SLOT(itemDataChanged()));
    return newItem;
}

QVariant MessagesModel::data(const QModelIndex &index, int role) const
{
    Message * item = qobject_cast<Message *>(get(index.row()));
    if (!item)
        return QVariant();

    switch (role) {
    case Id:
        return item->id();
        break;
    case AdditionalData:
        return QVariant::fromValue(item->additionalData());
        break;
    case Text:
        return item->text();
        break;
    case Type:
        return item->type();
        break;
    case Direction:
        return item->direction();
        break;
    case WasRead:
        return item->wasRead();
        break;
    case FilePath:
        return item->filePath();
        break;
    case SendDateTime:
        return item->sendDateTime();
        break;
    }

    return QVariant();
}
