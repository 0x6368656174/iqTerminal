#include "foldersfiltermodel.h"
#include <QDebug>

FoldersFilterModel::FoldersFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    _foldersModel(NULL),
    _filterString("")
{
    setFilterRole(FoldersModel::Name);
    setFilterKeyColumn(1);
    setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(countChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(countChanged()));
}

void FoldersFilterModel::setFoldersModel(FoldersModel *foldersModel)
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

void FoldersFilterModel::setFilterString(const QString &filterString)
{
    if (_filterString != filterString)
    {
        _filterString = filterString;

        emit filterStringChanged();

        invalidateFilter();
    }
}

Folder * FoldersFilterModel::get(const int row) const
{
    if (row < 0 || row > rowCount() - 1)
        return NULL;

    if (!_foldersModel)
        return NULL;

    return qobject_cast<Folder *>(_foldersModel->get(mapToSource(index(row, 0)).row()));
}

bool FoldersFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);

    if (!_foldersModel)
        return false;
    Folder * folder = qobject_cast<Folder *>(_foldersModel->get(source_row));
    if (folder->name().contains(_filterString, filterCaseSensitivity()))
    {
        return true;
    }
    else
    {
        for(int i = 0; i < folder->filesModel()->rowCount(); i++)
        {
            File* file = qobject_cast<File*>(folder->filesModel()->get(i));
            if (file->name().contains(_filterString, filterCaseSensitivity()))
                return true;
        }
    }
    return false;
}
