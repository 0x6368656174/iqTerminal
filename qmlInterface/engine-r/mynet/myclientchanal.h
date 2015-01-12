#ifndef MYCLIENTCHANAL_H
#define MYCLIENTCHANAL_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QTimer>
#include <QUdpSocket>

#include "../MyGlobal.h"
#include "../crypto/myrsa.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
//#include "../audio/zringtone.h"

#define stepRequestConnect 1
#define stepRequestMailRu 2
#define stepConnectWait 3
#define stepConnectConfirm 4
#define stepCofirmMailRu 5
#define stepConnectAccept 6
#define stepConnectMailRu 7
#define stepMarkerConnect 8
#define stepRequestProxy 9
#define stepTerminate 99

class MyClientChanal : public QObject
{
    Q_OBJECT

    QTimer m_timer;
    QUdpSocket m_socket;
    QUdpSocket m_socket_proxy;

    MyConfig      *m_config;    // config
    MyConnectBlock m_block;     // connect block
    qint32         m_direction; // direction: 0-out/1-in
    qint32         m_type;      // 0-SMS
    qint32         m_userID;    // with ?
    bool           m_proxy;
    qint32         m_connected;

    qint32 m_pin;
    qint32 m_pout;
    qint32 m_idle;

    MyIPv6Address m_serverIP, m_proxyIP, m_localIP, m_wlanIP, m_workIP, m_clientIP;

    QByteArray m_serverRSA;
    QByteArray m_clientRSAprivate;
    QByteArray m_clientRSApublic;
    QByteArray m_key;

    qint32 m_status, m_step, m_tick, m_attempt;

    MyPacket m_packet;

    void initParam();

    qint64 sendMessageToServer(MyPacket &packet);
    qint64 sendMessageToProxy(MyPacket &packet, MyIPv6Address clientIP);

    qint64 sendMessageToClient(MyPacket &packet, MyIPv6Address clientIP);
    MyPacket recvMessage(QByteArray &key);
    MyPacket recvMessageProxy(QByteArray &key);

    void requestConnect();
    void requestProxy();
    void sendConfirm();
    void acceptConnect();
    void sendMarker(qint32 ExitCode);

    void readServerAnswer();
    bool readConnectParam(MyPacket &Packet);
    void readMarker();

public:
    explicit MyClientChanal(QObject *parent = 0);
    ~MyClientChanal();

    void execute(MyConfig *Config, qint32 UserID, qint32 TypeChanal, bool useProxy=false);
    void execute(MyConfig *Config, MyPacket &Packet);

    void terminate();

    qint32 getStatus();
    qint32 getUserID();
    qint32 getSessionID();
    qint32 getTypeChanal();
    qint32 getIdle();
    bool   isProxy();
    bool   isConnected();
    qint32 getDirection();

    void sendPacket(MyPacket &packet);

signals:
    void status(MyClientChanal *, qint32);
    void receivePacket(MyClientChanal *, MyPacket &);

public slots:

private slots:
    void Timer100msec();
    void receiveMessage();
    void receiveMessageProxy();
};

#endif // MYCLIENTCHANAL_H
