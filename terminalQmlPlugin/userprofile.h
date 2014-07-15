#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <QObject>
#include <QImage>
#include "userstatesmodel.h"
#include "xmlinterface.h"

class UserProfile : public QObject, public XmlInterface
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QImage photo READ photo WRITE setPhoto NOTIFY photoChanged)
    Q_PROPERTY(UserStatesModel * stateModel READ stateModel CONSTANT)

public:
    explicit UserProfile(QObject *parent = 0);

    Q_INVOKABLE bool reload();

    Q_INVOKABLE bool save() const;

    Q_INVOKABLE void setPhoto(const QUrl &filePath);

    Q_INVOKABLE void removePhoto();

    Q_INVOKABLE bool photoIsNull() const;

public:
    inline QUrl source() const {return _source;}
    void setSource(const QUrl &source);

    inline QString parentElement() const {return _parentElement;}
    void setParentElement(const QString &parentElement);

    inline QString name() const {return _name;}
    void setName(const QString &name);

    inline QImage photo() const {return _photo;}
    void setPhoto(QImage &photo);

    inline UserStatesModel *stateModel() const {return _stateModel;}

signals:
    void sourceChanged();
    void parentElementChanged();
    void nameChanged();
    void photoChanged();

private:
    QUrl _source;
    QString _parentElement;
    QString _name;
    QImage _photo;
    UserStatesModel *_stateModel;
};

#endif // USERPROFILE_H
