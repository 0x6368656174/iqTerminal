#include "messagesmodel.h"
#include "message.h"

MessagesModel::MessagesModel(QObject *parent) :
    AbstractXmlItemsModel(parent),
    _source(QUrl()),
    _parentElement("")
{
    _roles[Id] = "message_id";
    _roles[AdditionalData] = "message_additional_data";
    _roles[Text] = "message_text";
    _roles[Type] = "message_type";
    _roles[Direction] = "message_direction";
    _roles[WasRead] = "message_was_read";
    _roles[FilePath] = "message_file_path";
    _roles[SendDateTime] = "message_send_date_time";
}

void MessagesModel::setSource(const QUrl &source)
{
    if(_source != source)
    {
        _source = source;

        emit sourceChanged();

        reload();
    }
}

void MessagesModel::setParentElement(const QString &parentElement)
{
    if (_parentElement != parentElement)
    {
        _parentElement = parentElement;

        emit parentElementChanged();

        reload();
    }
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

    switch (role)
    {
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

bool MessagesModel::reload()
{
    return reloadModel(this, source(), parentElement());
}

bool MessagesModel::save()
{
    return saveModel(this, source(), parentElement());
}
