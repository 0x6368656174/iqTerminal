#include "terminalimageprovider.h"
#include <QImage>
#include <QDomDocument>
#include <QFile>
#include <QDebug>

TerminalImageProvider::TerminalImageProvider():
    QQuickImageProvider(QQmlImageProviderBase::Image)
{
}

QImage TerminalImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "Request image with id: " << id;
    Q_UNUSED(requestedSize);
    if (id.right(6) == "/i.xml")
    {
        QString osValidId = id;
#ifdef Q_OS_WIN
        while (osValidId.left(1) == "\\" || osValidId.left(1) == "/")
        {
            osValidId = osValidId.mid(1);
        }
#endif
        if (!QFile::exists(osValidId))
        {
            qWarning() << QObject::tr("File %0 not found.").arg(osValidId);
            return QImage(QSize(1,1), QImage::Format_ARGB32);
        }
        QFile file (osValidId);
        if (!file.open(QFile::ReadOnly))
        {
            qWarning() << QObject::tr("Unable to open the file %0.").arg(osValidId);
            return QImage(QSize(1,1), QImage::Format_ARGB32);
        }

        QDomDocument domDoc;

        QString errorMessage;
        if (!domDoc.setContent(&file, &errorMessage))
        {
            qWarning() << QObject::tr("Error on parse \"%0\": \"%1\".").arg(id).arg(errorMessage);
            file.close();
            return QImage(QSize(1,1), QImage::Format_ARGB32);
        }
        file.close();

        QDomElement photoElement = domDoc.documentElement().firstChildElement("info").firstChildElement("photo");
        if (photoElement.isNull())
        {
            qWarning() << QObject::tr("Not found <info><photo> in \"%0\".").arg(id);
            return QImage(QSize(1,1), QImage::Format_ARGB32);
        }

        QByteArray imgData = QByteArray::fromBase64(photoElement.text().toLocal8Bit());
        if (imgData.isEmpty())
        {
            return QImage(QSize(1,1), QImage::Format_ARGB32);
        }

        QImage img;
        img.loadFromData(imgData);

        if (size)
        {
            size->setHeight(img.height());
            size->setWidth(img.width());
        }

        return img;
    }
    return QImage();
}
