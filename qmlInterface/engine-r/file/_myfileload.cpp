#include "myfileload.h"
#include <QUrl>

qint32 MyFileLoad::calcChanalsCount(qint64 fileSize)
{
    qint64 result=(fileSize>>18);
    if(result>10) result=10;
    return result;
}

void MyFileLoad::prepareFile()
{
    qint64 kBytes=(m_block.pos+1023)>>10;
    qint32 count=(kBytes+7)>>3;
    qDebug() << kBytes << "kBytes" << count;

    m_map.clear();
    m_map.reserve(count);
    for(qint32 i=0;i<count-1;i++) m_map.append(0xff);

    kBytes=kBytes&0x7;
    if(kBytes)
    {
        quint8 mask=(quint8(1)<<kBytes)-1;
        m_map.append(mask);
    }

    if(m_file.exists() && loadTorrent())
    {
        m_file.open(QIODevice::ReadWrite);
    }
    else
    {
        m_file.open(QIODevice::ReadWrite);
        m_file.resize(m_block.pos);
    }

    m_map_pos=0;
    m_chanals_count=calcChanalsCount(m_block.pos);
    qDebug() << "chanals count" << m_chanals_count;
}

qint64 MyFileLoad::getBlock()
{
    qint64 result=m_map.size();
    while(result>0)
    {
        if(m_map_pos>=m_map.size()) m_map_pos=0;
        if(m_map.at(m_map_pos)==0) m_map_pos++;

        result--;
    }

    result=m_map_pos++;

    if(m_map.at(result)==0) return -1; else return result;
}

void MyFileLoad::loaded()
{
    MyClientChanal *chanal;

    m_timer.stop();
    disconnect(&m_timer,0,this,0);

    for(qint32 i=0; i<m_chanals.size();i++)
    {
        chanal=m_chanals.at(i);
        disconnect(chanal,0,0,0);
        delete chanal;
    }
    m_chanals.clear();

    m_file.close();
    bool result=true;
    for(qint32 i=0;i<m_map.size();i++)
        if(m_map.at(i)!=0) { result=false; break; }

    if(result)
    {
        removeTorrent();
        m_state=2;
    }
    else
    {
        saveTorrent();
        m_state=3;
    }
}

void MyFileLoad::sendFileName(MyClientChanal *chanal)
{
    MyPacket packet;
    packet.setCmd(cmdSetFileName);
    packet.setFileBlock(m_messageID,0,0);
    chanal->sendPacket(packet);

    m_attempt++;
    m_tick=0;
    qDebug() << "send file name" << chanal;
}

void MyFileLoad::recvFileParam(MyClientChanal *chanal, MyPacket &packet)
{
    if((packet.getCmd()==(cmdSetFileName|cmdAnswer)) && packet.isFileBlock())
    {
        m_block=packet.getFileBlock();

        if(chanal==m_chanal)
        {
            prepareFile();
            m_chanals_state[0]=1;
            m_state=1;
            if(m_map.size()==0) loaded();
        }

        for(qint32 i=0;i<m_chanals.size();i++)
            if(m_chanals.at(i)==chanal)
            {
                m_chanals_state[i]=1;
                break;
            }

        m_attempt=0;
        m_tick=0;
    }
}

qint32 MyFileLoad::sendQueryData(MyClientChanal *chanal)
{
    MyPacket packet;
    MyFileBlock block;
    block.fileID=m_messageID;

    qint64 pos=getBlock();
    if(pos!=-1)
    {
        block.pos=pos;
        block.mask=m_map.at(pos);

        packet.setCmd(cmdGetData);
        packet.setFileBlock(block);
        chanal->sendPacket(packet);

        qDebug() << "query" << block.pos << QString(" - %1").arg(block.mask,0,16) << chanal;
    }
    return (pos==-1)?0:1;
}

void MyFileLoad::recvFileData(MyClientChanal *chanal, MyPacket &packet)
{
    Q_UNUSED(chanal)

    if((packet.getCmd()!=cmdSendData) || (!packet.isFileBlock()) || (!packet.isData())) return;
    MyFileBlock block=packet.getFileBlock();
    QByteArray data=packet.getData();

    if((block.pos>=0)&&(block.pos<m_map.size())&&block.mask)
    {
        if(data.size()>1024) data.resize(1024);

        qint64 pos=block.pos<<13;
        quint8 mask=m_map.at(block.pos);
        quint8 kByte=0x1;

        if(mask&block.mask)
        {
            while((block.mask&kByte)==0)
            {
                pos+=1024;
                kByte<<=1;
            }
            m_file.seek(pos);
            m_file.write(data);
            mask^=kByte;
            m_map[qint32(block.pos)]=mask;
        }
    }
}

void MyFileLoad::sendEndSession(MyClientChanal *chanal)
{
    MyPacket packet;
    packet.setCmd(cmdEndSession);
    chanal->sendPacket(packet);
}

