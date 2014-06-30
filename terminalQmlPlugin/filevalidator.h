#ifndef FILEVALIDATOR_H
#define FILEVALIDATOR_H

#include <QObject>
#include <QUrl>

class FileValidator : public QObject
{
    Q_OBJECT
public:
    explicit FileValidator(QObject *parent = 0);

    Q_INVOKABLE bool isImage(const QUrl &url);

    Q_INVOKABLE bool isVideo(const QUrl &url);

    Q_INVOKABLE bool isAudio(const QUrl &url);
};

#endif // FILEVALIDATOR_H
