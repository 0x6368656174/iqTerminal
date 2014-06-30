#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QUrl>

class Core : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl dataDir READ dataDir CONSTANT)
    Q_PROPERTY(QString dataDirPath READ dataDirPath CONSTANT)
    Q_PROPERTY(OS operatingSystem READ operatingSystem CONSTANT)
    Q_ENUMS(OS)
public:
    enum OS {
        Windows,
        Linux,
        Unix,
        Android,
        Mac
    };

    explicit Core(QObject *parent = 0);

    Q_INVOKABLE qreal dp(const qreal px) const;

public:
    OS operatingSystem() const;
    QUrl dataDir() const;
    QString dataDirPath() const;
};

#endif // CORE_H
