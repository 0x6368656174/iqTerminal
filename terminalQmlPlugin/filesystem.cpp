#include "filesystem.h"
#include <QFile>
#include <QUrl>
#include <QDebug>
#include <QFileInfo>

FileSystem::FileSystem(QObject *parent) :
    QObject(parent)
{
}

bool FileSystem::cp(const QUrl &source, const QUrl &destination)
{
    if (!QFile::exists(source.toLocalFile())) {
        qWarning() << tr("File %0 not exists.").arg(source.toLocalFile());
        return false;
    }

    QFileInfo fi (destination.toLocalFile());
    if (fi.isDir()) {
        return QFile::copy(source.toLocalFile(), destination.toLocalFile() + "/" + source.fileName());
    }

    return QFile::copy(source.toLocalFile(), destination.toLocalFile());
}
