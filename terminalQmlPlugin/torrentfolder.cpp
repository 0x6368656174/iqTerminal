#include "torrentfolder.h"
#include <QDebug>
#include <QDir>
#include <QUrl>

TorrentFolder::TorrentFolder(QObject *parent) :
    AbstractXmlItemObject(parent),
    _filesModel(new TorrentFilesModel(this)),
    _name(""),
    _sidsAvailability(0),
    _inProcess(false),
    _size(0),
    _downloadedSize(0)
{
    connect(_filesModel, SIGNAL(filesSumSizeChanged()), this, SLOT(updateSize()));
    connect(_filesModel, SIGNAL(filesDownloadedSumSizeChanged()), this, SLOT(updateDownloadedSize()));
}

void TorrentFolder::reset()
{
    setName("");
    setSidsAvailability(0);
    setInProcess(false);
    _filesModel->removeRows(0, _filesModel->rowCount());
}

void TorrentFolder::setName(const QString &name)
{
    if (_name != name)
    {
        _name = name;

        emit nameChanged();
    }
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

bool TorrentFolder::loadFromPath(const QUrl &path)
{
    reset();
    if (path.isValid())
    {
        QDir dir(path.toLocalFile());
        if (dir.exists())
        {
            setName(dir.dirName());

            loadFromDir(path);

            return true;
        }
    }
    return false;
}

void TorrentFolder::updateSize()
{
    setSize(_filesModel->filesSumSize());
}

void TorrentFolder::updateDownloadedSize()
{
    setDownloadedSize(_filesModel->filesDownloadedSumSize());
}

void TorrentFolder::loadFromDir(const QUrl &path)
{
    if (path.isValid())
    {
        QDir dir(path.toLocalFile());
        if (dir.exists())
        {
            foreach (QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot))
            {
                if (fileInfo.isDir())
                {
                    loadFromDir(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
                }
                else
                {
                    _filesModel->appendNew(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
                }
            }
        }
    }
}

bool TorrentFolder::loadFromDomElement(const QDomElement &domElement)
{
    if (domElement.isNull())
    {
        reset();
        return false;
    }

    if (domElement.tagName() != "folder")
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
        qWarning() << "In parsing folder elemnt found element without id, skipped...";
        reset();
        return false;
    }

    QDomElement nameElement = domElement.firstChildElement("name");
    if (!nameElement.isNull())
    {
        setName(nameElement.text());
    }
    else
    {
        setName("");
    }

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

    return _filesModel->loadFromDomElement(domElement);
}

QDomElement TorrentFolder::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = domDocument.createElement("folder");
    rootElement.setAttribute("id", id());

    QDomElement nameElement = domDocument.createElement("name");
    rootElement.appendChild(nameElement);
    QDomText nameText = domDocument.createTextNode(name());
    nameElement.appendChild(nameText);

    QDomElement sidsAvailabilityElement = domDocument.createElement("availability");
    rootElement.appendChild(sidsAvailabilityElement);
    QDomText sidsAvailabilityText = domDocument.createTextNode(QString::number(sidsAvailability()));
    sidsAvailabilityElement.appendChild(sidsAvailabilityText);

    QDomElement inProcessElement = domDocument.createElement("inProcess");
    rootElement.appendChild(inProcessElement);
    QDomText inProcessText = domDocument.createTextNode(inProcess()?"true":"false");
    inProcessElement.appendChild(inProcessText);

    _filesModel->toDomElement(rootElement, domDocument);

    return rootElement;
}
