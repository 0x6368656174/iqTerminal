#include "message.h"
#include <QDebug>

Message::Message(QObject *parent) :
    AbstractXmlItemObject(parent),
    m_text(""),
    m_type(Text),
    m_direction(Outgoing),
    m_wasRead(false),
    m_filePath(QUrl())
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

QString Message::text() const
{
    return m_text;
}

void Message::setText(const QString &text)
{
    if (m_text != text) {
        m_text = text;

        emit textChanged();
    }
}

Message::Type Message::type() const
{
    return m_type;
}

void Message::setType(const Type type)
{
    if (m_type != type) {
        m_type = type;

        emit typeChanged();
    }
}

Message::Direction Message::direction() const
{
    return m_direction;
}

void Message::setDirection(const Direction direction)
{
    if (m_direction != direction) {
        m_direction = direction;

        emit directionChanged();
    }
}

bool Message::wasRead() const
{
    return m_wasRead;
}

void Message::setWasRead(const bool wasRead)
{
    if (m_wasRead != wasRead) {
        m_wasRead = wasRead;

        emit wasReadChanged();
    }
}

QUrl Message::filePath() const
{
    return m_filePath;
}

void Message::setFilePath(const QUrl &filePath)
{
    if (m_filePath != filePath) {
        m_filePath = filePath;

        emit filePathChanged();
    }
}

QDateTime Message::sendDateTime() const
{
    return m_sendDateTime;
}

void Message::setSendDateTime(const QDateTime &dateTime)
{
    if (m_sendDateTime != dateTime) {
        m_sendDateTime = dateTime;

        emit sendDateTimeChanged();
    }
}

bool Message::loadFromDomElement(const QDomElement &domElement)
{
    if (AbstractXmlItemObject::loadFromDomElement(domElement)) {
        QString typeName = domElement.attribute("type", "text");
        if (typeName == "text") {
            setType(Text);
        } else if (typeName == "image") {
            setType(Image);
        } else if (typeName == "audio") {
            setType(Audio);
        } else if (typeName == "video") {
            setType(Video);
        } else if (typeName == "file") {
            setType(File);
        } else {
            qWarning() << tr("Invalid message type \"%0\".").arg(typeName);
            setType(Text);
        }

        QString directionName = domElement.attribute("direction", "outgoing");
        if (directionName == "outgoing") {
            setDirection(Outgoing);
        } else if (directionName == "incoming") {
            setDirection(Incoming);
        } else {
            qWarning() << tr("Invalid message direction \"%0\".").arg(directionName);
            setDirection(Outgoing);
        }

        setWasRead(domElement.attribute("read", "false") == "true");

        QString filePathStr = domElement.attribute("file_path", "");
        if (!filePathStr.isEmpty()) {
            setFilePath(QUrl(filePathStr));
        } else {
            setFilePath(QUrl());
        }

        QString sendDateTimeStr = domElement.attribute("send_date_time", "");
        if (!sendDateTimeStr.isEmpty()) {
            setSendDateTime(QDateTime::fromString(sendDateTimeStr, Qt::ISODate));
        } else {
            setSendDateTime(QDateTime());
        }

        setText(domElement.text().trimmed());

        return true;
    }

    return false;
}

QDomElement Message::toDomElement(QDomDocument &domDocument) const
{
    QDomElement rootElement = AbstractXmlItemObject::toDomElement(domDocument);

    switch (type()) {
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

    switch (direction()) {
    case Outgoing:
        rootElement.setAttribute("direction", "outgoing");
        break;
    case Incoming:
        rootElement.setAttribute("direction", "incoming");
        break;
    }

    if (wasRead()) {
        rootElement.setAttribute("read", "true");
    } else {
        rootElement.setAttribute("read", "false");
    }

    rootElement.setAttribute("file_path", filePath().toString());

    rootElement.setAttribute("send_date_time", sendDateTime().toString(Qt::ISODate));

    QDomText textText = domDocument.createTextNode(text());
    rootElement.appendChild(textText);
    return rootElement;
}
