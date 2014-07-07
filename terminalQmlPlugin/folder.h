#ifndef FOLDER_H
#define FOLDER_H

#include <QObject>
#include "filesmodel.h"
#include <QDomElement>

class FilesModel;

class Folder : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FilesModel * filesModel READ filesModel CONSTANT)
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(qint32 sidsAvailability READ sidsAvailability WRITE setSidsAvailability NOTIFY sidsAvailabilityChanged)
    Q_PROPERTY(bool inProcess READ inProcess WRITE setInProcess NOTIFY inProcessChanged)
    Q_PROPERTY(QObject* additionalData READ additionalData WRITE setAdditionalData NOTIFY additionalDataChanged)
    Q_PROPERTY(qint64 size READ size NOTIFY sizeChanged)
    Q_PROPERTY(qint64 downloadedSize READ downloadedSize NOTIFY downloadedSizeChanged)

public:
    explicit Folder(QObject *parent = 0);

    bool loadFromPath(const QUrl &path);

    bool loadFromDomElement(const QDomElement &domElement);

    QDomElement toDomElement(QDomDocument &domDocument) const;

public:
    inline FilesModel * filesModel() const {return _filesModel;}

    inline qint64 id() const {return _id;}
    void setId(const qint64 id);

    inline QString name() const {return _name;}
    void setName(const QString &name);

    inline qint32 sidsAvailability() const {return _sidsAvailability;}
    void setSidsAvailability(const qint32 sidsAvailability);

    inline bool inProcess() const {return _inProcess;}
    void setInProcess(const bool inProcess);

    inline QObject *additionalData() const {return _additionalData;}
    void setAdditionalData(QObject *additionalData);

    inline qint64 size() const {return _size;}

    inline qint64 downloadedSize() const {return _downloadedSize;}

signals:
    void idChanged();
    void nameChanged();
    void sidsAvailabilityChanged();
    void inProcessChanged();
    void additionalDataChanged();
    void sizeChanged();
    void downloadedSizeChanged();

private slots:
    void updateSize();
    void updateDownloadedSize();

private:
    FilesModel *_filesModel;
    qint64 _id;
    QString _name;
    qint32 _sidsAvailability;
    bool _inProcess;
    QObject *_additionalData;
    qint64 _size;
    qint64 _downloadedSize;

    void reset();
    void loadFromDir(const QUrl &path);
    void setSize(const qint64 size);
    void setDownloadedSize(const qint64 downloadedSize);
};

#endif // FOLDER_H
