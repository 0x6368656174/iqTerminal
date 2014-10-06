#include "torrents.h"
#include "core.h"

Torrents::Torrents(QObject *parent) :
    QObject(parent),
    m_searchModel(new TorrentFoldersModel(this)),
    m_downloadModel(new TorrentFoldersModel(this)),
    m_uploadModel(new TorrentFoldersModel(this))
{
    QUrl torrentXmlPath = QUrl::fromLocalFile(Core::dataDir().toLocalFile() + "/torrent.xml");

    m_searchModel->setParentElement("torrent/upload");
    m_searchModel->setSource(torrentXmlPath);

    m_downloadModel->setParentElement("torrent/download");
    m_downloadModel->setSource(torrentXmlPath);

    m_uploadModel->setParentElement("torrent/upload");
    m_uploadModel->setSource(torrentXmlPath);
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
