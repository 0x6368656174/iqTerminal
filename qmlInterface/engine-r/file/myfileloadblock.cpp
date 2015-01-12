#include "myfileloadblock.h"

void MyFileLoadBlock::addConnect(bool proxy)
{
    m_chanal=new MyClientChanal(this);
    connect(m_chanal, SIGNAL(status(MyClientChanal*,qint32)),
            this,     SLOT  (statusClient(MyClientChanal*,qint32)));
    connect(m_chanal, SIGNAL(receivePacket(MyClientChanal*,MyPacket&)),
            this,     SLOT  (recvPacket(MyClientChanal*,MyPacket&)));

    m_chanal->execute(m_config,m_userID,typeFile,proxy);
}

void MyFileLoadBlock::sendFileName()
{
    qDebug() << "send filename";

    MyPacket packet;
    packet.setCmd(cmdSetFileName);
    packet.setFileBlock(m_messageID,0,0);
    m_chanal->sendPacket(packet);

    m_attempt++;
}

void MyFileLoadBlock::recvFileParam(MyPacket &packet)
{
    if((packet.getCmd()==(cmdSetFileName|cmdAnswer)) && packet.isFileBlock())
    {
        qDebug() << "recv file param";

        m_filelength=packet.getFileBlock().pos;
        m_state=1;
        m_attempt=0;

        emit statusBlock(this,2);
    }
}

void MyFileLoadBlock::sendQueryData()
{
    MyPacket packet;
    MyFileBlock block;
    block.fileID=m_messageID;
    block.pos=m_block;
    block.mask=(m_mask^m_mask_return);

    packet.setCmd(cmdGetData);
    packet.setFileBlock(block);

//    qDebug() << "send query" << block.pos << QString::number(block.mask,16);

    m_chanal->sendPacket(packet);
    m_state=2;
    m_attempt++;
}

void MyFileLoadBlock::recvFileData(MyPacket &packet)
{
    if((packet.getCmd()!=cmdSendData) || (!packet.isFileBlock()) || (!packet.isData())) return;

    MyFileBlock block=packet.getFileBlock();

    if(block.pos!=m_block) return;

    QByteArray data=packet.getData();
    if(data.size()>1024) data.resize(1024);

    quint8 kByte=0x1;
    for(qint32 i=0;i<8;i++)
    {
        if(block.mask&kByte)
        {
            m_buf[i]=data;
            m_mask_return|=kByte;
            break;
        }
        kByte<<=1;
    }

    m_attempt=0;
    if((m_mask&m_mask_return) == m_mask)
    {
//        qDebug() << "complete";
        m_state=3;
        emit blockCompleted(this);
    }
}

MyFileLoadBlock::MyFileLoadBlock(QObject *parent) :
    QObject(parent)
{
    m_attempt=0;
    m_chanal=NULL;
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timer100msec()));
}

MyFileLoadBlock::~MyFileLoadBlock()
{
    if(m_chanal) delete m_chanal;
    m_timer.stop();
    disconnect(&m_timer,0,0,0);
    deleteLater();
}

void MyFileLoadBlock::init(MyConfig *config, qint32 UserID, qint32 MessageID, QString FileName)
{
    m_config=config;
    m_userID=UserID;
    m_messageID=MessageID;
    m_filename=FileName;
    m_state=0;

    addConnect(false);
}

qint64 MyFileLoadBlock::getFileLength()
{
    return m_filelength;
}

qint64 MyFileLoadBlock::getBlock()
{
    return m_block;
}

quint8 MyFileLoadBlock::getMask()
{
    return m_mask_return;
}

QByteArray MyFileLoadBlock::getData(qint32 index)
{
    return m_buf[index];
}

void MyFileLoadBlock::setBlock(qint64 block, quint8 mask)
{
    if(mask==0)
    {
        m_state=3;
        emit statusBlock(this,3);
        return;
    }

    m_block=block;
    m_mask=mask;
    m_mask_return=0;
    for(qint32 i=0;i<8;i++) m_buf[i].clear();

    m_attempt=0;
    sendQueryData();
}

void MyFileLoadBlock::timer100msec()
{
    if(m_attempt>5)
    {
        emit statusBlock(this,99);
    }
    else
    {
        switch(m_state)
        {
            case 0: sendFileName(); break;
            case 2: sendQueryData(); break;
        }
    }
}

void MyFileLoadBlock::recvPacket(MyClientChanal *chanal, MyPacket &packet)
{
    Q_UNUSED(chanal)

    switch(m_state)
    {
        case 0: recvFileParam(packet); break;
        case 2: recvFileData(packet); break;
    }
}

void MyFileLoadBlock::statusClient(MyClientChanal *chanal, qint32 status)
{
    qDebug() << "status" << status;
    switch(status)
    {
        case 2:
            m_timer.start(2000);
            sendFileName();
            break;
        case 3:
            if((m_state==0) && chanal->isConnected() && !chanal->isProxy())
            {
                disconnect(chanal,0,0,0);
                delete chanal;

                addConnect(true);
            }
            else
            {
                emit statusBlock(this,3);
            }
            break;
    }
}
