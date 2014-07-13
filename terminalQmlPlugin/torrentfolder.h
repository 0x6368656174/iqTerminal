#ifndef TORRENTFOLDER_H
#define TORRENTFOLDER_H

#include <QObject>
#include "folder.h"
#include <QDomElement>

class TorrentFolder : public Folder
{
    Q_OBJECT
    Q_PROPERTY(qint32 sidsAvailability READ sidsAvailability WRITE setSidsAvailability NOTIFY sidsAvailabilityChanged)
    Q_PROPERTY(bool inProcess READ inProcess WRITE setInProcess NOTIFY inProcessChanged)
    Q_PROPERTY(qint64 size READ size NOTIFY sizeChanged)
    Q_PROPERTY(qint64 downloadedSize READ downloadedSize NOTIFY downloadedSizeChanged)

public:
    explicit TorrentFolder(QObject *parent = 0);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

protected:
    virtual AbstractXmlItemsModel * newFilesModel() const;

    virtual void reset();

public:
    inline qint32 sidsAvailability() const {return _sidsAvailability;}
    void setSidsAvailability(const qint32 sidsAvailability);

    inline bool inProcess() const {return _inProcess;}
    void setInProcess(const bool inProcess);

    inline qint64 size() const {return _size;}

    inline qint64 downloadedSize() const {return _downloadedSize;}

signals:
    void sidsAvailabilityChanged();
    void inProcessChanged();
    void sizeChanged();
    void downloadedSizeChanged();

private slots:
    void updateSize();
    void updateDownloadedSize();

private:
    qint32 _sidsAvailability;
    bool _inProcess;
    qint64 _size;
    qint64 _downloadedSize;

    void setSize(const qint64 size);
    void setDownloadedSize(const qint64 downloadedSize);
};

#endif // TORRENTFOLDER_H
