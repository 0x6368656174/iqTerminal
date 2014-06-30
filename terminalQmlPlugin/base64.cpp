#include "base64.h"
#include <QByteArray>
#include <QFile>
#include <QDebug>

Base64::Base64(QObject *parent) :
    QObject(parent)
{
}

QString Base64::toBase64(const QString &data) const
{
    return data.toLocal8Bit().toBase64();
}

QString Base64::fromBase64(const QString &data) const
{
    return QByteArray::fromBase64(data.toLocal8Bit());
}

QString Base64::fileToBase64(const QUrl &url)
{
    if (url.isEmpty())
    {
        qWarning() << tr("URL is empty.");
        return QString();
    }

    QFile file(url.path());
    if ( file.open(QIODevice::ReadOnly) )
    {
        QByteArray fileContent = file.readAll();

        file.close();

        return QString(fileContent.toBase64());
    }
    else
    {
        qWarning() <<tr ("Unable to open the file %0.").arg(url.path());
        return QString();
    }
    return "";
}
