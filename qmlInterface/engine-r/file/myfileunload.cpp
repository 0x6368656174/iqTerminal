#include "myfileunload.h"
#include "applicationmodel.h"
#include "user.h"
#include "message.h"
#include "messagesmodel.h"
#include "../crypto/myhash.h"
#include <QFileInfo>

bool MyFileUnload::findFileInChat()
{
    bool result=false;
    ApplicationModel * appModel=ApplicationModel::instance();
    User *u;

    for(qint32 i=0;i<appModel->contactsModel()->count();i++)
    {
        u=(User *)appModel->contactsModel()->get(i);
        if(u->id()==m_userID)
        {
            MessagesModel *chat=(MessagesModel *)u->messagesModel();
            for(qint32 j=0;j<chat->count();j++)
            {
                Message *msg=(Message *)chat->get(j);
                if(msg->id()==m_messageID)
                {
                    QString filename=msg->filePath().toLocalFile();
                    if(QFileInfo(filename).exists())
                    {
                        m_file.setFileName(filename);
                        m_file.open(QIODevice::ReadOnly);

                        qDebug() << "unload" << m_file.fileName() << m_file.size();
                        result=true;
                    }
                    break;
                }
            }
        }
    }
    return result;
}

void MyFileUnload::endUnload()
{
    qDebug() << "end unload";

    m_timer.stop();
    disconnect(&m_timer,0,this,0);

    m_file.close();

    disconnect(m_chanal,0,this,0);
    delete m_chanal;

    deleteLater();
}

void MyFileUnload::recvFileParam(MyPacket &packet)
{
    if(packet.isFileBlock())
    {

        qint32 userID=packet.getUserID();
        MyFileBlock block=packet.getFileBlock();

        qDebug() << "recv filename" << userID << block.fileID;

        if((m_userID!=-1)&&(m_userID!=userID)) return;
        if((m_messageID!=-1)&&(m_messageID!=block.fileID)) return;

        if(m_messageID==-1)
        {
            m_userID=userID;
            m_messageID=block.fileID;
            if(!findFileInChat())
            {
                m_userID=-1;
                m_messageID=-1;
            }
        }

        if(m_messageID!=-1)
        {
            sendFileParam();
            m_attempt=0;
        }
    }
}

void MyFileUnload::sendFileParam()
{
    MyPacket packet;
    MyFileBlock block;
    packet.setCmd(cmdSetFileName|cmdAnswer);
    block.fileID=m_messageID;
    block.pos=m_file.size();
    block.mask=0;
    packet.setFileBlock(block);
    m_chanal->sendPacket(packet);
    m_state=1;
}

void MyFileUnload::recvQueryData(MyPacket &packet)
{
    if(m_state!=1) return;
    if(packet.isFileBlock())
    {
        MyFileBlock block=packet.getFileBlock();
        MyPacket newpacket;
        QByteArray data;

        qint64 pos=block.pos<<13;
        quint8 mask=block.mask;
        quint8 kByte=0x1;

        for(qint32 i=0;i<8;i++)
        {
            if(mask&kByte)
            {
                m_file.seek(pos);
                data=m_file.read(1024);
                block.mask=kByte;

                newpacket.Clear();
                newpacket.setCmd(cmdSendData);
                newpacket.setFileBlock(block);
                newpacket.setData(data);
                m_chanal->sendPacket(newpacket);
            }
            kByte<<=1;
            pos+=1024;
        }
        m_attempt=0;
    }
}

MyFileUnload::MyFileUnload(QObject *parent) :
    QObject(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timer100msec()));
}

void MyFileUnload::unload(MyConfig *config, MyClientChanal *chanal)
{
    m_config=config;
    m_chanal=chanal;
    connect(m_chanal, SIGNAL(status(MyClientChanal*,qint32)),
            this,     SLOT  (statusClient(MyClientChanal*,qint32)));
    connect(m_chanal, SIGNAL(receivePacket(MyClientChanal*,MyPacket&)),
            this,     SLOT  (recvPacket(MyClientChanal*,MyPacket&)));

    m_state=0;
    m_attempt=0;
    m_userID=-1;
    m_messageID=-1;

    m_timer.start(1000);
    qDebug() << "upload file";
}

void MyFileUnload::timer100msec()
{
    if(m_attempt++>10)
    {
        qDebug() << "timeout";
        endUnload();
    }
}

void MyFileUnload::recvPacket(MyClientChanal *chanal, MyPacket &packet)
{
    Q_UNUSED(chanal)

    if(packet.getCmd()==cmdGetData) recvQueryData(packet); else
        if(packet.getCmd()==cmdSetFileName) recvFileParam(packet);
}

void MyFileUnload::statusClient(MyClientChanal *chanal, qint32 status)
{
    Q_UNUSED(chanal)
    qDebug() << "status" << status;
    if(status==3)
    {
        endUnload();
    }
}
