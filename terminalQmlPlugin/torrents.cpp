#include "torrents.h"

Torrents::Torrents(QObject *parent) :
    QObject(parent),
    m_searchModel(new TorrentFoldersModel(this)),
    m_downloadModel(new TorrentFoldersModel(this)),
    m_uploadModel(new TorrentFoldersModel(this))
{
}
TorrentFoldersModel *Torrents::searchModel() const
{
    return m_searchModel;
}

TorrentFoldersModel *Torrents::downloadModel() const
{
    return m_downloadModel;
}

TorrentFoldersModel *Torrents::uploadModel() const
{
    return m_uploadModel;
}
