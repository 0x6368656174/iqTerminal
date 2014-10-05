#ifndef MESSAGESMODEL_H
#define MESSAGESMODEL_H

#include "abstractxmlitemsmodel.h"
#include "xmlinterface.h"

class MessagesModel : public AbstractXmlItemsModel, public XmlInterface
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString parentElement READ parentElement WRITE setParentElement NOTIFY parentElementChanged)

public:
    enum Roles
    {
        Id = Qt::UserRole,
        AdditionalData,
        Text,
        Type,
        Direction,
        WasRead,
        FilePath,
        SendDateTime
    };

    explicit MessagesModel(QObject *parent = 0);

    Q_INVOKABLE bool reload();

    Q_INVOKABLE bool save();

public:
    virtual QHash<int, QByteArray> roleNames() const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual AbstractXmlItemObject * newItem();

public:
    QUrl source() const;
    void setSource(const QUrl &source);

    QString parentElement() const;
    void setParentElement(const QString &parentElement);

signals:
    void sourceChanged();
    void parentElementChanged();

private:
    QHash<int, QByteArray> m_roles;
    QUrl m_source;
    QString m_parentElement;
};

#endif // MESSAGESMODEL_H
