#include "userprofile.h"
#include <QFile>
#include <QDebug>
#include <QBuffer>

UserProfile::UserProfile(QObject *parent) :
    QObject(parent),
    _source(QUrl()),
    _parentElement("user/info"),
    _name(""),
    _photo(QImage()),
    _stateModel(new UserStatesModel(this))
{
}

void UserProfile::setName(const QString &name)
{
    if (_name != name)
    {
        _name = name;

        emit nameChanged();
    }
}

void UserProfile::setPhoto(QImage &photo)
{
    if (_photo != photo)
    {
        _photo = photo;

        emit photoChanged();
    }
}

void UserProfile::setSource(const QUrl &source)
{
    if(_source != source)
    {
        _source = source;

        emit sourceChanged();

        reload();
    }
}

void UserProfile::setParentElement(const QString &parentElement)
{
    if (_parentElement != parentElement)
    {
        _parentElement = parentElement;

        emit parentElementChanged();

        reload();
    }
}

bool UserProfile::reload()
{
    QDomDocument domDoc;
    QDomElement rootElement = findElement(this, source(), parentElement(), domDoc);
    if (!rootElement.isNull())
    {
        QDomElement nameElement = rootElement.firstChildElement("name");
        if (!nameElement.isNull())
        {
            setName(nameElement.text());
        }
        else
        {
            setName("");
        }

        QImage nullImage;
        QDomElement photoElement = rootElement.firstChildElement("photo");
        if (!photoElement.isNull())
        {
            QByteArray photoData = QByteArray::fromBase64(photoElement.text().toLocal8Bit());
            if (!photoData.isEmpty())
            {
                QImage photo;
                if(photo.loadFromData(photoData))
                {
                    setPhoto(photo);
                }
                else
                {
                    setPhoto(nullImage);
                }
            }
            else
            {
                setPhoto(nullImage);
            }
        }
        else
        {
            setPhoto(nullImage);
        }

        QDomElement statusesElement = rootElement.firstChildElement("statuses");
        if (!statusesElement.isNull())
        {
            _stateModel->loadFromDomElement(statusesElement);
        }
        else
        {
            _stateModel->removeRows(0, _stateModel->rowCount());
        }
        return true;
    }
    setName("");
    QImage nullImage (QSize(1,1), QImage::Format_ARGB32);
    setPhoto(nullImage);
    _stateModel->removeRows(0, _stateModel->rowCount());

    return false;
}

bool UserProfile::save() const
{
    QDomDocument domDoc;
    QDomElement rootElement = createElement(this, source(), parentElement(), domDoc);
    if (!rootElement.isNull())
    {
        //Пересоздадим основной элемент
        QDomElement oldRootElement = rootElement;
        rootElement = domDoc.createElement(rootElement.tagName());
        oldRootElement.parentNode().appendChild(rootElement);
        oldRootElement.parentNode().removeChild(oldRootElement);


        QDomElement nameElement = domDoc.createElement("name");
        rootElement.appendChild(nameElement);
        QDomText nameText = domDoc.createTextNode(name());
        nameElement.appendChild(nameText);

        QDomElement photoElement = domDoc.createElement("photo");
        rootElement.appendChild(photoElement);
        QByteArray ba;
        QBuffer buffer(&ba);
        if (!photo().isNull())
        {
            buffer.open(QIODevice::WriteOnly);
            photo().save(&buffer, "PNG");
        }
        QDomText photoText = domDoc.createTextNode(ba.toBase64());
        photoElement.appendChild(photoText);

        QDomElement statusesElement = domDoc.createElement("statuses");
        rootElement.appendChild(statusesElement);
        _stateModel->appendItemsToDomElement(statusesElement, domDoc);

        QFile file (source().toLocalFile());
        if(file.open(QFile::WriteOnly))
        {
            QTextStream ts(&file);
            ts << domDoc.toString();
            file.close();

            return true;
        }
    }

    return false;
}

void UserProfile::removePhoto()
{
    QImage nullImage;
    setPhoto(nullImage);
}

void UserProfile::setPhoto(const QUrl &filePath)
{
    QImage photo (filePath.toLocalFile());
    setPhoto(photo);
}

bool UserProfile::photoIsNull() const
{
    return photo().isNull();
}
