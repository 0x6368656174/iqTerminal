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
    qint32 sidsAvailability() const;
    void setSidsAvailability(const qint32 sidsAvailability);

    bool inProcess() const;
    void setInProcess(const bool inProcess);

    qint64 size() const;

    qint64 downloadedSize() const;

signals:
    void sidsAvailabilityChanged();
    void inProcessChanged();
    void sizeChanged();
    void downloadedSizeChanged();

private slots:
    void updateSize();
    void updateDownloadedSize();

private:
    qint32 m_sidsAvailability;
    bool m_inProcess;
    qint64 m_size;
    qint64 m_downloadedSize;

    void setSize(const qint64 size);
    void setDownloadedSize(const qint64 downloadedSize);
};

#endif // TORRENTFOLDER_H
