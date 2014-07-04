#include "folder.h"
#include <QDebug>
#include <QDir>

Folder::Folder(QObject *parent) :
    QObject(parent),
    _filesModel(new FilesModel(this)),
    _id(-1),
    _name(""),
    _sidsAvailability(0),
    _loadingInProcess(false)
{
}

void Folder::reset()
{
    setName("");
    setSidsAvailability(0);
    setLoadingInProcess(false);
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

void Folder::setLoadingInProcess(const bool loadingInProcess)
{
    if (_loadingInProcess != loadingInProcess)
    {
        _loadingInProcess = loadingInProcess;

        emit loadingInProcessChanged();
    }
}

bool Folder::loadFromPath(const QString &path)
{
    reset();
    QDir dir(path);
    if (dir.exists())
    {
        setName(dir.dirName());

        foreach (QString filePath, dir.entryList(QDir::NoDotAndDotDot))
        {
            _filesModel->appendNew(filePath);
        }

        return true;
    }
    return false;
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

    QDomElement loadingInProcessElement = domElement.firstChildElement("loading");
    if (!loadingInProcessElement.isNull())
    {
        setLoadingInProcess(loadingInProcessElement.text().compare("true", Qt::CaseInsensitive) == 0);
    }
    else
    {
        setLoadingInProcess(false);
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

    QDomElement loadingInProcessElement = domDocument.createElement("loading");
    rootElement.appendChild(loadingInProcessElement);
    QDomText loadingInProcessText = domDocument.createTextNode(loadingInProcess()?"true":"false");
    loadingInProcessElement.appendChild(loadingInProcessText);

    _filesModel->toDomElement(rootElement, domDocument);

    return rootElement;
}
