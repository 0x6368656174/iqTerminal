#include "folder.h"
#include <QDebug>
#include <QDir>
#include <QUrl>

Folder::Folder(QObject *parent) :
    QObject(parent),
    _filesModel(new FilesModel(this)),
    _id(-1),
    _name(""),
    _sidsAvailability(0),
    _inProcess(false),
    _additionalData(NULL),
    _size(0),
    _downloadedSize(0)
{
    connect(_filesModel, SIGNAL(filesSumSizeChanged()), this, SLOT(updateSize()));
    connect(_filesModel, SIGNAL(filesDownloadedSumSizeChanged()), this, SLOT(updateDownloadedSize()));
}

void Folder::reset()
{
    setName("");
    setSidsAvailability(0);
    setInProcess(false);
    _filesModel->removeRows(0, _filesModel->rowCount());
}

void Folder::setId(const qint64 id)
{
    if (_id != id)
    {
        _id = id;

        emit idChanged();
    }
}

void Folder::setName(const QString &name)
{
    if (_name != name)
    {
        _name = name;

        emit nameChanged();
    }
}

void Folder::setSidsAvailability(const qint32 sidsAvailability)
{
    if (_sidsAvailability != sidsAvailability)
    {
        _sidsAvailability = sidsAvailability;

        emit sidsAvailabilityChanged();
    }
}

void Folder::setInProcess(const bool loadingInProcess)
{
    if (_inProcess != loadingInProcess)
    {
        _inProcess = loadingInProcess;

        emit inProcessChanged();
    }
}

void Folder::setAdditionalData(QObject *additionalData)
{
    if (_additionalData != additionalData)
    {
        _additionalData = additionalData;
        _additionalData->setParent(this);

        emit additionalDataChanged();
    }
}

void Folder::setSize(const qint64 size)
{
    if (_size != size)
    {
        _size = size;

        emit sizeChanged();
    }
}

void Folder::setDownloadedSize(const qint64 downloadedSize)
{
    if (_downloadedSize != downloadedSize)
    {
        _downloadedSize = downloadedSize;

        emit downloadedSizeChanged();
    }
}

bool Folder::loadFromPath(const QUrl &path)
{
    reset();
    QDir dir(path.toLocalFile());
    if (dir.exists())
    {
        setName(dir.dirName());

        loadFromDir(path);

        return true;
    }
    return false;
}

void Folder::updateSize()
{
    setSize(_filesModel->filesSumSize());
}

void Folder::updateDownloadedSize()
{
    setDownloadedSize(_filesModel->filesDownloadedSumSize());
}

void Folder::loadFromDir(const QUrl &path)
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

bool Folder::loadFromDomElement(const QDomElement &domElement)
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

QDomElement Folder::toDomElement(QDomDocument &domDocument) const
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
