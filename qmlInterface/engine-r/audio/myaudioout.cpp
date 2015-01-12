#include "myaudioout.h"

MyAudioTransmitter::MyAudioTransmitter(QObject *parent) :
   QObject(parent)
{
   m_device=QAudioDeviceInfo::defaultOutputDevice();
   m_format=m_device.preferredFormat();
   m_output=NULL;
   m_IO=NULL;

   m_isWait=true;
   emit isWait(m_isWait);
}

MyAudioTransmitter::~MyAudioTransmitter()
{
   if(m_output) delete m_output;
   deleteLater();
}

bool MyAudioTransmitter::startTransmiter(qint32 sampleRate,
                                         qint16 channelsCount,
                                         qint16 sampleSize)
{
   QAudioFormat format=m_format;
   format.setChannelCount(channelsCount);
   format.setSampleRate(sampleRate);
   format.setSampleSize(sampleSize);

   if(m_device.isFormatSupported(format))
      m_format=format;
   else
      m_format=m_device.nearestFormat(format);

   if(m_output==NULL)
   {
      m_output = new QAudioOutput(m_device,m_format,this);
      connect(m_output, SIGNAL(stateChanged(QAudio::State)), this, SLOT(stateChanged(QAudio::State)));
   }

   m_IO=m_output->start();
   return (m_IO!=NULL);
}

void MyAudioTransmitter::stopTransmiter()
{
   if(m_output)
   {
      disconnect(m_output, 0, this, 0);
      m_output->stop();
   }
}

void MyAudioTransmitter::play(QByteArray &data)
{
   if(m_IO)
   {
      m_IO->write(data);
      m_isWait=false;
      emit isWait(m_isWait);
   }
}

void MyAudioTransmitter::stateChanged(QAudio::State newState)
{
   switch (newState)
   {
      case QAudio::IdleState:
         m_isWait=true;
         emit isWait(m_isWait);
         break;
      default:
         break;
   }
}

void MyAudioOut::checkTerminate()
{
   if(m_isFinish && m_isWait)
   {
      disconnect(m_device,0,this,0);
      m_device->stopTransmiter();

      exit();
      wait();
   }
}

MyAudioOut::MyAudioOut(QObject *parent) :
   QThread(parent)
{
   m_device=new MyAudioTransmitter(this);
   connect(m_device, SIGNAL(isWait(bool)), this, SLOT(deviceState(bool)));
}

MyAudioOut::~MyAudioOut()
{
   //    delete v_device;
}

bool MyAudioOut::init(qint32 sampleRate, qint16 channelsCount, qint16 sampleSize)
{
   bool result=m_device->startTransmiter(sampleRate, channelsCount, sampleSize);

   if(result) start();

   m_ttl=-1;
   return result;
}

void MyAudioOut::play(qint32 ttl, QByteArray &data)
{
   if(ttl>m_ttl)
   {
      m_device->play(data);
      m_ttl=ttl;
   }
}

void MyAudioOut::finish()
{
   m_isFinish=true;
   checkTerminate();
}

void MyAudioOut::run()
{
   exec();
}

void MyAudioOut::deviceState(bool state)
{
   m_isWait=state;
   checkTerminate();
}
