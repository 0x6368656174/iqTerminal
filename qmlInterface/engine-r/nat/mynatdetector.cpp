#include "mynatdetector.h"
#include <QHostAddress>

qint32 MyNATDetector::DetectorStep0()
{
   StunAtrString hmacPassword;
   MySTUNmessage stun_msg;
   char buf[STUN_MAX_MESSAGE_SIZE];

   sameAddres =false;
   sameAddres2=false;
   sourceIP=m_stunAddr;
   localIP =m_lanAddr;

   // запрос к основному STUN серверу
   localIP.port=0;
   //    emit message("nat step0 "+QHostAddress(localIP.host).toString());
   if(!m_socket.bind(QHostAddress(localIP.host),localIP.port))
   {
      return natErrorBind;
   }
   localIP.port=m_socket.localPort();
   //    emit message("port="+QString::number(localIP.port));

   stun_msg.Clear();
   stun_msg.msg.msgHdr.msgType=BindRequestMsg;
   stun_msg.msg.msgHdr.msgLength=0;
   stun_msg.SetID();
   hmacPassword.sizeValue = 0;
   quint32 length=stun_msg.EncodeMessage(buf,STUN_MAX_MESSAGE_SIZE,hmacPassword);

   m_qwestion.resize(length);
   memcpy(m_qwestion.data(),buf,length);

   m_step=1;
   m_repeat=0;
   m_timeout=1;
   m_host=sourceIP;
   m_socket.writeDatagram(m_qwestion,QHostAddress(m_host.host),m_host.port);
   return 0;
}

qint32 MyNATDetector::DetectorStep1()
{
   StunAtrString hmacPassword;
   MySTUNmessage stun_msg;
   char buf[STUN_MAX_MESSAGE_SIZE];

   if(m_answer.length()==0)
   {
      return natErrorStun;
   }

   bool ok=stun_msg.DecodeMessage(m_answer.data(),m_answer.length());

   if(!(ok &&
        stun_msg.msg.hasMappedAddress &&
        stun_msg.msg.hasSourceAddress &&
        stun_msg.msg.hasChangedAddress))
   {
      return natErrorResp;
   }

   publicIP=stun_msg.msg.mappedAddress.ipv6;
   aliasIP =stun_msg.msg.changedAddress.ipv6;

   // локальный адрес==публичный адрес клиента ???
   if((memcmp(publicIP.host.c,localIP.host.c,16)==0) && (publicIP.port==localIP.port))
   {
      sameAddres=true;
   }

   if(!sameAddres) {
      if(publicIP.port==localIP.port)
         m_typePort=2;
      else
         m_typePort=1;
   }

   // запрос к альтернативному STUN серверу
   stun_msg.Clear();
   stun_msg.msg.msgHdr.msgType=BindRequestMsg;
   stun_msg.msg.msgHdr.msgLength=0;
   stun_msg.SetID();
   hmacPassword.sizeValue = 0;
   qint32 length=stun_msg.EncodeMessage(buf,STUN_MAX_MESSAGE_SIZE,hmacPassword);

   m_qwestion.resize(length);
   memcpy(m_qwestion.data(),buf,length);

   m_step=2;
   m_repeat=0;
   m_timeout=1;
   m_host=aliasIP;
   m_socket.writeDatagram(m_qwestion,QHostAddress(m_host.host),m_host.port);
   return 0;
}

