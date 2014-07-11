#include "torrentfoldersfiltermodel.h"
#include <QDebug>

TorrentFoldersFilterModel::TorrentFoldersFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    _foldersModel(NULL),
    _filterString("")
{
    setFilterRole(TorrentFoldersModel::Name);
    setFilterKeyColumn(1);
    setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(countChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(countChanged()));
}

void TorrentFoldersFilterModel::setFoldersModel(TorrentFoldersModel *foldersModel)
{
    if (_foldersModel != foldersModel)
    {
        int oldCount = count();
        _foldersModel = foldersModel;
        setSourceModel(_foldersModel);

        if (oldCount != count())
            emit countChanged();
        emit foldersModelChanged();
    }
}

void TorrentFoldersFilterModel::setFilterString(const QString &filterString)
{
    if (_filterString != filterString)
    {
        _filterString = filterString;

        emit filterStringChanged();

        invalidateFilter();
    }
}

TorrentFolder * TorrentFoldersFilterModel::get(const int row) const
{
    if (row < 0 || row > rowCount() - 1)
        return NULL;

    if (!_foldersModel)
        return NULL;

    return qobject_cast<TorrentFolder *>(_foldersModel->get(mapToSource(index(row, 0)).row()));
}

bool TorrentFoldersFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    if (!_foldersModel)
        return false;
    TorrentFolder * folder = qobject_cast<TorrentFolder *>(_foldersModel->get(source_row));
    if (folder->name().contains(_filterString, filterCaseSensitivity()))
    {
        return true;
    }
    else
    {
        for(int i = 0; i < folder->filesModel()->rowCount(); i++)
        {
            TorrentFile* file = qobject_cast<TorrentFile*>(folder->filesModel()->get(i));
            if (file->name().contains(_filterString, filterCaseSensitivity()))
                return true;
        }
    }
    return false;
}
