#ifndef MYAUDIOITEM_H
#define MYAUDIOITEM_H
#include <QObject>
#include <QIODevice>
#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <QAudioOutput>

#include "../common/mypacket.h"
#include "../mynet/myclientchanal.h"
#include "myaudioout.h"

class MyAudioItem : public QObject
{
   Q_OBJECT

   MyClientChanal *m_chanal;
   MyAudioOut     *m_audio;

   bool m_inWork;

public:
   explicit MyAudioItem(MyClientChanal *chanal, QObject *parent = 0);
   ~MyAudioItem();

   MyClientChanal *getChanal();
   MyAudioOut *getAudio();

signals:
   void newStatus(qint32 UserID, qint32 status);

public slots:
   void play(MyAudioBlock &param, QByteArray &data);

private slots:
   void playAudio(MyClientChanal *chanal, MyPacket &packet);
   void statusClient(MyClientChanal *chanal, qint32 status);
};

#endif // MYAUDIOITEM_H
