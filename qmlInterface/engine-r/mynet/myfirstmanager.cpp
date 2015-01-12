#include "myfirstmanager.h"

MyFirstManager::MyFirstManager(QObject *parent) :
    QObject(parent)
{
    m_list.clear();
}

MyFirstManager::~MyFirstManager()
{
    MyClientChanal *chanal;
    for(qint32 i=0;i<m_list.count();i++)
    {
        chanal=m_list.at(i);
        chanal->terminate();
        chanal->deleteLater();
    }
    m_list.clear();
}

void MyFirstManager::init(MyConfig *config)
{
    m_config=config;
}

qint32 MyFirstManager::getCountConnect()
{
    return m_list.count();
}

qint32 MyFirstManager::getUserID(qint32 index)
{
    qint32 userID=0;
    if((index>=0)&&(index<m_list.count()))
    {
        userID=m_list[index]->getUserID();
    }
    return userID;
}

MyClientChanal *MyFirstManager::getChanal(qint32 index)
{
    MyClientChanal *chanal=NULL;
    if((index>=0)&&(index<m_list.count()))
    {
        chanal=m_list[index];
    }
    return chanal;
}

void MyFirstManager::recvServer(MyPacket &packet)
{
    MyClientChanal *chanal;
    if((packet.getCmd()==cmdConnect) && packet.isConnectBlock())
    {
        qDebug() << "first" << packet.getCmd();

        chanal=new MyClientChanal;
        connect(chanal, SIGNAL(status(MyClientChanal*,qint32)),
                this,   SLOT(statusClient(MyClientChanal*,qint32)));

        chanal->execute(m_config,packet);
        m_list.append(chanal);
    }
}

void MyFirstManager::statusClient(MyClientChanal *chanal, qint32 status)
{
    qint32 n=-1;
    MyClientChanal *tmp;

    if(status==1) return;

    for(qint32 i=0;i<m_list.count();i++)
    {
        tmp=m_list.at(i);
        if(tmp==chanal)
        {
            n=i;
            break;
        }
    }
    if(n==-1) return;
    m_list.removeAt(n);

    disconnect(chanal,0,this,0);

    // канал готов
    if(status==2)
    {
        emit newConnect(chanal);
    }

    // закрытие канала
    if(status==3)
    {
        chanal->deleteLater();
    }
}
