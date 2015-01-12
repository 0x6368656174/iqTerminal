#include "myaudioitem.h"

MyAudioItem::MyAudioItem(MyClientChanal *chanal, QObject *parent) :
    QObject(parent)
{
    m_chanal=chanal;
    m_audio=new MyAudioOut(this);

    connect(m_chanal, SIGNAL(receivePacket(MyClientChanal*,MyPacket&)),
            this,     SLOT  (playAudio(MyClientChanal*,MyPacket&)));
    connect(m_chanal, SIGNAL(status(MyClientChanal*,qint32)),
            this,     SLOT  (statusClient(MyClientChanal*,qint32)));

    m_inWork=false;
}

MyAudioItem::~MyAudioItem()
{
    disconnect(m_chanal,0,this,0);
    m_chanal->terminate();
    m_chanal->deleteLater();

    m_audio->finish();
    m_audio->deleteLater();
}

MyClientChanal *MyAudioItem::getChanal()
{
    return m_chanal;
}

MyAudioOut *MyAudioItem::getAudio()
{
    return m_audio;
}

void MyAudioItem::play(MyAudioBlock &param, QByteArray &data)
{
    if(!m_inWork)
        m_inWork=m_audio->init(param.samplerate,param.channels,param.samplesize);

    if(m_inWork)
        m_audio->play(param.ttl, data);
}

void MyAudioItem::playAudio(MyClientChanal *chanal, MyPacket &packet)
{
    Q_UNUSED(chanal);
    MyAudioBlock param;
    QByteArray data;
    if(packet.isAudio() && packet.isData())
    {
        param=packet.getAudio();
        data=packet.getData();
        play(param,data);
    }
}

void MyAudioItem::statusClient(MyClientChanal *chanal, qint32 status)
{
    emit newStatus(chanal->getUserID(),status);
}
