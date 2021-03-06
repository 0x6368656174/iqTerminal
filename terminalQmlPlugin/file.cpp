#include "file.h"
#include <QFileInfo>
#include <QDebug>
#include <QUrl>

File::File(QObject *parent) :
    AbstractXmlItemObject(parent),
    m_path(""),
    m_name("")
{
}

void File::reset()
{
    AbstractXmlItemObject::reset();
    setPath("");
    setName("");
}

QString File::tagName() const
{
    return "file";
}

QString File::path() const
{
    return m_path;
}

void File::setPath(const QString &path)
{
    if (m_path != path) {
        m_path = path;
        emit pathChanged();
    }
}

QString File::name() const
{
    return m_name;
}

void File::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

bool File::loadFromPath(const QUrl &path)
{
    if (path.isValid()) {
        QFileInfo info (path.toLocalFile());
        if (info.exists()) {
            setName(info.fileName());
            setPath(path.toString());
            return true;
        }
    }
    return false;
}

bool File::loadFromDomElement(const QDomElement &domElement)
{
    if (AbstractXmlItemObject::loadFromDomElement(domElement)) {
        QDomElement pathElement = domElement.firstChildElement("path");
        if (!pathElement.isNull()) {
            setPath(pathElement.text());
        } else {
            setPath("");
        }

        QDomElement nameElement = domElement.firstChildElement("name");
        if (!nameElement.isNull()) {
            setName(nameElement.text());
        } else {
            setName("");
        }

        return true;
    }
    return false;
}

QDomElement File::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = AbstractXmlItemObject::toDomElement(domDocument);

    QDomElement pathElement = domDocument.createElement("path");
    rootElement.appendChild(pathElement);
    QDomText pathText = domDocument.createTextNode(path());
    pathElement.appendChild(pathText);

    QDomElement nameElement = domDocument.createElement("name");
    rootElement.appendChild(nameElement);
    QDomText nameText = domDocument.createTextNode(name());
    nameElement.appendChild(nameText);

    return rootElement;
}
