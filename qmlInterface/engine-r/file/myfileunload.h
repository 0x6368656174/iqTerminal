#ifndef MYFILEUNLOAD_H
#define MYFILEUNLOAD_H

#include <QObject>
#include <QTimer>
#include <QFile>

#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
#include "../mynet/myclientchanal.h"

class MyFileUnload : public QObject
{
    Q_OBJECT
    QTimer m_timer;
    QFile m_file;

    MyConfig *m_config;
    MyClientChanal *m_chanal;
    qint32 m_state;
    qint32 m_attempt;
    qint32 m_userID;
    qint32 m_messageID;

    bool findFileInChat();
    void endUnload();
    void recvFileParam(MyPacket &packet);
    void sendFileParam();
    void recvQueryData(MyPacket &packet);

public:
    explicit MyFileUnload(QObject *parent = 0);

    void unload(MyConfig *config, MyClientChanal *chanal);

signals:

public slots:

private slots:
    void timer100msec();
    void recvPacket(MyClientChanal *chanal, MyPacket &packet);
    void statusClient(MyClientChanal *chanal, qint32 status);
};

#endif // MYFILEUNLOAD_H
