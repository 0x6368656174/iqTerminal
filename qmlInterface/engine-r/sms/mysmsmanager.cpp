#include "mysmsmanager.h"
#include "applicationmodel.h"

QByteArray MySMSManager::encodeMessage(Message *msg)
{
    qint32 l;
    char *ptr;
    QByteArray tmp;
    QByteArray data;
    qint32 id=msg->id();
    Message::Type typeMsg=msg->type();
    QString filePath=msg->filePath().toString();

    ptr=(char *)&id;
    data.append(ptr,sizeof(qint32));

    ptr=(char *)&typeMsg;
    data.append(ptr,sizeof(Message::Type));

    tmp=msg->text().toUtf8();
    l=tmp.size();
    ptr=(char *)&l;
    data.append(ptr,sizeof(qint32));
    data.append(tmp);

    tmp=filePath.toUtf8();
    l=tmp.size();
    ptr=(char *)&l;
    data.append(ptr,sizeof(qint32));
    data.append(tmp);

    return data;
}

qint32 MySMSManager::decodeMessage(Message *msg, QByteArray &data)
{
    char *ptr;

    qint32 id;
    Message::Type typeMsg;
    QString text;
    QString filePath;
    qint32 l;

    msg->setSendDateTime(QDateTime::currentDateTime());
    msg->setDirection(Message::Incoming);

    qint32 index=0;
    ptr=(char *)&id;
    for(quint32 i=0;i<sizeof(qint32);i++)
    {
        ptr[i]=data.at(index);
        index++;
    }

    ptr=(char *)&typeMsg;
    for(quint32 i=0;i<sizeof(Message::Type);i++)
    {
        ptr[i]=data.at(index);
        index++;
    }
    msg->setType(typeMsg);

    ptr=(char *)&l;
    for(quint32 i=0;i<sizeof(qint32);i++)
    {
        ptr[i]=data.at(index);
        index++;
    }

    text.append(data.mid(index,l));
    msg->setText(text);
    index+=l;

    ptr=(char *)&l;
    for(quint32 i=0;i<sizeof(qint32);i++)
    {
        ptr[i]=data.at(index);
        index++;
    }

    filePath.append(data.mid(index,l));
    msg->setFilePath(filePath);

    return id;
}

void MySMSManager::addSms(qint32 UserID)
{
    bool isnew=true;
    for(int i=0;i<m_userID.count();i++)
    {
        if(m_userID.at(i)==UserID)
        {
            isnew=false; break;
        }
    }

    if(isnew)
    {
        m_userID.append(UserID);

        ApplicationModel * appModel=ApplicationModel::instance();
        appModel->setChatNotEmpty(true);

        User *u;

        for(qint32 i=0;i<appModel->contactsModel()->count();i++)
        {
            u=(User *)appModel->contactsModel()->get(i);
            if(u->id()==UserID)
            {
                u->userInfo()->setSms(true);
                break;
            }
        }
    }
    sound();
}

void MySMSManager::delSms(qint32 UserID)
{
    for(int i=0;i<m_userID.count();i++)
    {
        if(m_userID.at(i)==UserID)
        {
            m_userID.removeAt(i);
            break;
        }
    }

    ApplicationModel * appModel=ApplicationModel::instance();
    if(m_userID.count()==0)
    {
        appModel->setChatNotEmpty(false);
    }

    User *u;
    for(qint32 i=0;i<appModel->contactsModel()->count();i++)
    {
        u=(User *)appModel->contactsModel()->get(i);
        if(u->id()==UserID)
        {
            u->userInfo()->setSms(false);
            break;
        }
    }
}

void MySMSManager::sound()
{
    MyRingtone *ringtone=new MyRingtone(this);
    connect(ringtone,SIGNAL(stopped(MyRingtone *)),this,SLOT(stopRingtone(MyRingtone *)));
    ringtone->play(m_config->getHomeDir()+"ringtone/sms_in.wav");
}

void MySMSManager::addChanal(qint32 UserID, bool proxy)
{
    MyClientChanal *chanal=new MyClientChanal(this);
    m_list.append(chanal);

    connect(chanal, SIGNAL(status(MyClientChanal*,qint32)),
            this,   SLOT  (statusClient(MyClientChanal*,qint32)));
    connect(chanal, SIGNAL(receivePacket(MyClientChanal*,MyPacket&)),
            this,   SLOT  (recvSMS(MyClientChanal*,MyPacket&)));

    chanal->execute(m_config,UserID,typeSMS,proxy);
}

MySMSManager::MySMSManager(QObject *parent) :
    QObject(parent)
{
    m_list.clear();
    m_isStarted=false;
}

