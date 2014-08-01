#ifndef ABSTRACTXMLITEMSFILTERMODEL_H
#define ABSTRACTXMLITEMSFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "abstractxmlitemsmodel.h"

class AbstractXmlItemsFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(AbstractXmlItemsModel* filterModel READ filterModel WRITE setFilterModel NOTIFY filterModelChanged)
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit AbstractXmlItemsFilterModel(QObject *parent = 0);

    Q_INVOKABLE AbstractXmlItemObject * get(const int row) const;

    Q_INVOKABLE bool remove(int row);

public:
    inline AbstractXmlItemsModel *filterModel() const {return _filterModel;}
    void setFilterModel(AbstractXmlItemsModel * filterModel);

    inline QString filterString() const {return _filterString;}
    void setFilterString(const QString &filterString);

    inline int count() const {return rowCount();}

signals:
    void filterModelChanged();
    void filterStringChanged();
    void countChanged();

private:
    AbstractXmlItemsModel *_filterModel;
    QString _filterString;
};

#endif // ABSTRACTXMLITEMSFILTERMODEL_H