qint32 MyNATDetector::DetectorStep2()
{
   StunAtrString hmacPassword;
   MySTUNmessage stun_msg;
   char buf[STUN_MAX_MESSAGE_SIZE];

   if(m_answer.length()==0)
   {
      return natErrorStun;
   }

   bool ok=stun_msg.DecodeMessage(m_answer.data(),m_answer.length());

   if(!(ok &&
        stun_msg.msg.hasMappedAddress &&
        stun_msg.msg.hasSourceAddress &&
        stun_msg.msg.hasChangedAddress))
   {
      return natErrorResp;
   }

   if((memcmp(publicIP.host.c,stun_msg.msg.mappedAddress.ipv6.host.c,16)==0) &&
         (publicIP.port==stun_msg.msg.mappedAddress.ipv6.port))
   {
      sameAddres2=true;
   }

   m_socket.close();

   // запрос к основному STUN серверу - ответ ждем от альтернативного
   localIP.port=0;
   //    emit message("nat step2 "+QHostAddress(localIP.host).toString());
   if(!m_socket.bind(QHostAddress(localIP.host),localIP.port))
   {
      return natErrorBind;
   }
   localIP.port=m_socket.localPort();
   //    emit message("port="+QString::number(localIP.port));

   stun_msg.Clear();
   stun_msg.msg.msgHdr.msgType=BindRequestMsg;
   stun_msg.msg.msgHdr.msgLength=0;
   stun_msg.SetID();
   stun_msg.SetChangeRequest(true,true);
   hmacPassword.sizeValue = 0;
   qint32 length=stun_msg.EncodeMessage(buf,STUN_MAX_MESSAGE_SIZE,hmacPassword);

   m_qwestion.resize(length);
   memcpy(m_qwestion.data(),buf,length);

   m_step=3;
   m_repeat=0;
   m_timeout=1;
   m_host=sourceIP;
   m_socket.writeDatagram(m_qwestion,QHostAddress(m_host.host),m_host.port);
   return 0;
}

qint32 MyNATDetector::DetectorStep3()
{
   StunAtrString hmacPassword;
   MySTUNmessage stun_msg;
   char buf[STUN_MAX_MESSAGE_SIZE];
   bool ok;

   if(sameAddres)
   {
      if(m_answer.length()==0)
      {
         m_step=10;
         m_typeNAT=natUDPBlocked; // UDP блокированы
      }
      else
      {
         ok=stun_msg.DecodeMessage(m_answer.data(),m_answer.length());

         if(!(ok &&
              stun_msg.msg.hasMappedAddress &&
              stun_msg.msg.hasSourceAddress &&
              stun_msg.msg.hasChangedAddress))
         {
            return natErrorResp;
         }
         else
         {
            m_step=10;
            m_typeNAT=natOpenInternet; // Open Internet
         }
      }
      return 0;
   }
   else
   {
      if(m_answer.length()>0)
      {
         ok=stun_msg.DecodeMessage(m_answer.data(),m_answer.length());

         if(!(ok &&
              stun_msg.msg.hasMappedAddress &&
              stun_msg.msg.hasSourceAddress &&
              stun_msg.msg.hasChangedAddress))
         {
            return natErrorResp;
         }

         m_step=10;
         m_typeNAT=natFullCone; // Full Cone NAT
         return 0;
      }
   }

   if(!sameAddres2)
   {
      m_step=10;
      m_typeNAT=natSymmetric; // Symmetric NAT
      return 0;
   }

   // запрос к основному STUN серверу - ответ ждем с другого порта
   m_socket.close();
   localIP.port=0;
   //    emit message("nat step3 "+QHostAddress(localIP.host).toString());
   if(!m_socket.bind(QHostAddress(localIP.host),localIP.port))
   {
      return natErrorBind;
   }
   localIP.port=m_socket.localPort();
   //    emit message("port="+QString::number(localIP.port));

   stun_msg.Clear();
   stun_msg.msg.msgHdr.msgType=BindRequestMsg;
   stun_msg.msg.msgHdr.msgLength=0;
   stun_msg.SetID();
   stun_msg.SetChangeRequest(false,true);
   hmacPassword.sizeValue = 0;
   qint32 length=stun_msg.EncodeMessage(buf,STUN_MAX_MESSAGE_SIZE,hmacPassword);

   m_qwestion.resize(length);
   memcpy(m_qwestion.data(),buf,length);

   m_step=4;
   m_repeat=0;
   m_timeout=1;
   m_host=sourceIP;
   m_socket.writeDatagram(m_qwestion,QHostAddress(m_host.host),m_host.port);
   return 0;
}

