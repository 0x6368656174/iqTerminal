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
    Q_PROPERTY(QSize preferredSize READ preferredSize WRITE setPreferredSize NOTIFY preferredSizeChanged)
    Q_PROPERTY(int id READ id WRITE setId NOTIFY idChanged)

public:
    explicit UserProfile(QObject *parent = 0);

    Q_INVOKABLE bool reload();

    Q_INVOKABLE bool save() const;

    Q_INVOKABLE void setPhoto(const QUrl &filePath);

    Q_INVOKABLE void removePhoto();

    Q_INVOKABLE bool photoIsNull() const;

public:
    QUrl source() const;
    void setSource(const QUrl &source);
    void resetSource();

    QString parentElement() const;
    void setParentElement(const QString &parentElement);

    QString name() const;
    void setName(const QString &name);

    QImage photo() const;
    void setPhoto(QImage &photo);

    UserStatesModel *stateModel() const;

    QSize preferredSize() const;
    void setPreferredSize(const QSize &preferredSize);

    int id() const;
    void setId(int id);

signals:
    void sourceChanged();
    void parentElementChanged();
    void nameChanged();
    void photoChanged();
    void preferredSizeChanged();
    void idChanged();

private:
    QUrl m_source;
    QString m_parentElement;
    QString m_name;
    QImage m_photo;
    UserStatesModel *m_stateModel;
    QSize m_preferredSize;
    int m_id;
};

#endif // USERPROFILE_H
