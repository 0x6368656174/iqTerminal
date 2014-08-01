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
public:
    explicit TorrentFile(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

    virtual bool loadFromPath(const QUrl &path);

public:
    inline qint64 size() const {return _size;}
    void setSize(const qint64 size);

    inline qint64 downloadedSize() const {return _downloadedSize;}
    void setDownloadedSize(const qint64 downloadedSize);

protected:
    virtual void reset();

signals:
    void sizeChanged();
    void downloadedSizeChanged();

private:
    qint64 _size;
    qint64 _downloadedSize;
};

#endif // TORRENTFILE_H
