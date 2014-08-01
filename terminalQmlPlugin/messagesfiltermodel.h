#ifndef MESSAGESFILTERMODEL_H
#define MESSAGESFILTERMODEL_H

#include "abstractxmlitemsfiltermodel.h"

class MessagesFilterModel : public AbstractXmlItemsFilterModel
{
    Q_OBJECT
    Q_PROPERTY(AbstractXmlItemsModel* messagesModel READ filterModel WRITE setFilterModel NOTIFY messagesModelChanged)
public:
    explicit MessagesFilterModel(QObject *parent = 0);


signals:
    void messagesModelChanged();

protected:
    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;

};

#endif // MESSAGESFILTERMODEL_H
