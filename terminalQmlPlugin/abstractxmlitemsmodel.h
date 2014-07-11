#ifndef ABSTRACTXMLITEMSMODEL_H
#define ABSTRACTXMLITEMSMODEL_H

#include <QAbstractListModel>
#include "abstractxmlitemobject.h"
#include <QQmlComponent>

class AbstractXmlItemsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QQmlComponent* itemAdditionalData READ itemAdditionalData WRITE setItemAdditionalData NOTIFY itemAdditionalDataChanged)
public:
    explicit AbstractXmlItemsModel(QObject *parent = 0);

    Q_INVOKABLE AbstractXmlItemObject * get(const int row) const;

    Q_INVOKABLE AbstractXmlItemObject * find(const qint64 id) const;

    Q_INVOKABLE AbstractXmlItemObject * appendNew();

    Q_INVOKABLE AbstractXmlItemObject * insertNew(int row);

    Q_INVOKABLE bool remove(int row);

    bool loadFromDomElement(const QDomElement &domElement);

    void toDomElement(QDomElement &rootElement, QDomDocument &domDocument) const;

public:
    virtual inline int rowCount(const QModelIndex &parent = QModelIndex()) const {Q_UNUSED(parent); return _items.count();}

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

public:
    int count() const {return rowCount();}

    inline QQmlComponent *itemAdditionalData() const {return _itemAdditionalData;}
    void setItemAdditionalData(QQmlComponent *itemAdditionalData);

signals:
    void countChanged();
    void itemAdditionalDataChanged();

protected:
    virtual AbstractXmlItemObject * newItem() = 0;

    virtual QString itemTagName() const = 0;

protected slots:
    void itemDataChanged();

private:
    QList<AbstractXmlItemObject *> _items;
    QQmlComponent *_itemAdditionalData;

    qint64 newId() const;

};

#endif // ABSTRACTXMLITEMSMODEL_H
