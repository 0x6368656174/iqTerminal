#include "urlhelper.h"
#include <QFileInfo>
#include <QDir>
#include <QDebug>

UrlHelper::UrlHelper(QObject *parent) :
    QObject(parent)
{
}

QString UrlHelper::fileName(const QUrl &url)
{
    QFileInfo fi (url.toLocalFile());
    return fi.fileName();
}

QString UrlHelper::dirName(const QUrl &url)
{
    QFileInfo fi (url.toLocalFile());
    QDir dir;
    if (fi.isDir())
    {
        dir.setPath(fi.filePath());
    }
    else
    {
        dir.setPath(fi.path());
    }
    if (dir.path() == "/")
        return "/";
#ifdef Q_OS_WIN
    if (dir.dirName() == "")
        return dir.absolutePath();
#endif
    return dir.dirName();
}

QUrl UrlHelper::containingFolder(const QUrl &url)
{
    QFileInfo fi (url.toLocalFile());
    QDir dir;
    if (fi.isDir())
    {
        dir.setPath(fi.filePath());
    }
    else
    {
        dir.setPath(fi.path());
    }
    dir.cdUp();
    if (dir.absolutePath() != "/")
    {
        return QUrl::fromLocalFile(dir.absolutePath() + "/");
    }
    else
    {
        return QUrl::fromLocalFile("/");
    }
}

bool UrlHelper::hasContainingFolder(const QUrl &url)
{
    QFileInfo fi (url.toLocalFile());
    QDir dir;
    if (fi.isDir())
    {
        dir.setPath(fi.filePath());
    }
    else
    {
        dir.setPath(fi.path());
    }
    return dir.cdUp();
}

bool UrlHelper::isDir(const QUrl &url)
{
    QFileInfo fi (url.toLocalFile());
    return fi.isDir();
}

int UrlHelper::dirEntryCount(const QUrl &url)
{
    QFileInfo fi (url.toLocalFile());
    QDir dir;

    dir.setPath(fi.absoluteFilePath() + "/");


    return dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot).count();
}
