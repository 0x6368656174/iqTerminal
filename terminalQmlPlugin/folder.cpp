#include "folder.h"
#include <QDir>
#include <QDebug>

Folder::Folder(QObject *parent) :
    AbstractXmlItemObject(parent),
    _filesModel(NULL),
    _name("")
{
}

void Folder::reset()
{
    AbstractXmlItemObject::reset();
    setName("");
    filesModel()->removeRows(0, filesModel()->rowCount());
}

QString Folder::tagName() const
{
    return "folder";
}

AbstractXmlItemsModel * Folder::newFilesModel() const
{
    return new FilesModel(const_cast<Folder *>(this));
}

FilesModel *Folder::filesModel() const
{
    if (_filesModel)
    {
        return _filesModel;
    }
    else
    {
        _filesModel = qobject_cast<FilesModel *>(newFilesModel());
        return _filesModel;
    }
    return NULL;
}

void Folder::setName(const QString &name)
{
    if (_name != name)
    {
        _name = name;

        emit nameChanged();
    }
}

bool Folder::loadFromDomElement(const QDomElement &domElement)
{
    if (AbstractXmlItemObject::loadFromDomElement(domElement))
    {
        QDomElement nameElement = domElement.firstChildElement("name");
        if (!nameElement.isNull())
        {
            setName(nameElement.text());
        }
        else
        {
            setName("");
        }

        return filesModel()->loadFromDomElement(domElement);
    }
    return false;
}

QDomElement Folder::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = AbstractXmlItemObject::toDomElement(domDocument);

    QDomElement nameElement = domDocument.createElement("name");
    rootElement.appendChild(nameElement);
    QDomText nameText = domDocument.createTextNode(name());
    nameElement.appendChild(nameText);

    filesModel()->appendItemsToDomElement(rootElement, domDocument);

    return rootElement;
}

bool Folder::loadFromPath(const QUrl &path)
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

void Folder::loadFromDir(const QUrl &path)
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
                    filesModel()->appendNew(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
                }
            }
        }
    }
}
