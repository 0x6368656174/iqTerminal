#include "torrentfile.h"
#include <QFileInfo>
#include <QUrl>
#include <QDebug>

TorrentFile::TorrentFile(QObject *parent) :
    AbstractXmlItemObject(parent),
    _path(""),
    _size(0),
    _downloadedSize(0),
    _name("")
{
}

void TorrentFile::reset()
{
    setPath("");
    setSize(0);
    setDownloadedSize(0);
}

void TorrentFile::setPath(const QString &path)
{
    if (_path != path)
    {
        _path = path;

        QFileInfo info (_path);
        setName(info.fileName());

        emit pathChanged();
    }
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

void TorrentFile::setName(const QString &name)
{
    if (_name != name)
    {
        _name = name;

        emit nameChanged();
    }
}

bool TorrentFile::loadFromPath(const QUrl &path)
{
    if (path.isValid())
    {
        QFileInfo info (path.toLocalFile());
        if (info.exists())
        {
            setPath(path.toLocalFile());
            setSize(info.size());
            setDownloadedSize(0);
            return true;
        }
    }
    return false;
}

bool TorrentFile::loadFromDomElement(const QDomElement &domElement)
{
    if (domElement.isNull())
    {
        reset();
        return false;
    }

    if (domElement.tagName() != "file")
    {
        reset();
        return false;
    }

    if (domElement.hasAttribute("id"))
    {
        setId(domElement.attribute("id").toLongLong());
    }
    else
    {
        qWarning() << "In parsing file elemnt found element without id, skipped...";
        reset();
        return false;
    }

    QDomElement pathElement = domElement.firstChildElement("path");
    if (!pathElement.isNull())
    {
        setPath(pathElement.text());
    }
    else
    {
        setPath("");
    }

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
    QDomElement rootElement = domDocument.createElement("file");
    rootElement.setAttribute("id", id());

    QDomElement pathElement = domDocument.createElement("path");
    rootElement.appendChild(pathElement);
    QDomText pathText = domDocument.createTextNode(path());
    pathElement.appendChild(pathText);

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
