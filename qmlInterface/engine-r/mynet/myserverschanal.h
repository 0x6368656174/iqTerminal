#ifndef MYSERVERSCHANAL_H
#define MYSERVERSCHANAL_H

#include <QObject>
#include <QByteArray>
#include <QList>
#include <QTimer>
#include <QString>
#include <QHostAddress>
#include <QUdpSocket>

#include "../MyGlobal.h"
#include "../crypto/myrsa.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"

class MyServersChanal : public QObject
{
   Q_OBJECT

   QTimer m_timer;
   bool m_flag;
   QList<QHostAddress> m_list;
   QUdpSocket m_socket;

   MyIPv6Address m_serverIP,
   m_localIP,
   m_publicIP;

   QByteArray m_serverRSA;
   QByteArray m_clientRSAprivate;
   QByteArray m_clientRSApublic;

   qint32 m_countPing,
   m_sessionID,
   m_userID;

   bool m_needMapping;
   qint32 m_numberIP;

   qint64 sendMessageToServer(MyPacket &packet);
   void parseMessage();

public:
   explicit MyServersChanal(QObject *parent = 0);

   void execute(MyConfig *Config);

   ~MyServersChanal();

signals:
   void receiveMessage(MyPacket &);                  // to dialog
   void ConnectServer(bool);                         // to config
   void MappingAddres(MyIPv6Address, MyIPv6Address); // to config
   void UserID(qint32);                              // to config

public slots:
   void sendMessage(MyPacket &packet);     // from dialog
   void setSessionID(qint32 sessionID);    // from dialog

private slots:
   void Timer30Second();
   void receiveMessageFromServer();
};

#endif // MYSERVERSCHANAL_H
