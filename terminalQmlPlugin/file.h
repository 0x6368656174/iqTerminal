#ifndef FILE_H
#define FILE_H

#include <QObject>
#include <QDomElement>

class File : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(qint64 size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(qint64 downloadedSize READ downloadedSize WRITE setDownloadedSize NOTIFY downloadedSizeChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
public:
    explicit File(QObject *parent = 0);

    bool loadFromPath(const QUrl &path);

    bool loadFromDomElement(const QDomElement &domElement);

    QDomElement toDomElement(QDomDocument &domDocument) const;

public:
    inline qint64 id() const {return _id;}
    void setId(const qint64 id);

    inline QString path() const {return _path;}
    void setPath(const QString &path);

    inline qint64 size() const {return _size;}
    void setSize(const qint64 size);

    inline qint64 downloadedSize() const {return _downloadedSize;}
    void setDownloadedSize(const qint64 downloadedSize);

    inline QString name() const {return _name;}

signals:
    void idChanged();
    void pathChanged();
    void sizeChanged();
    void downloadedSizeChanged();
    void nameChanged();

private:
    qint64 _id;
    QString _path;
    qint64 _size;
    qint64 _downloadedSize;

    QString _name;
    void setName(const QString &name);

    void reset();
};

#endif // FILE_H
