#include "torrentfile.h"
#include <QFileInfo>
#include <QUrl>
#include <QCryptographicHash>

TorrentFile::TorrentFile(QObject *parent) :
    File(parent),
    m_size(0),
    m_downloadedSize(0),
    m_hash("")
{
}

void TorrentFile::reset()
{
    File::reset();
    setSize(0);
    setDownloadedSize(0);
    setHash("");
}
QString TorrentFile::hash() const
{
    return m_hash;
}

void TorrentFile::setHash(const QString &hash)
{
    if (m_hash != hash) {
        m_hash = hash;
        emit hashChanged();
    }
}

qint64 TorrentFile::size() const
{
    return m_size;
}

void TorrentFile::setSize(const qint64 size)
{
    if (m_size != size) {
        m_size = size;

        emit sizeChanged();
    }
}

qint64 TorrentFile::downloadedSize() const
{
    return m_downloadedSize;
}

void TorrentFile::setDownloadedSize(const qint64 downloadedSize)
{
    if (m_downloadedSize != downloadedSize) {
        m_downloadedSize = downloadedSize;

        emit downloadedSizeChanged();
    }
}

bool TorrentFile::loadFromPath(const QUrl &path)
{
    if (File::loadFromPath(path)) {
        QFileInfo info (path.toLocalFile());

        QFile file (path.toLocalFile());
        QString fileHash;
        if (file.open(QFile::ReadOnly)) {
            fileHash = QCryptographicHash::hash(file.readAll(), QCryptographicHash::Md5);
            file.close();
        }

        setHash(fileHash);
        setSize(info.size());
        setDownloadedSize(0);
        return true;
    }
    return false;
}

bool TorrentFile::loadFromDomElement(const QDomElement &domElement)
{
    if (!File::loadFromDomElement(domElement))
        return false;

    QDomElement sizeElement = domElement.firstChildElement("size");
    if (!sizeElement.isNull()) {
        setSize(sizeElement.text().toLongLong());
    } else {
        setSize(0);
    }

    QDomElement downloadedSizeElement = domElement.firstChildElement("downloaded");
    if (!downloadedSizeElement.isNull()) {
        setDownloadedSize(downloadedSizeElement.text().toLongLong());
    } else {
        setDownloadedSize(0);
    }

    QDomElement hashElement = domElement.firstChildElement("hash");
    if (!hashElement.isNull()) {
        setHash(hashElement.text());
    } else {
        setHash("");
    }

    return true;
}

QDomElement TorrentFile::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = File::toDomElement(domDocument);

    QDomElement sizeElement = domDocument.createElement("size");
    rootElement.appendChild(sizeElement);
    QDomText sizeText = domDocument.createTextNode(QString::number(size()));
    sizeElement.appendChild(sizeText);

    QDomElement downloadedSizeElement = domDocument.createElement("downloaded");
    rootElement.appendChild(downloadedSizeElement);
    QDomText downloadedSizeText = domDocument.createTextNode(QString::number(downloadedSize()));
    downloadedSizeElement.appendChild(downloadedSizeText);

    QDomElement hashElement = domDocument.createElement("hash");
    rootElement.appendChild(hashElement);
    QDomText hashText = domDocument.createTextNode(hash());
    hashElement.appendChild(hashText);

    return rootElement;
}
