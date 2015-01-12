#include "torrents.h"
#include "core.h"

Torrents::Torrents(QObject *parent) :
    QObject(parent),
    m_searchModel(new TorrentFoldersModel(this)),
    m_downloadModel(new TorrentFoldersModel(this)),
    m_uploadModel(new TorrentFoldersModel(this))
{
    QUrl torrentXmlPath = QUrl::fromLocalFile(Core::dataDir().toLocalFile() + "/search.xml");
    m_searchModel->setParentElement("torrent");
    m_searchModel->setSource(torrentXmlPath);

    torrentXmlPath = QUrl::fromLocalFile(Core::dataDir().toLocalFile() + "/download.xml");
    m_downloadModel->setParentElement("torrent");
    m_downloadModel->setSource(torrentXmlPath);

    torrentXmlPath = QUrl::fromLocalFile(Core::dataDir().toLocalFile() + "/upload.xml");
    m_uploadModel->setParentElement("torrent");
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
