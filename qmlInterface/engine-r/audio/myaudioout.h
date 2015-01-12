#ifndef MYAUDIOOUT_H
#define MYAUDIOOUT_H

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <QIODevice>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QAudioOutput>

class MyAudioTransmitter : public QObject
{
   Q_OBJECT

   QAudioDeviceInfo m_device;
   QAudioFormat     m_format;
   QAudioOutput    *m_output;
   QIODevice       *m_IO;

   bool m_isWait;

public:
   explicit MyAudioTransmitter(QObject *parent = 0);
   ~MyAudioTransmitter();

signals:
   void isWait(bool);

public slots:
   bool startTransmiter(qint32 sampleRate, qint16 channelsCount, qint16 sampleSize);
   void stopTransmiter(void);
   void play(QByteArray &data);

private slots:
   void stateChanged(QAudio::State newState);
};


class MyAudioOut : public QThread
{
   Q_OBJECT

   bool m_isFinish;
   bool m_isWait;

   qint32 m_ttl;

   MyAudioTransmitter *m_device;

   void checkTerminate();

public:
   explicit MyAudioOut(QObject *parent = 0);
   ~MyAudioOut();

   bool init(qint32 sampleRate, qint16 channelsCount, qint16 sampleSize);
   void play(qint32 ttl, QByteArray &data);
   void finish();

protected:
   void run();

signals:

public slots:

private slots:
   void deviceState(bool state);
};

#endif // MYAUDIOOUT_H
