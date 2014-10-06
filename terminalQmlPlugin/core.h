#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QUrl>

/*!
 * \defgroup Helpers Вспомогательные классы
 *
 * \brief Набор вспомогательных классов, расширяющих QML и C++
 *
 * Данная группа содержит набор вспомогательных классов, написаных на С++ и расширяющих С++ и QML.
 *
 * \ingroup TerminalQmlPlugin
 */


/*!
 * \brief Класс, опсиывающий основные функции
 *
 * Данный класс описывает основные функции, которые очень часто используются в QML и С++
 *
 * \ingroup Helpers
 */
class Core : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl dataDir READ dataDir CONSTANT)
    Q_PROPERTY(QString dataDirPath READ dataDirPath CONSTANT)
    Q_PROPERTY(QUrl homeDir READ homeDir CONSTANT)
    Q_PROPERTY(QUrl musicDir READ musicDir CONSTANT)
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

    Q_INVOKABLE static qreal dp(const qreal px);

    Q_INVOKABLE static QString humanReadableSize(const qint64 size);
public:
    static OS operatingSystem();
    static QUrl dataDir();
    static QUrl homeDir();
    static QUrl musicDir();
    static QString dataDirPath();
};

#endif // CORE_H
