#ifndef TERMINALIMAGEPROVIDER_H
#define TERMINALIMAGEPROVIDER_H

#include <QQuickImageProvider>


class TerminalImageProvider : public QQuickImageProvider
{
public:
    TerminalImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
};

#endif // TERMINALIMAGEPROVIDER_H
