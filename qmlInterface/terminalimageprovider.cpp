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
    Q_UNUSED(requestedSize);

    QImage nullImage (QSize(1,1), QImage::Format_ARGB32);
    if (id.right(4) == ".xml") {
        QUrl path = QUrl(id);
        if (!QFile::exists(path.toLocalFile())) {
            qWarning() << QObject::tr("File %0 not found.").arg(path.toLocalFile());
            return nullImage;
        }
        QFile file (path.toLocalFile());
        if (!file.open(QFile::ReadOnly)) {
            qWarning() << QObject::tr("Unable to open the file %0.").arg(path.toLocalFile());
            return nullImage;
        }

        QDomDocument domDoc;

        QString errorMessage;
        if (!domDoc.setContent(&file, &errorMessage)) {
            qWarning() << QObject::tr("Error on parse \"%0\": \"%1\".").arg(id).arg(errorMessage);
            file.close();
            return nullImage;
        }
        file.close();

        if (domDoc.documentElement().tagName() == "user") {
            QDomElement photoElement = domDoc.documentElement().firstChildElement("info").firstChildElement("photo");
            if (photoElement.isNull()) {
                qWarning() << QObject::tr("Not found <info><photo> in \"%0\".").arg(id);
                return nullImage;
            }

            QByteArray imgData = QByteArray::fromBase64(photoElement.text().toLocal8Bit());
            if (imgData.isEmpty()) {
                return nullImage;
            }

            QImage img;
            img.loadFromData(imgData);

            if (size) {
                size->setHeight(img.height());
                size->setWidth(img.width());
            }

            return img;
        }
    }
    return nullImage;
}
