#ifndef MYFIRSTMANAGER_H
#define MYFIRSTMANAGER_H

#include <QObject>
#include <QString>
#include <QList>

#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
#include "myclientchanal.h"

class MyFirstManager : public QObject
{
   Q_OBJECT

   MyConfig *m_config;
   QList<MyClientChanal*> m_list;

public:
   explicit MyFirstManager(QObject *parent = 0);
   ~MyFirstManager();

   void init(MyConfig *config);

   qint32 getCountConnect();
   qint32 getUserID(qint32 index);
   MyClientChanal *getChanal(qint32 index);

signals:
   void newConnect(MyClientChanal*);

public slots:
   void recvServer(MyPacket &packet);

private slots:
   void statusClient(MyClientChanal *chanal, qint32 status);
};

#endif // MYFIRSTMANAGER_H