MySMSManager::~MySMSManager()
{
    MyClientChanal *chanal;
    for(qint32 i=0;i<m_list.count();i++)
    {
        chanal=m_list[i];
        delete chanal;
    }
    m_list.clear();

    delete m_proxy;
}

void MySMSManager::init(MyConfig *config)
{
    m_config=config;
    m_proxy=new MyProxyChanal;
    connect(m_proxy,SIGNAL(recvMessage(qint32,MyPacket&,bool)),this,SLOT(recvSMS(qint32,MyPacket&,bool)));
    connect(this,SIGNAL(sendToProxy(qint32,MyPacket&)),m_proxy,SLOT(sendMessage(qint32,MyPacket&)));
}

void MySMSManager::execute()
{
    if(m_isStarted) return;
    m_isStarted=true;
    m_proxy->execute(m_config);
}

qint32 MySMSManager::getCountConnect()
{
    return m_list.count();
}

qint32 MySMSManager::getUserID(qint32 index)
{
    qint32 userID=0;
    if((index>=0)&&(index<m_list.count()))
    {
        userID=m_list[index]->getUserID();
    }
    return userID;
}

MyClientChanal *MySMSManager::getChanal(qint32 index)
{
    MyClientChanal *chanal=NULL;
    if((index>=0)&&(index<m_list.count()))
    {
        chanal=m_list[index];
    }
    return chanal;
}

void MySMSManager::Connect(qint32 index)
{

    ApplicationModel * appModel=ApplicationModel::instance();
    User *u=(User *)appModel->contactsModel()->get(index);

    qint32 UserID=u->id();
    qDebug() << "create sms chanal" << UserID;

    bool newconnect=true;
    for(qint32 i=0;i<m_list.count();i++)
    {
        if(m_list[i]->getUserID()==UserID)
        {
            newconnect=false;
            break;
        }
    }

    if(newconnect) addChanal(UserID, false);
}

void MySMSManager::Disconnect(qint32 UserID)
{
    for(qint32 i=0;i<m_list.count();i++)
    {
        if(m_list[i]->getUserID()==UserID)
        {
            m_list[i]->terminate();
            break;
        }
    }
}

void MySMSManager::sendMessage(qint32 index, Message *msg)
{
    ApplicationModel * appModel=ApplicationModel::instance();
    User *u=(User *)appModel->contactsModel()->get(index);

    qint32 UserID=u->id();

    MyPacket packet;
    QByteArray data;
    packet.setCmd(cmdSendMessage);
    data=encodeMessage(msg);
    packet.setData(data);

    bool flag=false;
    for(qint32 i=0;i<m_list.count();i++)
    {
        if((m_list[i]->getUserID()==UserID) &&(m_list[i]->getStatus()==2))
        {
            m_list[i]->sendPacket(packet);
            flag=true;
        }
    }

    if(!flag)
    {
        emit sendToProxy(UserID,packet);
        if(msg->type()!=Message::Text)
        {
            qDebug() << "send file" << UserID << msg->id() << msg->filePath().toString();
            MyProxyTcp *chanal=new MyProxyTcp(this);
            connect(chanal,SIGNAL(Exit(MyProxyTcp*,qint32,qint32,QString)),
                    this,SLOT(exitProxy(MyProxyTcp*,qint32,qint32,QString)));
            chanal->unloadFile(m_config,UserID,msg->id(),msg->filePath().toString());
        }
        addChanal(UserID, false);
    }
}

void MySMSManager::checkSms()
{
    ApplicationModel * appModel=ApplicationModel::instance();
    User *u;

    for(qint32 i=0;i<appModel->contactsModel()->count();i++)
    {
        u=(User *)appModel->contactsModel()->get(i);
        u->userInfo()->setSms(false);
        for(qint32 j=0;j<m_userID.count();j++)
        {
            if(u->id()==m_userID.at(j))
            {
                u->userInfo()->setSms(true);
                break;
            }
        }
    }
}

void MySMSManager::checkOnline()
{
    ApplicationModel * appModel=ApplicationModel::instance();
    User *u;

    for(qint32 i=0;i<appModel->contactsModel()->count();i++)
    {
        u=(User *)appModel->contactsModel()->get(i);
        u->userInfo()->setOnlinec(false);
        for(qint32 j=0;j<m_list.count();j++)
        {
            if((u->id()==m_list.at(j)->getUserID())&&(m_list.at(j)->getStatus()==2))
            {
                u->userInfo()->setOnlinec(true);
                break;
            }
        }
    }
}

void MySMSManager::clearSms(int index)
{
    ApplicationModel * appModel=ApplicationModel::instance();
    if((index<0)||(index>=appModel->contactsModel()->count())) return;

    User *u=(User *)appModel->contactsModel()->get(index);
    delSms(u->id());
    u->userInfo()->setSms(false);
}

