#ifndef MYAUDIOIN_H
#define MYAUDIOIN_H

#include <QObject>
#include <QThread>
#include <QByteArray>
#include <QIODevice>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QAudioInput>

class MyAudioIn : public QThread
{
   Q_OBJECT

   QAudioFormat m_format;
   QAudioInput *m_audioInput;
   QIODevice   *m_audioDeviceInput;

   bool m_isRuning;

public:
   explicit MyAudioIn(QObject *parent = 0);
   ~MyAudioIn();

   QAudioFormat getFormat();

   qint32 getInterval();

protected:
   void run(void);

signals:
   void capturedData(QByteArray &data);

public slots:
   void startThr();
   void stopThr();
   void exitThr();

private slots:
   void readData();
};

#endif // MYAUDIOIN_H
