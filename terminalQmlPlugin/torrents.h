#ifndef TORRENTS_H
#define TORRENTS_H

#include <QObject>
#include "torrentfoldersmodel.h"

class Torrents : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TorrentFoldersModel* searchModel READ searchModel CONSTANT)
    Q_PROPERTY(TorrentFoldersModel* downloadModel READ downloadModel CONSTANT)
    Q_PROPERTY(TorrentFoldersModel* uploadModel READ uploadModel CONSTANT)
public:
    explicit Torrents(QObject *parent = 0);

public:
    TorrentFoldersModel *searchModel() const;

    TorrentFoldersModel *downloadModel() const;

    TorrentFoldersModel *uploadModel() const;

private:
    TorrentFoldersModel *m_searchModel;
    TorrentFoldersModel *m_downloadModel;
    TorrentFoldersModel *m_uploadModel;
};

#endif // TORRENTS_H