qint32 MyNATDetector::DetectorStep4()
{
   StunAtrString hmacPassword;
   MySTUNmessage stun_msg;
   char buf[STUN_MAX_MESSAGE_SIZE];

   if(m_answer.length()==0)
   {
      // запрос к основному серверу, ждем ответа с другого IP
      stun_msg.Clear();
      stun_msg.msg.msgHdr.msgType=BindRequestMsg;
      stun_msg.msg.msgHdr.msgLength=0;
      stun_msg.SetID();
      stun_msg.SetChangeRequest(true,false);

      hmacPassword.sizeValue = 0;
      qint32 length=stun_msg.EncodeMessage(buf,STUN_MAX_MESSAGE_SIZE,hmacPassword);

      m_qwestion.resize(length);
      memcpy(m_qwestion.data(),buf,length);

      m_step=5;
      m_repeat=0;
      m_timeout=1;
      m_host=sourceIP;
      m_socket.writeDatagram(m_qwestion,QHostAddress(m_host.host),m_host.port);
      return 0;
   }

   bool ok=stun_msg.DecodeMessage(m_answer.data(),m_answer.length());

   if(!(ok &&
        stun_msg.msg.hasMappedAddress &&
        stun_msg.msg.hasSourceAddress &&
        stun_msg.msg.hasChangedAddress))
   {
      return natErrorResp;
   }

   m_step=10;
   m_typeNAT=natRestricted;   // Restricted NAT
   return 0;
}

qint32 MyNATDetector::DetectorStep5()
{
   if(m_answer.length()==0)
      m_typeNAT=natStrongPortRestr;   // Strong Port Restricted NAT
   else
      m_typeNAT=natPortRestricted;    // Port Restricted NAT
   m_step=10;
   return 0;

}

void MyNATDetector::Detector()
{
   switch(m_step)
   {
      case 0: // запуск детектора
         m_errorCode=DetectorStep0();
         break;
      case 1: //
         m_errorCode=DetectorStep1();
         break;
      case 2: //
         m_errorCode=DetectorStep2();
         break;
      case 3: //
         m_errorCode=DetectorStep3();
         break;
      case 4: //
         m_errorCode=DetectorStep4();
         break;
      case 5: //
         m_errorCode=DetectorStep5();
         break;
   }

   if(m_errorCode || m_step>5)
      emit NATdetected(m_typeNAT,m_typePort,m_errorCode);
}

MyNATDetector::MyNATDetector(QObject *parent) :
   QObject(parent)
{
   connect(&m_socket,SIGNAL(readyRead()),this,SLOT(RecvMessage()));
   connect(&m_timer,SIGNAL(timeout()),this,SLOT(Timer100msec()));
}

MyNATDetector::~MyNATDetector()
{
   m_timer.stop();
   m_socket.close();
   deleteLater();
}

void MyNATDetector::execute(MyIPv6Address StunAddr, MyIPv6Address LanAddr)
{
   m_stunAddr=StunAddr;
   m_lanAddr=LanAddr;
   m_lanAddr.port=0;

   m_typeNAT=natNotDetected;
   m_typePort=0;
   m_errorCode=natErrorNo;

   m_step=0;
   m_repeat=9;
   m_timeout=0;

   m_qwestion.resize(0);
   m_answer.resize(0);
   m_timer.start(100);
}

void MyNATDetector::Timer100msec()
{
   // ответ не получен -> необходимо повторить запрос (если попытки не кончились)
   if(m_repeat<9)
   {
      if(m_timeout>0)
      {
         m_timeout--;
      }
      else
      {
         m_repeat++;
         m_timeout=(m_repeat<<1);
         m_socket.writeDatagram(m_qwestion,QHostAddress(m_host.host),m_host.port);
      }
   }
   else
   {
      m_answer.resize(0);
      Detector();
   }
}

void MyNATDetector::RecvMessage()
{
   qint32 length;

   while(m_socket.hasPendingDatagrams())
   {
      length=m_socket.pendingDatagramSize();
      m_answer.resize(length);
      m_socket.readDatagram(m_answer.data(), length, &work_addr, &work_port);

      if((length>=20) && (memcmp(m_qwestion.data()+4,m_answer.data()+4,16)==0))
      {
         Detector(); break;
      }
   }
}

