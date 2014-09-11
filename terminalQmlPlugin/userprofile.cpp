#include "userprofile.h"
#include <QFile>
#include <QDebug>
#include <QBuffer>

UserProfile::UserProfile(QObject *parent) :
    QObject(parent),
    m_source(QUrl()),
    m_parentElement("user/info"),
    m_name(""),
    m_photo(QImage()),
    m_stateModel(new UserStatesModel(this)),
    m_preferredSize(QSize(800, 600))
{
}

void UserProfile::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

void UserProfile::setPhoto(QImage &photo)
{
    if (m_photo != photo) {
        m_photo = photo;
        emit photoChanged();
    }
}
QSize UserProfile::preferredSize() const
{
    return m_preferredSize;
}

void UserProfile::setPreferredSize(const QSize &preferredSize)
{
    if (m_preferredSize != preferredSize) {
        m_preferredSize = preferredSize;
        emit preferredSizeChanged();
    }
}


void UserProfile::setSource(const QUrl &source)
{
    if(m_source != source) {
        m_source = source;
        emit sourceChanged();

        reload();
    }
}

void UserProfile::setParentElement(const QString &parentElement)
{
    if (m_parentElement != parentElement) {
        m_parentElement = parentElement;
        emit parentElementChanged();

        reload();
    }
}

bool UserProfile::reload()
{
    QDomDocument domDoc;
    QDomElement rootElement = findElement(this, source(), parentElement(), domDoc);
    if (!rootElement.isNull()) {
        QDomElement nameElement = rootElement.firstChildElement("name");
        if (!nameElement.isNull()) {
            setName(nameElement.text());
        } else {
            setName("");
        }

        QImage nullImage;
        QDomElement photoElement = rootElement.firstChildElement("photo");
        if (!photoElement.isNull()) {
            QByteArray photoData = QByteArray::fromBase64(photoElement.text().toLocal8Bit());
            if (!photoData.isEmpty()) {
                QImage photo;
                if(photo.loadFromData(photoData)) {
                    setPhoto(photo);
                } else {
                    setPhoto(nullImage);
                }
            } else {
                setPhoto(nullImage);
            }
        } else {
            setPhoto(nullImage);
        }

        QDomElement statusesElement = rootElement.firstChildElement("statuses");
        if (!statusesElement.isNull()) {
            m_stateModel->loadFromDomElement(statusesElement);
        } else {
            m_stateModel->removeRows(0, m_stateModel->rowCount());
        }
        return true;
    }
    setName("");
    QImage nullImage (QSize(1,1), QImage::Format_ARGB32);
    setPhoto(nullImage);
    m_stateModel->removeRows(0, m_stateModel->rowCount());

    return false;
}

bool UserProfile::save() const
{
    QDomDocument domDoc;
    QDomElement rootElement = createElement(this, source(), parentElement(), domDoc);
    if (!rootElement.isNull()) {
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
        if (!photo().isNull()) {
            buffer.open(QIODevice::WriteOnly);
            //Смаштабируем фото
            if (photo().width() > m_preferredSize.width() || photo().height() > m_preferredSize.height()) {
                QImage scaledPhoto = photo().scaled(m_preferredSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                scaledPhoto.save(&buffer, "PNG");
            } else {
                photo().save(&buffer, "PNG");
            }
        }
        QDomText photoText = domDoc.createTextNode(ba.toBase64());
        photoElement.appendChild(photoText);

        QDomElement statusesElement = domDoc.createElement("statuses");
        rootElement.appendChild(statusesElement);
        m_stateModel->appendItemsToDomElement(statusesElement, domDoc);

        QFile file (source().toLocalFile());
        if(file.open(QFile::WriteOnly)) {
            QTextStream ts(&file);
            ts.setCodec("UTF-8");
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
