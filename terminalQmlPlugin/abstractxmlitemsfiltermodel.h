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
    AbstractXmlItemsModel *filterModel() const;
    void setFilterModel(AbstractXmlItemsModel * filterModel);

    QString filterString() const;
    void setFilterString(const QString &filterString);

    int count() const;

signals:
    void filterModelChanged();
    void filterStringChanged();
    void countChanged();

private:
    AbstractXmlItemsModel *m_filterModel;
    QString m_filterString;
};

#endif // ABSTRACTXMLITEMSFILTERMODEL_H
