#ifndef TEXTDECORATOR_H
#define TEXTDECORATOR_H

#include <QObject>
#include <QHash>

class TextDecorator : public QObject
{
    Q_OBJECT
public:
    explicit TextDecorator(QObject *parent = 0);

    Q_INVOKABLE static QString toPlainText(const QString &str);

    Q_INVOKABLE static QString toFormattedText(const QString &str);

private:
    static QHash<QString, QString> _smiles;

    static void initSmiles();
};

#endif // TEXTDECORATOR_H
