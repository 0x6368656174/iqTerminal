#include "myvideoitem.h"
#include <QBuffer>

void MyVideoItem::sendFrame(qint32 n)
{
    MyPacket packet;
    packet.setSignature(signature_no_RSA);
    packet.setCmd(cmdSendVideo);
    packet.setExitCode(0);

    MyVideoBlock block;
    block.kadr=m_kadrOutIDw;
    block.length=m_byteOut.size();
    block.block=n;
    packet.setVideo(block);

    QByteArray data;
    data=m_byteOut.mid((n<<10),1024);
    packet.setData(data);

    m_chanal->sendPacket(packet);
}

void MyVideoItem::sendKadr()
{
//    qDebug() << "send kadr" << m_kadrOutIDw << m_byteOut.size() << m_bitOut.count(true);
    for(qint32 i=0;i<m_bitOut.size();i++)
        if(m_bitOut.at(i))
            sendFrame(i);
}

void MyVideoItem::checkNextOut()
{
    if((m_bitOut.count(true)==0) && m_newKadrOut)
    {
        m_byteOut.clear();
        QBuffer bf(&m_byteOut);
        bf.open(QIODevice::ReadWrite);

        m_kadrOut.save(&bf,"JPG",25);
        m_kadrOutIDw=m_kadrOutID;

        qint32 kbyte=(m_byteOut.size()+1023)>>10;
        m_bitOut.clear();
        m_bitOut.fill(true,kbyte);

        m_newKadrOut=false;
        m_tickOut=0;

        sendKadr();
    }
}

void MyVideoItem::sendAnswer(MyVideoBlock &block)
{
    MyPacket packet;
    packet.setSignature(signature_no_RSA);
    packet.setCmd(cmdSendVideo|cmdAnswer);
    packet.setExitCode(0);

    packet.setVideo(block);

    m_chanal->sendPacket(packet);
}

void MyVideoItem::checkKadrIn()
{
    if((m_kadrInID!=-1)&&(m_bitIn.count(false)==0))
    {
        m_kadrIn.loadFromData(m_byteIn);

//        qDebug() << "show kadr" << m_kadrInID << m_byteIn.size() << m_bitIn.count(true);
        emit updateVideoIn(m_kadrIn);

        m_kadrInID=-1;
        m_byteIn.clear();
        m_bitIn.clear();
        m_tickIn=0;
    }
}

void MyVideoItem::buildPacket(MyPacket &packet)
{
    MyVideoBlock block=packet.getVideo();
    QByteArray data=packet.getData();
    if(data.size()>1024) data.resize(1024);

    if((m_kadrInID==-1) || (m_kadrInID<block.kadr))
    {
        m_kadrInID=block.kadr;
        m_byteIn.resize(block.length);
        qint32 kbyte=(block.length+1023)>>10;
        m_bitIn.clear();
        m_bitIn.fill(false,kbyte);

//        qDebug() << "recv kadr" << m_kadrInID << m_byteIn.size() << m_bitIn.count(true);
    }

    if((block.kadr==m_kadrInID)&&(block.block<m_bitIn.size()))
    {
        m_byteIn.replace((block.block<<10),data.size(),data);
        m_bitIn.setBit(block.block,true);

        sendAnswer(block);
    }
    checkKadrIn();
}

MyVideoItem::MyVideoItem(MyClientChanal *chanal, QObject *parent) :
    QObject(parent)
{
    m_chanal=chanal;

    connect(m_chanal, SIGNAL(receivePacket(MyClientChanal*,MyPacket&)),
            this,     SLOT  (receivePacket(MyClientChanal*,MyPacket&)));
    connect(m_chanal, SIGNAL(status(MyClientChanal*,qint32)),
            this,     SLOT  (statusClient(MyClientChanal*,qint32)));

    m_inWork=false;

    m_newKadrOut=false;
    m_tickOut=0;
    m_kadrOutID=-1;
    m_byteOut.clear();
    m_bitOut.clear();

    m_tickIn=0;
    m_kadrInID=-1;
    m_byteIn.clear();
    m_bitIn.clear();

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timer15msec()));
    m_timer.start(50);
}

MyVideoItem::~MyVideoItem()
{
    disconnect(&m_timer,0,0,0);

    disconnect(m_chanal,0,this,0);
    m_chanal->terminate();
    m_chanal->deleteLater();
}

MyClientChanal *MyVideoItem::getChanal()
{
    return m_chanal;
}

void MyVideoItem::updateVideoOut(QImage kadr)
{
    m_kadrOut=kadr;
    m_kadrOutID++;
    m_newKadrOut=true;
    checkNextOut();

//    emit updateVideoIn(m_kadrOut);
}

void MyVideoItem::receivePacket(MyClientChanal *chanal, MyPacket &packet)
{
// получить пакет собрать кадр и отобразить его
    Q_UNUSED(chanal)

    quint16 cmd=packet.getCmd();


    if((cmd==(cmdAnswer|cmdSendVideo)) && (packet.isVideo()))
    {
        MyVideoBlock block=packet.getVideo();
        if((block.kadr==m_kadrOutIDw)&&(block.block<m_bitOut.size()))
        {
//            qDebug() << "recv" << block.kadr << block.block;

            m_bitOut.clearBit(block.block);
            checkNextOut();
        }
    }

    if((cmd==cmdSendVideo)&&(packet.isVideo())&&(packet.isData()))
    {
        buildPacket(packet);
    }
}

void MyVideoItem::statusClient(MyClientChanal *chanal, qint32 status)
{
    emit newStatus(chanal->getUserID(),status);
}

void MyVideoItem::timer15msec()
{
    if(++m_tickOut>3)
    {
        m_bitOut.fill(false);
        checkNextOut();
    }
    else
    {
        if((m_tickOut&0x1) == 0) sendKadr();
    }


    if(m_kadrInID!=-1)
    {
        if(++m_tickIn>3)
        {
            m_tickIn=0;
            m_kadrInID=-1;
            m_byteIn.clear();
            m_bitIn.clear();
        }
    }
}
