#include "torrentfolder.h"
#include "torrentfilesmodel.h"
#include <QDebug>
#include <QDir>
#include <QUrl>

TorrentFolder::TorrentFolder(QObject *parent) :
    Folder(parent),
    _sidsAvailability(0),
    _inProcess(false),
    _size(0),
    _downloadedSize(0)
{
    connect(filesModel(), SIGNAL(filesSumSizeChanged()), this, SLOT(updateSize()));
    connect(filesModel(), SIGNAL(filesDownloadedSumSizeChanged()), this, SLOT(updateDownloadedSize()));
}

AbstractXmlItemsModel * TorrentFolder::newFilesModel() const
{
    return new TorrentFilesModel(const_cast<TorrentFolder *>(this));
}

void TorrentFolder::reset()
{
    Folder::reset();
    setSidsAvailability(0);
    setInProcess(false);
}

void TorrentFolder::setSidsAvailability(const qint32 sidsAvailability)
{
    if (_sidsAvailability != sidsAvailability)
    {
        _sidsAvailability = sidsAvailability;

        emit sidsAvailabilityChanged();
    }
}

void TorrentFolder::setInProcess(const bool loadingInProcess)
{
    if (_inProcess != loadingInProcess)
    {
        _inProcess = loadingInProcess;

        emit inProcessChanged();
    }
}

void TorrentFolder::setSize(const qint64 size)
{
    if (_size != size)
    {
        _size = size;

        emit sizeChanged();
    }
}

void TorrentFolder::setDownloadedSize(const qint64 downloadedSize)
{
    if (_downloadedSize != downloadedSize)
    {
        _downloadedSize = downloadedSize;

        emit downloadedSizeChanged();
    }
}

void TorrentFolder::updateSize()
{
    setSize(qobject_cast<TorrentFilesModel *>(filesModel())->filesSumSize());
}

void TorrentFolder::updateDownloadedSize()
{
    setDownloadedSize(qobject_cast<TorrentFilesModel *>(filesModel())->filesDownloadedSumSize());
}

bool TorrentFolder::loadFromDomElement(const QDomElement &domElement)
{
    if (Folder::loadFromDomElement(domElement))
    {
        QDomElement sidsAvailabilityElement = domElement.firstChildElement("availability");
        if (!sidsAvailabilityElement.isNull())
        {
            setSidsAvailability(sidsAvailabilityElement.text().toInt());
        }
        else
        {
            setSidsAvailability(0);
        }

        QDomElement inProcessElement = domElement.firstChildElement("inProcess");
        if (!inProcessElement.isNull())
        {
            setInProcess(inProcessElement.text().compare("true", Qt::CaseInsensitive) == 0);
        }
        else
        {
            setInProcess(false);
        }
        return true;
    }
    return false;
}

QDomElement TorrentFolder::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = Folder::toDomElement(domDocument);

    QDomElement sidsAvailabilityElement = domDocument.createElement("availability");
    rootElement.appendChild(sidsAvailabilityElement);
    QDomText sidsAvailabilityText = domDocument.createTextNode(QString::number(sidsAvailability()));
    sidsAvailabilityElement.appendChild(sidsAvailabilityText);

    QDomElement inProcessElement = domDocument.createElement("inProcess");
    rootElement.appendChild(inProcessElement);
    QDomText inProcessText = domDocument.createTextNode(inProcess()?"true":"false");
    inProcessElement.appendChild(inProcessText);

    return rootElement;
}
