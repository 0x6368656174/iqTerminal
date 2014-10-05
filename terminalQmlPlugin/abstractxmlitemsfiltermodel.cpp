#include "abstractxmlitemsfiltermodel.h"

AbstractXmlItemsFilterModel::AbstractXmlItemsFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent),
    m_filterModel(nullptr),
    m_filterString("")
{
    setFilterKeyColumn(1);
    setFilterCaseSensitivity(Qt::CaseInsensitive);

    connect(this, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SIGNAL(countChanged()));
    connect(this, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SIGNAL(countChanged()));
}

int AbstractXmlItemsFilterModel::count() const
{
    return rowCount();
}

AbstractXmlItemsModel *AbstractXmlItemsFilterModel::filterModel() const
{
    return m_filterModel;
}

void AbstractXmlItemsFilterModel::setFilterModel(AbstractXmlItemsModel *filterModel)
{
    if (m_filterModel != filterModel) {
        int oldCount = count();
        m_filterModel = filterModel;
        setSourceModel(m_filterModel);

        if (oldCount != count())
            emit countChanged();
        emit filterModelChanged();
    }
}

QString AbstractXmlItemsFilterModel::filterString() const
{
    return m_filterString;
}

void AbstractXmlItemsFilterModel::setFilterString(const QString &filterString)
{
    if (m_filterString != filterString) {
        m_filterString = filterString;

        emit filterStringChanged();

        invalidateFilter();
    }
}

AbstractXmlItemObject * AbstractXmlItemsFilterModel::get(const int row) const
{
    if (row < 0 || row > rowCount() - 1)
        return nullptr;

    if (!m_filterModel)
        return nullptr;

    return m_filterModel->get(mapToSource(index(row, 0)).row());
}

bool AbstractXmlItemsFilterModel::remove(int row)
{
    if (row < 0 || row > rowCount() - 1)
        return false;

    if (!m_filterModel)
        return false;

    return m_filterModel->remove(mapToSource(index(row, 0)).row());
}
