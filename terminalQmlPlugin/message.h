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
    inline QString text() const {return _text;}
    void setText(const QString &text);

    inline Type type() const {return _type;}
    void setType(const Type type);

    inline Direction direction() const {return _direction;}
    void setDirection(const Direction direction);

    inline bool wasRead() const {return _wasRead;}
    void setWasRead(const bool wasRead);

    inline QUrl filePath() const {return _filePath;}
    void setFilePath(const QUrl &filePath);

    inline QDateTime sendDateTime() const {return _sendDateTime;}
    void setSendDateTime(const QDateTime &dateTime);

signals:
    void textChanged();
    void typeChanged();
    void directionChanged();
    void wasReadChanged();
    void filePathChanged();
    void sendDateTimeChanged();

private:
    QString _text;
    Type _type;
    Direction _direction;
    bool _wasRead;
    QUrl _filePath;
    QDateTime _sendDateTime;
};

#endif // MESSAGE_H
