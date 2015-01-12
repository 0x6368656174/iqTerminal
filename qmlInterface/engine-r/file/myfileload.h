#ifndef MYFILELOAD_H
#define MYFILELOAD_H

#include <QObject>
#include <QFile>
#include <QList>

#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
#include "../mynet/myclientchanal.h"
#include "myfileloadblock.h"

class MyFileLoad : public QObject
{
    Q_OBJECT

    QFile m_file;
    MyConfig *m_config;
    MyClientChanal *m_chanal;
    QString m_filename;
    qint32 m_countitem;
    qint32 m_state;
    qint32 m_userID;
    qint32 m_messageID;

    qint32 m_map_current;
    QByteArray m_map;

    MyFileBlock m_block;

    QList<MyFileLoadBlock *> m_item;

    void prepareFile(qint64 length);
    bool isLoad();
    void getBlock(MyFileLoadBlock *item);

    void loaded();

    void saveTorrent();
    bool loadTorrent();
    void removeTorrent();

    void addConnect();
    void delConnect(MyFileLoadBlock *item);

public:
    explicit MyFileLoad(QObject *parent = 0);

    void load(MyConfig *config, QString filename);
    qint32 getState();
    qint32 getUserID();
    QString getFileName();

signals:
    void markMessage(MyFileLoad *item, qint32 UserID, QString FileName);

public slots:

private slots:
    void blockCompleted(MyFileLoadBlock *item);
    void statusClient(MyFileLoadBlock *item, qint32 status);
};

#endif // MYFILELOAD_H
