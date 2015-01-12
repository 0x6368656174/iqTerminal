#ifndef MYNATDETECTOR_H
#define MYNATDETECTOR_H

#include <QObject>
#include <QByteArray>
#include <QTimer>
#include <QUdpSocket>

#include "../MyGlobal.h"
#include "mystunmessage.h"

//---------------------------------------------------------------------------
// типы NAT
#define natNotDetected      0
#define natUDPBlocked       1
#define natOpenInternet     2
#define natFullCone         3
#define natSymmetric        4
#define natRestricted       6
#define natPortRestricted   6
#define natStrongPortRestr  7

#define natErrorNo      0       // нет ошибок
#define natErrorBind    1       // ошибка привязки сокета
#define natErrorSend    2       // ошибка отправки пакета
#define natErrorStun    3       // сервер не отвечает
#define natErrorResp    4       // некорректный ответ сервера

class MyNATDetector : public QObject
{
   Q_OBJECT

private:
   QTimer m_timer;
   QUdpSocket m_socket;
   MyIPv6Address m_stunAddr,
   m_lanAddr,
   m_host,
   sourceIP, localIP, publicIP, aliasIP;

   qint32 m_step,
   m_repeat,
   m_timeout,
   m_typeNAT,
   m_typePort,
   m_errorCode;

   QHostAddress work_addr;
   quint16      work_port;

   QByteArray m_qwestion,
   m_answer;

   bool sameAddres,
   sameAddres2;

   qint32 DetectorStep0();
   qint32 DetectorStep1();
   qint32 DetectorStep2();
   qint32 DetectorStep3();
   qint32 DetectorStep4();
   qint32 DetectorStep5();

   void NATdetect();
   void Detector();

public:
   explicit MyNATDetector(QObject *parent = 0);
   ~MyNATDetector();

   void execute(MyIPv6Address StunAddr, MyIPv6Address LanAddr);

signals:
   void message(QString);
   void NATdetected(qint32, qint32, qint32);    // to config (typeNat, typePort, errorCode)

public slots:

private slots:
   void Timer100msec();
   void RecvMessage();
};

#endif // MYNATDETECTOR_H
