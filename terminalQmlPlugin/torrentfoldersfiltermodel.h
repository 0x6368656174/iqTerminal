#ifndef TORRENTFOLDERSFILTERMODEL_H
#define TORRENTFOLDERSFILTERMODEL_H

#include "abstractxmlitemsfiltermodel.h"
#include "torrentfoldersmodel.h"
#include "torrentfolder.h"

class TorrentFoldersFilterModel : public AbstractXmlItemsFilterModel
{
    Q_OBJECT
    Q_PROPERTY(AbstractXmlItemsModel* foldersModel READ filterModel WRITE setFilterModel NOTIFY foldersModelChanged)
public:
    explicit TorrentFoldersFilterModel(QObject *parent = 0);

signals:
    void foldersModelChanged();

protected:
    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;
};

#endif // TORRENTFOLDERSFILTERMODEL_H
