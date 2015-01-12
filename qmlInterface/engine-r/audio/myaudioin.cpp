#include "myaudioin.h"

MyAudioIn::MyAudioIn(QObject *parent) :
   QThread(parent)
{
   m_audioInput=NULL;
   m_audioDeviceInput=NULL;

   QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
   m_format=info.preferredFormat();
   m_format.setChannelCount(info.supportedChannelCounts().at(0));
   m_format.setSampleRate(info.supportedSampleRates().at(0));
   m_format.setSampleSize(16 /*info.supportedSampleSizes().at(0)*/);

   m_isRuning=false;
}

MyAudioIn::~MyAudioIn()
{
   exitThr();
   deleteLater();
}

QAudioFormat MyAudioIn::getFormat()
{
   return m_format;
}

qint32 MyAudioIn::getInterval()
{
   return m_audioInput->bufferSize();
}

void MyAudioIn::run()
{
   exec();
}

void MyAudioIn::startThr()
{
   if(m_audioInput==NULL)
   {
      m_audioInput = new QAudioInput(m_format,this);
   }

   if(m_audioDeviceInput==NULL)
   {
      m_audioDeviceInput = m_audioInput->start();
      connect(m_audioDeviceInput, SIGNAL(readyRead()), this, SLOT(readData()));
   }

   if(!m_isRuning)
   {
      m_isRuning=true;
      start();
   }
}

void MyAudioIn::stopThr()
{
   if(m_audioDeviceInput)
   {
      disconnect(m_audioDeviceInput, 0, this, 0);
      m_audioDeviceInput=NULL;
   }

   if(m_audioInput)
      m_audioInput->stop();
}

void MyAudioIn::exitThr()
{
   if(m_isRuning)
   {
      exit();
      wait();
      m_isRuning=false;
   }

   stopThr();
   delete m_audioInput;
   m_audioInput=NULL;
}

void MyAudioIn::readData()
{
   QByteArray tmp=m_audioDeviceInput->read(8192);
   emit capturedData(tmp);
}
