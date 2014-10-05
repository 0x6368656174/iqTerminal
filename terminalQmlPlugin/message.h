#ifndef MESSAGE_H
#define MESSAGE_H

#include "abstractxmlitemobject.h"
#include <QUrl>
#include <QDateTime>

class Message : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(Direction direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(bool wasRead READ wasRead WRITE setWasRead NOTIFY wasReadChanged)
    Q_PROPERTY(QUrl filePath READ filePath WRITE setFilePath NOTIFY filePathChanged)
    Q_PROPERTY(QDateTime sendDateTime READ sendDateTime WRITE setSendDateTime NOTIFY sendDateTimeChanged)

    Q_ENUMS (Type)
    Q_ENUMS (Direction)

public:
    enum Type {
        Text,
        Image,
        Audio,
        Video,
        File
    };

    enum Direction {
        Outgoing,
        Incoming
    };

    explicit Message(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual QString tagName() const;

protected:
    virtual void reset();


public:
    QString text() const;
    void setText(const QString &text);

    Type type() const;
    void setType(const Type type);

    Direction direction() const;
    void setDirection(const Direction direction);

    bool wasRead() const;
    void setWasRead(const bool wasRead);

    QUrl filePath() const;
    void setFilePath(const QUrl &filePath);

    QDateTime sendDateTime() const;
    void setSendDateTime(const QDateTime &dateTime);

signals:
    void textChanged();
    void typeChanged();
    void directionChanged();
    void wasReadChanged();
    void filePathChanged();
    void sendDateTimeChanged();

private:
    QString m_text;
    Type m_type;
    Direction m_direction;
    bool m_wasRead;
    QUrl m_filePath;
    QDateTime m_sendDateTime;
};

#endif // MESSAGE_H
