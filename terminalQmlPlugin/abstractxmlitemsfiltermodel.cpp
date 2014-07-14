#include "abstractxmlitemsfiltermodel.h"

AbstractXmlItemsFilterModel::AbstractXmlItemsFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    _filterModel(NULL),
    _filterString("")
{
    setFilterKeyColumn(1);
    setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(countChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(countChanged()));
}

void AbstractXmlItemsFilterModel::setFilterModel(AbstractXmlItemsModel *filterModel)
{
    if (_filterModel != filterModel)
    {
        int oldCount = count();
        _filterModel = filterModel;
        setSourceModel(_filterModel);

        if (oldCount != count())
            emit countChanged();
        emit filterModelChanged();
    }
}

void AbstractXmlItemsFilterModel::setFilterString(const QString &filterString)
{
    if (_filterString != filterString)
    {
        _filterString = filterString;

        emit filterStringChanged();

        invalidateFilter();
    }
}

AbstractXmlItemObject * AbstractXmlItemsFilterModel::get(const int row) const
{
    if (row < 0 || row > rowCount() - 1)
        return NULL;

    if (!_filterModel)
        return NULL;

    return _filterModel->get(mapToSource(index(row, 0)).row());
}

bool AbstractXmlItemsFilterModel::remove(int row)
{
    if (row < 0 || row > rowCount() - 1)
        return false;

    if (!_filterModel)
        return false;

    return _filterModel->remove(mapToSource(index(row, 0)).row());
}
