#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <QObject>

class FileSystem : public QObject
{
    Q_OBJECT
public:
    explicit FileSystem(QObject *parent = 0);

    Q_INVOKABLE static bool cp(const QUrl &source, const QUrl &destination);
};

#endif // FILESYSTEM_H