void MyFileLoad::createChanal()
{
    if(m_chanals_count<=0) return;

    MyClientChanal * chanal=new MyClientChanal(this);
    connect(chanal, SIGNAL(status(MyClientChanal*,qint32)),
            this,   SLOT  (statusClient(MyClientChanal*,qint32)));
    connect(chanal, SIGNAL(receivePacket(MyClientChanal*,MyPacket&)),
            this,   SLOT  (recvPacket(MyClientChanal*,MyPacket&)));

    chanal->execute(m_config,m_userID,typeFile);

    m_chanals.append(chanal);
    m_chanals_state.append(0);
    m_chanals_count--;
}

void MyFileLoad::saveTorrent()
{
    QByteArray map;
    map.resize(m_map.size());
    for(qint32 i=0;i<m_map.size();i++)
        map[i]=char(m_map.at(i));

    QUrl path(m_filename+"_map");
    QFile file;
    file.setFileName(path.toLocalFile());
    file.open(QIODevice::ReadWrite);

    file.write(map);
    file.close();
}

bool MyFileLoad::loadTorrent()
{
    bool result=false;
    QByteArray map;
    QUrl path(m_filename+"_map");
    QFile file;
    file.setFileName(path.toLocalFile());

    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        map=file.readAll();
        file.close();

        if(map.size()==m_map.size())
        {
            for(qint32 i=0;i<m_map.size();i++)
                m_map[i]=quint8(map.at(i));
            result=true;
        }
    }
    return result;
}

void MyFileLoad::removeTorrent()
{
    QUrl path(m_filename+"_map");
    QFile file;
    file.setFileName(path.toLocalFile());
    file.remove();
}

MyFileLoad::MyFileLoad(QObject *parent) :
    QObject(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timer100msec()));
}

void MyFileLoad::load(MyConfig *config, QString filename)
{
    m_config=config;
    m_filename=filename;
    QUrl path(m_filename);
    m_file.setFileName(path.toLocalFile());

    // отметим прием файла
    QString fname=path.fileName();

    qint32 def1, def2;
    def1=fname.indexOf("-",0);
    def2=fname.indexOf("-",def1+1);

    m_userID=fname.mid(0,def1).toInt();
    m_messageID=fname.mid(def1+1,def2-def1-1).toInt();

    m_chanals_count=0;
    m_chanals.clear();
    m_chanals_state.clear();

    m_chanal=new MyClientChanal(this);
    connect(m_chanal, SIGNAL(status(MyClientChanal*,qint32)),
            this,     SLOT  (statusClient(MyClientChanal*,qint32)));
    connect(m_chanal, SIGNAL(receivePacket(MyClientChanal*,MyPacket&)),
            this,     SLOT  (recvPacket(MyClientChanal*,MyPacket&)));

    m_chanal->execute(m_config,m_userID,typeFile);

    m_state=0;
    m_tick=0;
    m_attempt=0;
    qDebug() << "load" <<m_filename;
}

qint32 MyFileLoad::getState()
{
    return m_state;
}

qint32 MyFileLoad::getUserID()
{
    return m_userID;
}

QString MyFileLoad::getFileName()
{
    return m_filename;
}

void MyFileLoad::timer100msec()
{
    m_tick++;

    if(m_state==1)
    {
        if(m_chanals.size()==0) loaded();

        qint32 n=0;
        for(qint32 i=0;i<m_chanals.size();i++)
            if(m_chanals_state.at(i)==1)
                n+=sendQueryData(m_chanals.at(i));
            else
            {
                if(m_chanals_state.at(i)<-1) m_chanals.at(i)->terminate(); else
                    if(m_chanals.at(i)->getIdle()>3)
                    {
                        sendFileName(m_chanals.at(i));
                        m_chanals_state[i]=m_chanals_state.at(i)-1;
                    }
                n++;
            }

        if(n==0) loaded();
    }

    if(m_tick<10) return;

    if(m_attempt>5)
        loaded();
    else
    {
        if(m_state==0) sendFileName(m_chanal);
        createChanal();
        m_tick=0;
    }
}

void MyFileLoad::recvPacket(MyClientChanal *chanal, MyPacket &packet)
{
    for(qint32 i=0;i<m_chanals.size();i++)
        if(m_chanals.at(i)==chanal)
        {
            switch(m_chanals_state.at(i))
            {
                case 0: recvFileParam(chanal,packet); break;
                case 1: recvFileData(chanal,packet); break;
            }
            break;
        }
}

void MyFileLoad::statusClient(MyClientChanal *chanal, qint32 status)
{
    qDebug() << "status" << status << chanal;
    switch(status)
    {
        case 2:
            if(chanal==m_chanal)
            {
                m_chanals.append(m_chanal);
                m_chanals_state.append(0);
                m_timer.start(100);
            }
            sendFileName(chanal);
            break;
        case 3:
            for(qint32 i=0; i<m_chanals.size();i++)
            {
                if(chanal==m_chanals.at(i))
                {
                    m_chanals.removeAt(i);
                    m_chanals_state.removeAt(i);
                    break;
                }
            }
            disconnect(chanal,0,0,0);
            delete chanal;

            if(m_chanals.size()==0) loaded();
            break;
    }
}
