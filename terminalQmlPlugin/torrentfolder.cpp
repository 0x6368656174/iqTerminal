#include "torrentfolder.h"
#include "torrentfilesmodel.h"
#include <QDebug>
#include <QDir>
#include <QUrl>

TorrentFolder::TorrentFolder(QObject *parent) :
    Folder(parent),
    m_sidsAvailability(0),
    m_inProcess(false),
    m_size(0),
    m_downloadedSize(0)
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

qint32 TorrentFolder::sidsAvailability() const
{
    return m_sidsAvailability;
}

void TorrentFolder::setSidsAvailability(const qint32 sidsAvailability)
{
    if (m_sidsAvailability != sidsAvailability) {
        m_sidsAvailability = sidsAvailability;

        emit sidsAvailabilityChanged();
    }
}

bool TorrentFolder::inProcess() const
{
    return m_inProcess;
}

void TorrentFolder::setInProcess(const bool loadingInProcess)
{
    if (m_inProcess != loadingInProcess) {
        m_inProcess = loadingInProcess;

        emit inProcessChanged();
    }
}

qint64 TorrentFolder::size() const
{
    return m_size;
}

void TorrentFolder::setSize(const qint64 size)
{
    if (m_size != size) {
        m_size = size;

        emit sizeChanged();
    }
}

qint64 TorrentFolder::downloadedSize() const
{
    return m_downloadedSize;
}

void TorrentFolder::setDownloadedSize(const qint64 downloadedSize)
{
    if (m_downloadedSize != downloadedSize) {
        m_downloadedSize = downloadedSize;

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
    if (Folder::loadFromDomElement(domElement)) {
        QDomElement sidsAvailabilityElement = domElement.firstChildElement("availability");
        if (!sidsAvailabilityElement.isNull()) {
            setSidsAvailability(sidsAvailabilityElement.text().toInt());
        } else {
            setSidsAvailability(0);
        }

        QDomElement inProcessElement = domElement.firstChildElement("in_process");
        if (!inProcessElement.isNull()) {
            setInProcess(inProcessElement.text().compare("true", Qt::CaseInsensitive) == 0);
        } else {
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

    QDomElement inProcessElement = domDocument.createElement("in_process");
    rootElement.appendChild(inProcessElement);
    QDomText inProcessText = domDocument.createTextNode(inProcess()?"true":"false");
    inProcessElement.appendChild(inProcessText);

    return rootElement;
}
