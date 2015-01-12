#include "myfilemanager.h"
#include "applicationmodel.h"
#include "user.h"
#include "message.h"
#include "messagesmodel.h"
#include <QUrl>

void MyFileManager::sound(qint32 state)
{
    MyRingtone *ringtone=new MyRingtone(this);
    connect(ringtone,SIGNAL(stopped(MyRingtone *)),this,SLOT(stopRingtone(MyRingtone *)));

    switch(state)
    {
        case 2: ringtone->play(m_config->getHomeDir()+"ringtone/file_load.wav"); break;
        case 3: ringtone->play(m_config->getHomeDir()+"ringtone/file_not_load.wav"); break;
    }

}

void MyFileManager::mark(qint32 userID, QString fileName)
{
    ApplicationModel * appModel=ApplicationModel::instance();
    User *u;
    QUrl path(fileName);

    for(qint32 i=0;i<appModel->contactsModel()->count();i++)
    {
        u=(User *)appModel->contactsModel()->get(i);
        if(u->id()==userID)
        {
            MessagesModel *chat=(MessagesModel *)u->messagesModel();
            for(qint32 j=0;j<chat->count();j++)
            {
                Message *msg=(Message *)chat->get(j);
                if(msg->filePath()==path)
                {
                    msg->setWasRead(true);
                    chat->save();
                    qDebug() << "mark" << userID << fileName;
                    break;
                }
            }

        }
    }
}

MyFileManager::MyFileManager(QObject *parent) :
    QObject(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timer1sec()));
}

MyFileManager::~MyFileManager()
{
    m_timer.stop();
    disconnect(&m_timer,0,this,0);
}

void MyFileManager::init(MyConfig *config)
{
    m_config=config;
    m_list.clear();
    m_timer.start(1000);
}

void MyFileManager::loadFile(QString filename)
{
    for(qint32 i=0;i<m_list.size();i++)
    {
        if(m_list.at(i)->getFileName()==filename) return;
    }

    MyFileLoad *item=new MyFileLoad(this);
    m_list.append(item);
    item->load(m_config, filename);
}

void MyFileManager::addConnect(MyClientChanal *chanal)
{
    if(chanal->getTypeChanal()!=typeFile) return;

    qDebug() << "add file chanal" << chanal->getUserID() << chanal->getTypeChanal();

    MyFileUnload *item=new MyFileUnload(this);
    item->unload(m_config, chanal);
}

void MyFileManager::timer1sec()
{
    MyFileLoad *item;
    for(qint32 i=0;i<m_list.size();i++)
    {
        item=m_list.at(i);
        if(item->getState()>1)
        {
            sound(item->getState());
            if(item->getState()==2) mark(item->getUserID(),item->getFileName());
            item->deleteLater();
            m_list.removeAt(i);
            break;
        }
    }
}

void MyFileManager::stopRingtone(MyRingtone *ringtone)
{
    disconnect(ringtone,0,0,0);
    ringtone->deleteLater();
}
