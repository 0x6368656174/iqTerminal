#ifndef MYFILELOAD_H
#define MYFILELOAD_H

#include <QObject>
#include <QTimer>
#include <QFile>
#include <QList>

#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
#include "../mynet/myclientchanal.h"

class MyFileLoad : public QObject
{
    Q_OBJECT

    QTimer m_timer;
    QFile m_file;
    MyConfig *m_config;
    MyClientChanal *m_chanal;
    QString m_filename;
    qint32 m_state;
    qint32 m_attempt;
    qint32 m_tick;
    qint32 m_userID;
    qint32 m_messageID;

    QList<quint8> m_map;
    qint32 m_map_pos;

    qint32 m_chanals_count;
    QList<MyClientChanal *> m_chanals;
    QList<qint32> m_chanals_state;

    MyFileBlock m_block;

    qint32 calcChanalsCount(qint64 fileSize);
    void prepareFile();
    qint64 getBlock();

    void loaded();

    void sendFileName(MyClientChanal *chanal);
    void recvFileParam(MyClientChanal *chanal, MyPacket &packet);
    qint32 sendQueryData(MyClientChanal *chanal);
    void recvFileData(MyClientChanal *chanal,MyPacket &packet);
    void sendEndSession(MyClientChanal *chanal);

    void createChanal();

    void saveTorrent();
    bool loadTorrent();
    void removeTorrent();

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
    void timer100msec();
    void recvPacket(MyClientChanal *chanal, MyPacket &packet);
    void statusClient(MyClientChanal *chanal, qint32 status);
};

#endif // MYFILELOAD_H
