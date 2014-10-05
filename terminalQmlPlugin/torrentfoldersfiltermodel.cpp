#include "torrentfoldersfiltermodel.h"
#include "file.h"
#include <QDebug>

TorrentFoldersFilterModel::TorrentFoldersFilterModel(QObject *parent) :
    AbstractXmlItemsFilterModel(parent)
{
    connect(this, SIGNAL(filterModelChanged()), this, SIGNAL(foldersModelChanged()));
}

bool TorrentFoldersFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    if (!filterModel())
        return false;
    TorrentFolder * folder = qobject_cast<TorrentFolder *>(filterModel()->get(source_row));
    if (!folder)
        return false;
    if (folder->name().contains(filterString(), filterCaseSensitivity())) {
        return true;
    } else {
        for(int i = 0; i < folder->filesModel()->rowCount(); i++) {
            File* file = qobject_cast<File *>(folder->filesModel()->get(i));
            if (file && file->name().contains(filterString(), filterCaseSensitivity()))
                return true;
        }
    }
    return false;
}
