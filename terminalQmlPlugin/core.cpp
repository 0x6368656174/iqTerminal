#include "core.h"
#include <QDir>
#include <QStandardPaths>

Core::Core(QObject *parent) :
    QObject(parent)
{
}

qreal Core::dp(const qreal px) const
{
#ifdef Q_OS_ANDROID
    return px*3;
#else
    return px*2;
#endif
}

QUrl Core::dataDir()
{
    return QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/");
}

QString Core::dataDirPath() const
{
    return dataDir().path();
}

Core::OS Core::operatingSystem() const
{
#ifdef Q_OS_ANDROID
    return Android;
#endif
#ifdef Q_OS_MAC
    return Mac;
#endif
#ifdef Q_OS_LINUX
    return Linux;
#endif
#ifdef Q_OS_UNIX
    return Unix;
#endif
#ifdef Q_OS_WIN
    return Windows;
#endif
}

QString Core::humanReadableSize(const qint64 size)
{
    qreal bytes = size;
    int thresh = 1024;
    if (bytes < thresh)
        return QString("%0 B").arg(bytes);
    QStringList units = QStringList() << tr("KiB") << tr("MiB") << tr("GiB")
                                      << tr("TiB") << tr("PiB") << tr("EiB")
                                      << tr("ZiB") << tr("YiB");
    int u = -1;
    do
    {
        bytes /= thresh;
        u++;
    }
    while(bytes >= thresh);

    return QString("%0 %1").arg(bytes, 0, 'f', 1).arg(units[u]);
}
