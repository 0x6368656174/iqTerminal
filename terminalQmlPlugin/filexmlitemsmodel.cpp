#include "filexmlitemsmodel.h"

FileXmlItemsModel::FileXmlItemsModel(QObject *parent) :
    AbstractXmlItemsModel(parent),
    m_source(QUrl()),
    m_parentElement("")
{
}

QUrl FileXmlItemsModel::source() const
{
    return m_source;
}

void FileXmlItemsModel::setSource(const QUrl &source)
{
    if(m_source != source) {
        m_source = source;

        emit sourceChanged();

        if (!parentElement().isEmpty())
            reload();
    }
}

QString FileXmlItemsModel::parentElement() const
{
    return m_parentElement;
}

void FileXmlItemsModel::setParentElement(const QString &parentElement)
{
    if (m_parentElement != parentElement) {
        m_parentElement = parentElement;

        emit parentElementChanged();

        if (source().isValid())
            reload();
    }
}

bool FileXmlItemsModel::reload()
{
    return reloadModel(this, source(), parentElement());
}

bool FileXmlItemsModel::save()
{
    return saveModel(this, source(), parentElement());
}
