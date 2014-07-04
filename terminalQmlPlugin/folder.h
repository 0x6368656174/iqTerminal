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
    Q_PROPERTY(bool loadingInProcess READ loadingInProcess WRITE setLoadingInProcess NOTIFY loadingInProcessChanged)

public:
    explicit Folder(QObject *parent = 0);

    bool loadFromPath(const QString &path);

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

    inline bool loadingInProcess() const {return _loadingInProcess;}
    void setLoadingInProcess(const bool loadingInProcess);

signals:
    void idChanged();
    void nameChanged();
    void sidsAvailabilityChanged();
    void loadingInProcessChanged();

private:
    FilesModel *_filesModel;
    qint64 _id;
    QString _name;
    qint32 _sidsAvailability;
    bool _loadingInProcess;

    void reset();
};

#endif // FOLDER_H
