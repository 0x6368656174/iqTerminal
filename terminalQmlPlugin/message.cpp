#include "message.h"
#include <QDebug>

Message::Message(QObject *parent) :
    AbstractXmlItemObject(parent),
    _text(""),
    _type(Text),
    _direction(Outgoing),
    _wasRead(false),
    _filePath(QUrl())
{
}

void Message::reset()
{
    AbstractXmlItemObject::reset();
    setText("");
    setType(Text);
    setDirection(Outgoing);
    setWasRead(false);
    setFilePath(QUrl());
}

QString Message::tagName() const
{
    return "message";
}

void Message::setText(const QString &text)
{
    if (_text != text)
    {
        _text = text;

        emit textChanged();
    }
}

void Message::setType(const Type type)
{
    if (_type != type)
    {
        _type = type;

        emit typeChanged();
    }
}

void Message::setDirection(const Direction direction)
{
    if (_direction != direction)
    {
        _direction = direction;

        emit directionChanged();
    }
}

void Message::setWasRead(const bool wasRead)
{
    if (_wasRead != wasRead)
    {
        _wasRead = wasRead;

        emit wasReadChanged();
    }
}
void Message::setFilePath(const QUrl &filePath)
{
    if (_filePath != filePath)
    {
        _filePath = filePath;

        emit filePathChanged();
    }
}

bool Message::loadFromDomElement(const QDomElement &domElement)
{
    if (AbstractXmlItemObject::loadFromDomElement(domElement))
    {
        QString typeName = domElement.attribute("type", "text");
        if (typeName == "text")
        {
            setType(Text);
        } else if (typeName == "image")
        {
            setType(Image);
        } else if (typeName == "audio")
        {
            setType(Audio);
        } else if (typeName == "video")
        {
            setType(Video);
        } else if (typeName == "file")
        {
            setType(File);
        } else
        {
            qWarning() << QString("Invalid message type \"%0\".").arg(typeName);
            setType(Text);
        }

        QString directionName = domElement.attribute("direction", "outgoing");
        if (directionName == "outgoing")
        {
            setDirection(Outgoing);
        } else if (directionName == "incoming")
        {
            setDirection(Incoming);
        } else
        {
            qWarning() << QString("Invalid message direction \"%0\".").arg(directionName);
            setDirection(Outgoing);
        }

        setWasRead(domElement.attribute("read", "false") == "true");

        setFilePath(QUrl(domElement.attribute("filePath", "")));

        setText(domElement.text().trimmed());

        return true;
    }

    return false;
}

QDomElement Message::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = AbstractXmlItemObject::toDomElement(domDocument);

    switch (type())
    {
    case Text:
        rootElement.setAttribute("type", "text");
        break;
    case Image:
        rootElement.setAttribute("type", "image");
        break;
    case Audio:
        rootElement.setAttribute("type", "audio");
        break;
    case Video:
        rootElement.setAttribute("type", "video");
        break;
    case File:
        rootElement.setAttribute("type", "file");
        break;
    }

    switch (direction())
    {
    case Outgoing:
        rootElement.setAttribute("direction", "outgoing");
        break;
    case Incoming:
        rootElement.setAttribute("direction", "incoming");
        break;
    }

    if (wasRead())
    {
        rootElement.setAttribute("read", "true");
    }
    else
    {
        rootElement.setAttribute("read", "false");
    }

    rootElement.setAttribute("filePath", filePath().toLocalFile());

    QDomText textText = domDocument.createTextNode(text());
    rootElement.appendChild(textText);

    return rootElement;
}
