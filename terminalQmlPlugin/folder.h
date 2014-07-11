#ifndef FOLDER_H
#define FOLDER_H

#include <QObject>
#include "filesmodel.h"
#include "abstractxmlitemobject.h"
#include <QDomElement>

class FilesModel;

class Folder : public AbstractXmlItemObject
{
    Q_OBJECT
    Q_PROPERTY(FilesModel * filesModel READ filesModel CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(qint32 sidsAvailability READ sidsAvailability WRITE setSidsAvailability NOTIFY sidsAvailabilityChanged)
    Q_PROPERTY(bool inProcess READ inProcess WRITE setInProcess NOTIFY inProcessChanged)
    Q_PROPERTY(qint64 size READ size NOTIFY sizeChanged)
    Q_PROPERTY(qint64 downloadedSize READ downloadedSize NOTIFY downloadedSizeChanged)

public:
    explicit Folder(QObject *parent = 0);

    bool loadFromPath(const QUrl &path);

public:
    virtual bool loadFromDomElement(const QDomElement &domElement);

    virtual QDomElement toDomElement(QDomDocument &domDocument) const;

public:
    inline FilesModel * filesModel() const {return _filesModel;}

    inline QString name() const {return _name;}
    void setName(const QString &name);

    inline qint32 sidsAvailability() const {return _sidsAvailability;}
    void setSidsAvailability(const qint32 sidsAvailability);

    inline bool inProcess() const {return _inProcess;}
    void setInProcess(const bool inProcess);

    inline qint64 size() const {return _size;}

    inline qint64 downloadedSize() const {return _downloadedSize;}

signals:
    void nameChanged();
    void sidsAvailabilityChanged();
    void inProcessChanged();
    void sizeChanged();
    void downloadedSizeChanged();

private slots:
    void updateSize();
    void updateDownloadedSize();

private:
    FilesModel *_filesModel;
    QString _name;
    qint32 _sidsAvailability;
    bool _inProcess;
    qint64 _size;
    qint64 _downloadedSize;

    void reset();
    void loadFromDir(const QUrl &path);
    void setSize(const qint64 size);
    void setDownloadedSize(const qint64 downloadedSize);
};

#endif // FOLDER_H
