#ifndef FOLDERSFILTERMODEL_H
#define FOLDERSFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "foldersmodel.h"

class FoldersFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(FoldersModel* foldersModel READ foldersModel WRITE setFoldersModel NOTIFY foldersModelChanged)
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit FoldersFilterModel(QObject *parent = 0);

    Q_INVOKABLE Folder * get(const int row) const;

public:
    inline FoldersModel *foldersModel() const {return _foldersModel;}
    void setFoldersModel(FoldersModel* foldersModel);

    inline QString filterString() const {return _filterString;}
    void setFilterString(const QString &filterString);

    inline int count() const {return rowCount();}

signals:
    void foldersModelChanged();
    void filterStringChanged();
    void countChanged();

protected:
    bool filterAcceptsRow (int source_row, const QModelIndex &source_parent) const;

private:
    FoldersModel *_foldersModel;
    QString _filterString;
};

#endif // FOLDERSFILTERMODEL_H
