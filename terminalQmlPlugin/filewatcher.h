#ifndef FILEWATCHER_H
#define FILEWATCHER_H

#include <QFileSystemWatcher>
#include <QUrl>

class FileWatcher : public QFileSystemWatcher
{
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
public:
    explicit FileWatcher(QObject *parent = 0);

    Q_INVOKABLE bool exist();

public:
    QString file() const;
    void setFile(const QUrl &file);

signals:
    void created();
    void deleted();
    void fileChanged();

private slots:
    void checkIsFileExist();

private:
    QString m_file;
    bool m_existed;
};

#endif // FILEWATCHER_H
