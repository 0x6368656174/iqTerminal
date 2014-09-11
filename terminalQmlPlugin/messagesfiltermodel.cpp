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
        return true;

    {
        QDate date = QDate::fromString(filterString(), "dd.MM.yyyy");
        {
            if (!date.isValid() || !date.year() > 2000) {
                QString dateStr = filterString();
                dateStr.insert(dateStr.length() - 2, "20");
                date = QDate::fromString(dateStr, "dd.MM.yyyy");
            }

            if (date.isValid()) {
                if (message->sendDateTime().date() == date) {
                    return true;
                }
            }
        }
    }

    {
        QStringList worlds = filterString().split(" ", QString::SkipEmptyParts);
        if (worlds.count() == 2) {
            QDate firstDate = QDate::fromString(worlds.at(0), "dd.MM.yyyy");
            {
                if (!firstDate.isValid() || !firstDate.year() > 2000) {
                    QString dateStr = worlds.at(0);
                    dateStr.insert(dateStr.length() - 2, "20");
                    firstDate = QDate::fromString(dateStr, "dd.MM.yyyy");
                }

                if (firstDate.isValid()) {
                    QDate secondDate = QDate::fromString(worlds.at(1), "dd.MM.yyyy");
                    {
                        if (!secondDate.isValid() || !secondDate.year() > 2000) {
                            QString dateStr = worlds.at(1);
                            dateStr.insert(dateStr.length() - 2, "20");
                            secondDate = QDate::fromString(dateStr, "dd.MM.yyyy");
                        }

                        if (secondDate.isValid()) {
                            QDate sendDate = message->sendDateTime().date();
                            if (firstDate <= sendDate && sendDate <= secondDate) {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}
