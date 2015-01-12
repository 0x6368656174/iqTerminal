#ifndef MYPRIVATTOSERVER_H
#define MYPRIVATTOSERVER_H

#include <QObject>
#include <QTimer>
#include <QHostAddress>
#include <QTcpSocket>
#include <QUdpSocket>

#include <QByteArray>
#include <QString>

#include "../MyGlobal.h"
#include "../common/mypacket.h"
#include "../crypto/myrsa.h"
#include "../common/myconfig.h"

class MyPrivatToServer : public QObject
{
   Q_OBJECT

   QTimer m_timer;
   QUdpSocket m_socket;
   QTcpSocket m_tcp;

   MyIPv6Address m_serverIP,
                 m_localIP,
                 m_publicIP;

   QByteArray m_serverRSA;
   QByteArray m_clientRSAprivate;
   QByteArray m_clientRSApublic;

   quint16 m_cmd;
   qint32  m_userID, m_sessionID;
   QString m_str;

   QByteArray m_data, m_work;

   MyPacket m_packet;

   qint32  m_step, m_tick, m_attempt, m_length;
   qint64  m_balance;
   bool m_eow;

   qint64   sendMessageToServer(MyPacket &packet);
   MyPacket recvMessageFromServer();

   void getChanal();
   void getChanalAnswer();
   void startSession();

   void writeCommand();
   void writeData(QByteArray &data);
   void readData();
   void closeChanal(int code);

public:
   explicit MyPrivatToServer(QObject *parent = 0);
   ~MyPrivatToServer();

   void execute(MyConfig *Config, quint16 Cmd, QString Str, QByteArray &Data);
   QString getParam();

signals:
   void message(QString);
   void Exit(MyPrivatToServer *, qint32, QByteArray &);

public slots:

private slots:
   void Timer100msec();
   void receiveMessage();

   void connected();
   void readyRead();
   void bytesWriten(qint64 size);
   void errorSocket(QAbstractSocket::SocketError error);

};

#endif // MYPRIVATTOSERVER_H
