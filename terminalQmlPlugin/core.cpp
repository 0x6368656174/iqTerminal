#include "core.h"
#include <QDir>

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

QUrl Core::dataDir() const
{
    return QUrl::fromLocalFile(QDir::homePath() + "/.terminalData/");
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
