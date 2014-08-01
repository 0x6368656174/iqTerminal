#include "torrentfile.h"
#include <QFileInfo>
#include <QUrl>

TorrentFile::TorrentFile(QObject *parent) :
    File(parent),
    _size(0),
    _downloadedSize(0)
{
}

void TorrentFile::reset()
{
    File::reset();
    setSize(0);
    setDownloadedSize(0);
}

void TorrentFile::setSize(const qint64 size)
{
    if (_size != size)
    {
        _size = size;

        emit sizeChanged();
    }
}

void TorrentFile::setDownloadedSize(const qint64 downloadedSize)
{
    if (_downloadedSize != downloadedSize)
    {
        _downloadedSize = downloadedSize;

        emit downloadedSizeChanged();
    }
}

bool TorrentFile::loadFromPath(const QUrl &path)
{
    if (File::loadFromPath(path))
    {
        QFileInfo info (path.toLocalFile());
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
    if (!sizeElement.isNull())
    {
        setSize(sizeElement.text().toLongLong());
    }
    else
    {
        setSize(0);
    }

    QDomElement downloadedSizeElement = domElement.firstChildElement("downloaded");
    if (!downloadedSizeElement.isNull())
    {
        setDownloadedSize(downloadedSizeElement.text().toLongLong());
    }
    else
    {
        setDownloadedSize(0);
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

    return rootElement;
}
