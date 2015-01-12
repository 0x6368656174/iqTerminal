#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QTimer>

#include "../MyGlobal.h"
#include "../common/mypacket.h"

class MyDialog : public QObject
{
   Q_OBJECT

   QTimer m_timer;
   qint32 m_step, m_count;
   MyPacket m_packet;

   QString m_Login;
   QString m_Password;

   QString getParam(QString param, qint32 n);
   qint16 checkPassword(QString password);
   qint16 checkEMail(QString email);

public:
   explicit MyDialog(QObject *parent = 0);
   ~MyDialog();
   void execute(qint16 cmd, qint32 sessionID, QString param, QByteArray rsa);	// from engine

signals:
   void message(QString);

   void Exit(MyDialog *, qint16, qint16);// to engine
   void sendMessage(MyPacket &);         // to server
   void Logon(QString, QString, qint32); // to config
   void SessionID(qint32);				  // to server

public slots:
   void receiveMessage(MyPacket &packet);                                        // from server

private slots:
   void Timer30Second();
};

#endif // MYDIALOG_H
