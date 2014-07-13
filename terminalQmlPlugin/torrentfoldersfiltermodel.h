#ifndef TORRENTFOLDERSFILTERMODEL_H
#define TORRENTFOLDERSFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "torrentfoldersmodel.h"
#include "torrentfolder.h"

class TorrentFoldersFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(TorrentFoldersModel* foldersModel READ foldersModel WRITE setFoldersModel NOTIFY foldersModelChanged)
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit TorrentFoldersFilterModel(QObject *parent = 0);

    Q_INVOKABLE TorrentFolder * get(const int row) const;

public:
    inline TorrentFoldersModel *foldersModel() const {return _foldersModel;}
    void setFoldersModel(TorrentFoldersModel* foldersModel);

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
    TorrentFoldersModel *_foldersModel;
    QString _filterString;
};

#endif // TORRENTFOLDERSFILTERMODEL_H
