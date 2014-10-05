#include "filevalidator.h"
#include <QImage>
#include <QDebug>

FileValidator::FileValidator(QObject *parent) :
    QObject(parent)
{
}

bool FileValidator::isImage(const QUrl &url)
{
    if (url.isEmpty()) {
        qWarning() << tr("URL is empty.");
        return false;
    }

    QImage img;
    return img.load(url.toLocalFile());
}

bool FileValidator::isVideo(const QUrl &url)
{
    //TODO: сделать валидатор видео
    return true;
}

bool FileValidator::isAudio(const QUrl &url)
{
    //TODO: сделать валидатор видео
    return true;
}
