#include "myvideomanager.h"
#include "user.h"
#include "usersmodel.h"
#include "applicationmodel.h"

#include <QDebug>

void MyVideoManager::checkOnline()
{
    ApplicationModel * appModel=ApplicationModel::instance();
    User *u;
    qint32 UserID=-1;
    if(m_list.size()!=0)
        if(m_list.at(0)->getChanal()->getStatus()==2)
            UserID=m_list.at(0)->getChanal()->getUserID();

    for(qint32 i=0;i<appModel->contactsModel()->count();i++)
    {
        u=(User *)appModel->contactsModel()->get(i);
        if(u->id()==UserID)
            u->userInfo()->setOnlinev(true);
        else
            u->userInfo()->setOnlinev(false);
    }
}

MyVideoManager::MyVideoManager(QObject *parent) :
    QObject(parent)
{
    m_list.clear();
}

MyVideoManager::~MyVideoManager()
{
    MyVideoItem *item;
    for(qint32 i=0;i<m_list.count();i++)
    {
        item=m_list.at(i);
        disconnect(item,0,0,0);
        delete item;
    }
    m_list.clear();

    checkOnline();
}

void MyVideoManager::init(MyConfig *config)
{
    m_config=config;
    m_ttl=0;
}

qint32 MyVideoManager::getCountConnect()
{
    return m_list.size();
}

qint32 MyVideoManager::getUserID(qint32 index)
{
    qint32 userID=0;
    if((index>=0)&&(index<m_list.count()))
    {
        userID=m_list.at(index)->getChanal()->getUserID();
    }
    return userID;
}

MyClientChanal *MyVideoManager::getChanal(qint32 index)
{
    MyClientChanal *chanal=NULL;
    if((index>=0)&&(index<m_list.count()))
    {
        chanal=m_list.at(index)->getChanal();
    }
    return chanal;
}

void MyVideoManager::Connect(qint32 UserID, bool proxy)
{
    MyVideoItem *item;
    MyClientChanal *chanal;
    if(m_list.size()!=0)
    {
        item=m_list.at(0);
        qint32 nUserID=item->getChanal()->getUserID();
//        disconnect(item,0,0,0);
//        delete item;
//        m_list.clear();

        if(nUserID==UserID) UserID=-1;

//        qDebug() << "close video chanal" << nUserID;
    }

    if(UserID!=-1)
    {
        qDebug() << "create video chanal" << UserID;
        chanal=new MyClientChanal(this);
        chanal->execute(m_config,UserID,typeVideo,proxy);

        item=new MyVideoItem(chanal);
        connect(item, SIGNAL(newStatus(qint32,qint32)), this, SLOT(statusClient(qint32,qint32)));
        connect(item, SIGNAL(updateVideoIn(QImage)), this, SIGNAL(updateVideoIn(QImage)));
        m_list.append(item);
    }

    checkOnline();
}

void MyVideoManager::Disconnect(qint32 UserID)
{
    MyVideoItem *item;
    if(m_list.size()!=0)
    {
        item=m_list.at(0);
        disconnect(item,0,0,0);
        delete item;
        m_list.clear();

        qDebug() << "close video chanal" << UserID;
    }
    checkOnline();
}

void MyVideoManager::addConnect(MyClientChanal *chanal)
{
    if(chanal->getTypeChanal()!=typeVideo) return;

    qDebug() << "add video chanal" << chanal->getUserID() << chanal->getTypeChanal();

    MyVideoItem *item;
    if(m_list.size()==0)
    {
        qDebug() << "create video chanal" << chanal->getUserID();

        item=new MyVideoItem(chanal);
        connect(item, SIGNAL(newStatus(qint32,qint32)), this, SLOT(statusClient(qint32,qint32)));
        connect(item, SIGNAL(updateVideoIn(QImage)), this, SIGNAL(updateVideoIn(QImage)));
        m_list.append(item);

        checkOnline();
    }
    else
    {
        chanal->terminate();
        delete chanal;
    }
}

void MyVideoManager::statusClient(qint32 UserID, qint32 status)
{
    bool need_proxy=false;
    if(status==3)
    {
        if(m_list.size()>0)
        {
            MyVideoItem *item=m_list.at(0);
            need_proxy=( item->getChanal()->isConnected() &&
                        !item->getChanal()->isProxy() &&
                        (item->getChanal()->getDirection()==0));
        }
        Disconnect(UserID);
        if(need_proxy) Connect(UserID,true);
    }
    checkOnline();
}

void MyVideoManager::updateVideoOut(QImage kadr)
{
    for(qint32 i=0;i<m_list.size();i++)
        m_list.at(i)->updateVideoOut(kadr);
}
