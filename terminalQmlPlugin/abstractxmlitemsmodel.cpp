#include "abstractxmlitemsmodel.h"
#include <QQmlContext>
#include <QQmlEngine>

AbstractXmlItemsModel::AbstractXmlItemsModel(QObject *parent) :
    QAbstractListModel(parent),
    _itemAdditionalData(NULL),
    _itemTagName("")
{
}

void AbstractXmlItemsModel::setItemAdditionalData(QQmlComponent *folderAdditionalData)
{
    if (_itemAdditionalData != folderAdditionalData)
    {
        _itemAdditionalData = folderAdditionalData;

        emit itemAdditionalDataChanged();
    }
}

bool AbstractXmlItemsModel::loadFromDomElement(const QDomElement &domElement)
{
    if (domElement.isNull())
        return false;

    QDomElement itemElement = domElement.firstChildElement(itemTagName());

    //Удалим все итемы, которых нет в новом XML-файле
    QList<qint64> itemIds;
    while (!itemElement.isNull())
    {
        qint64 itemId = itemElement.attribute("id", "-1").toLongLong();
        if (itemId != -1)
        {
            itemIds << itemId;
        }
        itemElement = itemElement.nextSiblingElement(itemTagName());
    }
    for (int i = rowCount() - 1; i > -1; i--)
    {
        AbstractXmlItemObject * item = get(i);
        if (itemIds.contains(item->id()))
            continue;

        removeRow(i);
    }

    //Пройдемся по новому XML-файлу
    itemElement = domElement.firstChildElement(itemTagName());
    int row = 0;
    while (!itemElement.isNull())
    {
        qint64 itemId = itemElement.attribute("id", "-1").toLongLong();
        if (itemId != -1)
        {
            AbstractXmlItemObject *oldItem = find(itemId);
            if (!oldItem)
            {
                //Создадим новый
                insertRow(row);
                AbstractXmlItemObject *newItem = get(row);
                newItem->loadFromDomElement(itemElement);
            }
            else
            {
                //Перезагрузим старый
                oldItem->loadFromDomElement(itemElement);

                //Переместим его, если он вдруг передвинулся
                int oldItemRow = _items.indexOf(oldItem);
                if (oldItemRow != row)
                {
                    emit beginMoveRows(QModelIndex(), oldItemRow, oldItemRow, QModelIndex(), row);
                    _items.move(oldItemRow, row);
                    emit endMoveRows();
                }
            }
            row++;
        }
        itemElement = itemElement.nextSiblingElement(itemTagName());
    }

    return true;
}

void AbstractXmlItemsModel::itemDataChanged()
{
    AbstractXmlItemObject *item = qobject_cast<AbstractXmlItemObject *>(sender());
    if (item)
    {
        int itemRow = _items.indexOf(item);
        if (itemRow > -1)
            emit dataChanged(index(itemRow, 0), index(itemRow, 0));
    }
}

AbstractXmlItemObject * AbstractXmlItemsModel::find(const qint64 id) const
{
    foreach (AbstractXmlItemObject * item, _items)
    {
        if (item && item->id() == id)
            return item;
    }

    return NULL;
}

void AbstractXmlItemsModel::appendItemsToDomElement(QDomElement &rootElement, QDomDocument &domDocument) const
{
    foreach (AbstractXmlItemObject *item, _items)
    {
        rootElement.appendChild(item->toDomElement(domDocument));
    }
}

AbstractXmlItemObject * AbstractXmlItemsModel::appendNew()
{
    return insertNew(rowCount());
}

AbstractXmlItemObject * AbstractXmlItemsModel::insertNew(int row)
{
    insertRow(row);
    AbstractXmlItemObject *newItem = get(row);
    return newItem;
}

bool AbstractXmlItemsModel::remove(int row)
{
    return removeRow(row);
}

bool AbstractXmlItemsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count-1);
    for (int i = 0; i < count; i++)
    {
        AbstractXmlItemObject *item = newItem();
        item->setId(newId());

        if (_itemAdditionalData)
        {
            QQmlContext *qmlContext = QQmlEngine::contextForObject(this);

            QObject *newItemAdditionalData = _itemAdditionalData->create(qmlContext);
            item->setAdditionalData(newItemAdditionalData);
        }

        _items.insert(row + i, item);
    }
    endInsertRows();
    emit countChanged();
    return true;
}

bool AbstractXmlItemsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    for (int i = row + count - 1; i > row - 1; i--)
    {
        AbstractXmlItemObject *item = get(i);
        item->deleteLater();
        _items.removeAt(i);
    }
    endRemoveRows();
    emit countChanged();
    return true;
}

qint64 AbstractXmlItemsModel::newId() const
{
    qint64 result = -1;
    foreach (AbstractXmlItemObject *item, _items)
    {
        if (item->id() > result)
            result = item->id();
    }
    return result + 1;
}

AbstractXmlItemObject * AbstractXmlItemsModel::get(const int row) const
{
    if (row < 0 || row > rowCount() - 1)
        return NULL;

    return _items.at(row);
}

QString AbstractXmlItemsModel::itemTagName() const
{
    if (!_itemTagName.isEmpty())
    {
        return _itemTagName;
    }
    else
    {
        AbstractXmlItemObject *newObject = const_cast<AbstractXmlItemsModel *>(this)->newItem();
        _itemTagName = newObject->tagName();
        delete newObject;
        return _itemTagName;
    }
    return "";
}
