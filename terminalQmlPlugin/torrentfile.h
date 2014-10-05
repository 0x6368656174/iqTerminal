#ifndef TORRENTFILE_H
#define TORRENTFILE_H

#include <QObject>
#include <QDomElement>
#include "file.h"

class TorrentFile : public File
{
    Q_OBJECT
    Q_PROPERTY(qint64 size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(qint64 downloadedSize READ downloadedSize WRITE setDownloadedSize NOTIFY downloadedSizeChanged)
    Q_PROPERTY(QString hash READ hash WRITE setHash NOTIFY hashChanged)
public:
    explicit TorrentFile(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual bool loadFromPath(const QUrl &path);

public:
    qint64 size() const;
    void setSize(const qint64 size);

    qint64 downloadedSize() const;
    void setDownloadedSize(const qint64 downloadedSize);

    QString hash() const;
    void setHash(const QString &hash);

protected:
    virtual void reset();

signals:
    void sizeChanged();
    void downloadedSizeChanged();
    void hashChanged();

private:
    qint64 m_size;
    qint64 m_downloadedSize;
    QString m_hash;
};

#endif // TORRENTFILE_H
