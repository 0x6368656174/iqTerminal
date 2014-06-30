#ifndef BASE64_H
#define BASE64_H

#include <QObject>
#include <QUrl>

class Base64 : public QObject
{
    Q_OBJECT
public:
    explicit Base64(QObject *parent = 0);

    Q_INVOKABLE QString fileToBase64(const QUrl &url);

    Q_INVOKABLE QString fromBase64(const QString &data) const;

    Q_INVOKABLE QString toBase64(const QString &data) const;
};

#endif // BASE64_H
