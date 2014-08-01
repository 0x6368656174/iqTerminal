#ifndef URLHELPER_H
#define URLHELPER_H

#include <QObject>
#include <QUrl>

class UrlHelper : public QObject
{
    Q_OBJECT
public:
    explicit UrlHelper(QObject *parent = 0);

    Q_INVOKABLE static QString fileName(const QUrl &url);

    Q_INVOKABLE static QString dirName(const QUrl &url);

    Q_INVOKABLE static QUrl containingFolder(const QUrl &url);

    Q_INVOKABLE static bool hasContainingFolder(const QUrl &url);

    Q_INVOKABLE static bool isDir(const QUrl &url);

    Q_INVOKABLE static int dirEntryCount(const QUrl &url);
};

#endif // URLHELPER_H
