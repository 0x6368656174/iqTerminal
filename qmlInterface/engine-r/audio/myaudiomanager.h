#ifndef MYAUDIOMANAGER_H
#define MYAUDIOMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QList>
#include <QStringList>

#include "user.h"
#include "usersmodel.h"

#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
#include "../mynet/myclientchanal.h"
#include "myaudioin.h"
#include "myaudioitem.h"

class MyAudioManager : public QObject
{
   Q_OBJECT

   MyConfig  *m_config;
   MyAudioIn *m_audioIn;

   QList<MyAudioItem *> m_list;

   qint32 m_ttl;

   void checkOnline();

public:
   explicit MyAudioManager(QObject *parent = 0);
   ~MyAudioManager();

   void init(MyConfig *config);

   qint32 getCountConnect();
   qint32 getUserID(qint32 index);
   MyClientChanal *getChanal(qint32 index);

   void Connect(qint32 UserID, bool proxy=false);
   void Disconnect(qint32 UserID);

signals:
   void process(qint32);
   void finished();
   void playArray(QByteArray &data);

public slots:
   void addConnect(MyClientChanal *chanal);
   void statusClient(qint32 UserID,qint32 status);
   void sendAudio(QByteArray &data);
};

#endif // MYAUDIOMANAGER_H