bool MySMSManager::isStarted()
{
    return m_isStarted;
}

void MySMSManager::recvSMS(MyClientChanal *chanal, MyPacket &packet)
{
    recvSMS(chanal->getUserID(),packet,false);
}

void MySMSManager::statusClient(MyClientChanal *chanal, qint32 status)
{
    qint32 n=-1;
    MyClientChanal *tmp;

    for(qint32 i=0;i<m_list.count();i++)
    {
        tmp=m_list[i];
        if(tmp==chanal)
        {
            n=i;
            break;
        }
    }
    if(n==-1) return;

    // закрытие канала
    if(status==3)
    {
        m_list.removeAt(n);
        disconnect(chanal,0,this,0);

        if(chanal->isConnected() && !chanal->isProxy() &&(chanal->getDirection()==0))
        {
            addChanal(chanal->getUserID(),true);
        }
        delete chanal;
    }

    checkOnline();
}

void MySMSManager::addConnect(MyClientChanal *chanal)
{
    if(chanal->getTypeChanal()!=typeSMS) return;

    qDebug() << "add sms chanal" << chanal->getUserID() << chanal->getTypeChanal();

    bool tmp_newconnect=true;
    for(qint32 i=0;i<m_list.count();i++)
    {
        if(m_list.at(i)->getUserID()==chanal->getUserID())
        {
            tmp_newconnect=false;
            break;
        }
    }

    if(tmp_newconnect)
    {
        connect(chanal, SIGNAL(status(MyClientChanal*,qint32)),
                this,   SLOT  (statusClient(MyClientChanal*,qint32)));
        connect(chanal, SIGNAL(receivePacket(MyClientChanal*,MyPacket&)),
                this,   SLOT  (recvSMS(MyClientChanal*,MyPacket&)));

        m_list.append(chanal);
        checkOnline();
    }
    else
    {
        chanal->terminate();
        delete chanal;
    }
}

void MySMSManager::stopRingtone(MyRingtone *ringtone)
{
    disconnect(ringtone,0,0,0);
    ringtone->deleteLater();
}

void MySMSManager::recvSMS(qint32 UserID, MyPacket &packet, bool proxy)
{
    ApplicationModel * appModel=ApplicationModel::instance();
    User *u;

    if((packet.getCmd()==cmdSendMessage) && packet.isData())
    {
        for(qint32 i=0;i<appModel->contactsModel()->count();i++)
        {
            u=(User *)appModel->contactsModel()->get(i);
            if(u->id()==UserID)
            {
                QByteArray data=packet.getData();

                Message *newMessage=(Message *)u->messagesModel()->appendNew();
                qint32 id=decodeMessage(newMessage,data);

                if(newMessage->type()==Message::Text)
                {
                    newMessage->setWasRead(true);
                }
                else
                {
                    QString result="%1/download/%2-%3-%4";
                    QString filename=result.arg(m_config->getHomeDir()).arg(u->id()).arg(id).arg(newMessage->filePath().fileName());
                    QUrl newPath=QUrl::fromLocalFile(filename);
                    newMessage->setFilePath(newPath);
                }

                u->messagesModel()->save();
                addSms(UserID);
                u->userInfo()->setSms(true);

                if(newMessage->type()!=Message::Text)
                {
                    if(!proxy)
                        emit loadFile(cmdEfilechat,newMessage->filePath().toString());
                    else
                    {
                        MyProxyTcp *chanal=new MyProxyTcp(this);
                        connect(chanal,SIGNAL(Exit(MyProxyTcp*,qint32,qint32,QString)),
                                this,SLOT(exitProxy(MyProxyTcp*,qint32,qint32,QString)));
                        chanal->downloadFile(m_config,UserID,newMessage->filePath().toString());
                    }
                }
            }
        }
    }
}

void MySMSManager::exitProxy(MyProxyTcp *chanal, qint32 code, qint32 UserID, QString fileName)
{
    qDebug() << "exit tcp proxy" << code;
    if(code==0)
    {
        ApplicationModel * appModel=ApplicationModel::instance();
        User *u;
        QUrl path(fileName);

        for(qint32 i=0;i<appModel->contactsModel()->count();i++)
        {
            u=(User *)appModel->contactsModel()->get(i);
            if(u->id() == UserID)
            {
                MessagesModel *chat=(MessagesModel *)u->messagesModel();
                for(qint32 j=0;j<chat->count();j++)
                {
                    Message *msg=(Message *)chat->get(j);
                    if(msg->filePath()==path)
                    {
                        msg->setWasRead(true);
                        chat->save();
                        break;
                    }
                }

            }
        }
    }
    chanal->deleteLater();
}
