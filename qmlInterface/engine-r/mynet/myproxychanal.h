#ifndef MYPROXYCHANAL_H
#define MYPROXYCHANAL_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include "message.h"
#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"

class MyProxyChanal : public QObject
{
    Q_OBJECT

    QTimer m_timer;
    QUdpSocket m_socket;
    MyConfig *m_config;
    MyIPv6Address m_proxyIP;

    QByteArray m_ServerKey;
    QByteArray m_ClientKey;

    void sendRequestToProxy();

public:
    explicit MyProxyChanal(QObject *parent = 0);
    void execute(MyConfig *config);

signals:
    void recvMessage(qint32 UserID, MyPacket &packet, bool proxy);

public slots:
    void sendMessage(qint32 UserID, MyPacket &packet);

private slots:
    void timer1sec();
    void getDatagramm();

};

#endif // MYPROXYCHANAL_H
