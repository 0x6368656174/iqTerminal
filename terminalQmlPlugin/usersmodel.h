#ifndef USERSMODEL_H
#define USERSMODEL_H

#include "abstractxmlitemsmodel.h"
#include "xmlinterface.h"
#include "user.h"

class UsersModel : public AbstractXmlItemsModel, public XmlInterface
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)
public:
    enum Roles
    {
        Id = Qt::UserRole,
        AdditionalData,
        Profile,
        Online,
        FriendshipAccepted
    };

    explicit UsersModel(QObject *parent = 0);

    Q_INVOKABLE bool reload();

    Q_INVOKABLE bool save();

public:
    virtual inline QHash<int, QByteArray> roleNames() const {return _roles;}

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

public:
    inline QUrl source() const {return _source;}
    void setSource(const QUrl &source);

    inline QString parentElement() const {return _parentElement;}
    void setParentElement(const QString &parentElement);

signals:
    void sourceChanged();
    void parentElementChanged();

private:
    QHash<int, QByteArray> _roles;
    QUrl _source;
    QString _parentElement;
};

#endif // USERSMODEL_H
