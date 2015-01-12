#include "myaudiomanager.h"
#include "applicationmodel.h"

void MyAudioManager::checkOnline()
{
    ApplicationModel * appModel=ApplicationModel::instance();
    User *u;
    qint32 UserID=-1;
    if(m_list.size()!=0)
        if(m_list.at(0)->getChanal()->getStatus()==2)
            UserID=m_list.at(0)->getChanal()->getUserID();

//    qDebug() << "check online" << UserID;

    for(qint32 i=0;i<appModel->contactsModel()->count();i++)
    {
        u=(User *)appModel->contactsModel()->get(i);
        if(u->id()==UserID)
            u->userInfo()->setOnlinea(true);
        else
            u->userInfo()->setOnlinea(false);
    }
}

MyAudioManager::MyAudioManager(QObject *parent) :
    QObject(parent)
{
    m_list.clear();
    m_audioIn=NULL;
}

MyAudioManager::~MyAudioManager()
{
    MyAudioItem *item;
    for(qint32 i=0;i<m_list.count();i++)
    {
        item=m_list.at(i);
        disconnect(item,0,0,0);
        delete item;
    }
    m_list.clear();

    if(m_audioIn)
    {
        disconnect(m_audioIn, 0, this,0);
        m_audioIn->exitThr();
        m_audioIn->deleteLater();
    }

    checkOnline();
}

void MyAudioManager::init(MyConfig *config)
{
    m_config=config;
    m_ttl=0;
}

qint32 MyAudioManager::getCountConnect()
{
    return m_list.count();
}

qint32 MyAudioManager::getUserID(qint32 index)
{
    qint32 userID=0;
    if((index>=0)&&(index<m_list.count()))
    {
        userID=m_list.at(index)->getChanal()->getUserID();
    }
    return userID;
}

MyClientChanal *MyAudioManager::getChanal(qint32 index)
{
    MyClientChanal *chanal=NULL;
    if((index>=0)&&(index<m_list.count()))
    {
        chanal=m_list.at(index)->getChanal();
    }
    return chanal;
}

void MyAudioManager::Connect(qint32 UserID, bool proxy)
{
    MyAudioItem *item;
    MyClientChanal *chanal;
    if(m_list.size()!=0)
    {
        item=m_list.at(0);
        qint32 nUserID=item->getChanal()->getUserID();
        disconnect(item,0,0,0);
        delete item;
        m_list.clear();

        disconnect(m_audioIn, 0, this, 0);
        m_audioIn->exitThr();
        m_audioIn->deleteLater();
        m_audioIn=NULL;

        if(nUserID==UserID) UserID=-1;

        qDebug() << "close audio chanal" << nUserID;
    }

    if(UserID!=-1)
    {
        qDebug() << "create audio chanal" << UserID;
        chanal=new MyClientChanal(this);
        chanal->execute(m_config,UserID,typeAudio,proxy);

        item=new MyAudioItem(chanal);
        connect(item, SIGNAL(newStatus(qint32,qint32)), this, SLOT(statusClient(qint32,qint32)));
        m_list.append(item);

        m_audioIn=new MyAudioIn;
        connect(m_audioIn, SIGNAL(capturedData(QByteArray&)), this, SLOT(sendAudio(QByteArray&)));
        m_audioIn->startThr();
    }

    checkOnline();
}

void MyAudioManager::Disconnect(qint32 UserID)
{
    MyAudioItem *item;
    if(m_list.size()!=0)
    {
        item=m_list.at(0);
        disconnect(item,0,0,0);
        delete item;
        m_list.clear();

        if(m_audioIn)
        {
            disconnect(m_audioIn, 0, this, 0);
            m_audioIn->exitThr();
            m_audioIn->deleteLater();
            m_audioIn=NULL;
        }

        qDebug() << "close audio chanal" << UserID;
    }
}

void MyAudioManager::addConnect(MyClientChanal *chanal)
{
    if(chanal->getTypeChanal()!=typeAudio) return;

    qDebug() << "add audio chanal" << chanal->getUserID() << chanal->getTypeChanal();

    MyAudioItem *item;
    if(m_list.size()==0)
    {
        qDebug() << "create audio chanal" << chanal->getUserID();

        item=new MyAudioItem(chanal);
        connect(item, SIGNAL(newStatus(qint32,qint32)), this, SLOT(statusClient(qint32,qint32)));
        m_list.append(item);

        m_audioIn=new MyAudioIn;
        connect(m_audioIn, SIGNAL(capturedData(QByteArray&)), this, SLOT(sendAudio(QByteArray&)));
        m_audioIn->startThr();

        checkOnline();
    }
    else
    {
        chanal->terminate();
        delete chanal;
    }
}

void MyAudioManager::statusClient(qint32 UserID, qint32 status)
{
    bool need_proxy=false;
    if(status==3)
    {
        if(m_list.size()>0)
        {
            MyAudioItem *item=m_list.at(0);
            need_proxy=( item->getChanal()->isConnected() &&
                        !item->getChanal()->isProxy() &&
                        (item->getChanal()->getDirection()==0));
        }
        Disconnect(UserID);
        if(need_proxy) Connect(UserID,true);
    }
    checkOnline();
}

void MyAudioManager::sendAudio(QByteArray &data)
{
    MyPacket packet;
    packet.Clear();
    packet.setCmd(cmdSendAudio);
    QAudioFormat format=m_audioIn->getFormat();
    packet.setAudio(m_config->getUserID(),
                    m_ttl,
                    format.sampleRate(),
                    format.channelCount(),
                    format.sampleSize());
    m_ttl++;

    packet.setData(data);
    for(qint32 i=0;i<m_list.size();i++)
        m_list[i]->getChanal()->sendPacket(packet);
}
