#include "messagesfiltermodel.h"
#include "message.h"
#include <QDebug>

MessagesFilterModel::MessagesFilterModel(QObject *parent) :
    AbstractXmlItemsFilterModel(parent)
{
    connect(this, SIGNAL(filterModelChanged()), this, SIGNAL(messagesModelChanged()));
}

bool MessagesFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    if (!filterModel())
        return false;
    Message * message = qobject_cast<Message *>(filterModel()->get(source_row));
    if (!message)
        return false;
    if (message->text().contains(filterString(), filterCaseSensitivity()))
    {
        return true;
    }
    else
    {
        QRegExp dateRx ("date\\s*:\\s*([^\\s]+)");
        dateRx.setCaseSensitivity(Qt::CaseInsensitive);
        if (dateRx.indexIn(filterString()) != -1)
        {
            QString dateStr = dateRx.cap(1).trimmed();
            QDate date = QDate::fromString(dateStr, "dd.MM.yyyy");
            if (!date.isValid() || !date.year() > 2000)
            {
                dateStr.insert(dateStr.length() - 2, "20");
                date = QDate::fromString(dateStr, "dd.MM.yyyy");
            }

            if (date.isValid())
            {
                if (message->sendDateTime().date() == date)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
