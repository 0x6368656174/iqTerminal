#include "myproxyengine.h"
#include <QFile>

#include <QDebug>
#include "applicationmodel.h"
#include "common/myxmldoc.h"

QString MyProxyEngine::extractParam(QString param, qint32 n)
{
    qint32 ifrom=0;
    qint32 ito=0;
    n--;
    while(n>0)
    {
        ifrom=param.indexOf(",",ifrom);
        if(ifrom==-1) return "";
        ifrom++;
        n--;
    }
    ito=param.indexOf(",",ifrom);
    if(ito<0) ito=param.size()+1;

    return param.mid(ifrom,ito-ifrom);
}

void MyProxyEngine::createDialog(qint16 cmd, QString param)
{
    MyDialog *m_dialog=new MyDialog(this);
    connect(m_server, SIGNAL(receiveMessage(MyPacket &)),
            m_dialog, SLOT  (receiveMessage(MyPacket &)));
    connect(m_dialog, SIGNAL(Exit(MyDialog *,qint16,qint16)),
            this,     SLOT  (DialogAnswer(MyDialog *,qint16,qint16)));
    connect(m_dialog, SIGNAL(sendMessage(MyPacket &)),
            m_server, SLOT  (sendMessage(MyPacket &)));
    connect(m_dialog, SIGNAL(Logon(QString, QString, qint32)),
            m_config, SLOT  (setLogin(QString, QString, qint32)));
    connect(m_dialog, SIGNAL(SessionID(qint32)),
            m_server, SLOT  (setSessionID(qint32)));

    m_dialog->execute(cmd,
                      m_config->getSessionID(),
                      param,
                      m_config->getClientRSApublic());
}

void MyProxyEngine::signalParsers(qint16 cmd, qint16 exitCode)
{
    qDebug() << "answer command: " << cmd << "=" << exitCode;

    switch(cmd)
    {
        case cmdLogon:
            switch(exitCode)
            {
                case  0: emit logonOk(""); loadAbonents(cmdGetAbonBook,""); break;
                case  1:
                case  6: emit logonError(tr("Invalid login or password")); break;
                case 99: emit logonError(tr("The server is not respondin")); break;
                default: emit logonError(tr("Authorization error =")+QString::number(exitCode));
            }
            break;

        case cmdLogoff:
            emit logoffExit("OK");
            break;

        case cmdRegistr:
            switch(exitCode)
            {
                case  0:
                case  4: emit registrOk(tr("Successful registration")); break;
                case  1: emit registrError(tr("Invalid email")); break;
                case  2: emit registrError(tr("Login busy")); break;
                case  3: emit registrError(tr("email busy")); break;
                case 99: emit registrError(tr("The server is not respondin")); break;
                default: emit registrError(tr("Registration error =")+QString::number(exitCode));
            }
            break;

        case cmdRestorePswd:
            switch(exitCode)
            {
                case  0:
                case  5: emit restorePswdOk(tr("The password sent to email")); break;
                case  1:
                case  3: emit restorePswdError(tr("Invalid email")); break;
                case 99: emit restorePswdError(tr("The server is not respondin")); break;
                default: emit restorePswdError(tr("Error recovery password =")+QString::number(exitCode));
            }
            break;

        case cmdChangePswd:
            switch(exitCode)
            {
                case  0:
                case  8: emit changePswdOk(tr("The password is changed")); break;
                case  1: emit changePswdError(tr("Weak password")); break;
                case  6:
                case  9: emit changePswdError(tr("Invalid login or password")); break;
                case 99: emit changePswdError(tr("The server is not respondin")); break;
                default: emit changePswdError(tr("Error changing password =")+QString::number(exitCode));
            }
            break;

        case cmdUpdateDynam:
            break;
        case cmdAddAbonBook:
            break;
        case cmdDelAbonBook:
            break;
    }
}

void MyProxyEngine::loadAbonents(quint16 cmd, QString param)
{
    QByteArray ba;
    MyPrivatToServer *chanal;

    chanal=new MyPrivatToServer(this);
    switch(cmd)
    {
        case cmdGetAbonBook:
            connect(chanal,SIGNAL(Exit(MyPrivatToServer*,qint32,QByteArray&)),
                    this,  SLOT(AbonBookIsLoad(MyPrivatToServer*,qint32,QByteArray&)));
            break;
        case cmdGetAbonList:
            connect(chanal,SIGNAL(Exit(MyPrivatToServer*,qint32,QByteArray&)),
                    this,  SLOT(AbonListIsLoad(MyPrivatToServer*,qint32,QByteArray&)));
            break;
        case cmdGetAbonWait:
            connect(chanal,SIGNAL(Exit(MyPrivatToServer*,qint32,QByteArray&)),
                    this,  SLOT(AbonWaitIsLoad(MyPrivatToServer*,qint32,QByteArray&)));
            break;
    }

    chanal->execute(m_config,cmd,param,ba);
}

void MyProxyEngine::setMyProfile()
{
    QByteArray ba;
    QFile file;
    file.setFileName(m_config->getHomeDir()+QString("i.xml"));
    if(file.open(QIODevice::ReadOnly))
    {
        ba=file.readAll();
        file.close();
        if(!isBOM(ba))
        {
            ba.prepend(BOM,3);
        }

        MyPrivatToServer *chanal;

        chanal=new MyPrivatToServer(this);
        connect(chanal,SIGNAL(Exit(MyPrivatToServer*,qint32,QByteArray&)),
                this,  SLOT(MyProfileUpload(MyPrivatToServer*,qint32,QByteArray&)));

        chanal->execute(m_config,cmdSetProfile,QString::number(m_config->getUserID()),ba);
    }
}

void MyProxyEngine::getAnyProfile(QString profile)
{
    QByteArray ba;
    MyPrivatToServer *chanal;

    chanal=new MyPrivatToServer(this);
    connect(chanal,SIGNAL(Exit(MyPrivatToServer*,qint32,QByteArray&)),
            this,  SLOT(AnyProfileDownload(MyPrivatToServer*,qint32,QByteArray&)));

    chanal->execute(m_config,cmdGetProfile,profile,ba);
}

void MyProxyEngine::setMyTorrent()
{
    QByteArray ba;
    QFile file;
    file.setFileName(m_config->getHomeDir()+QString("torrent.xml"));
    if(file.open(QIODevice::ReadOnly))
    {
        ba=file.readAll();
        file.close();
        if(!isBOM(ba))
        {
            ba.prepend(BOM,3);
        }

        MyPrivatToServer *chanal;

        chanal=new MyPrivatToServer(this);
        connect(chanal,SIGNAL(Exit(MyPrivatToServer*,qint32,QByteArray&)),
                this,  SLOT(MyTorrentUpload(MyPrivatToServer*,qint32,QByteArray&)));

        chanal->execute(m_config,cmdSetTorrent,QString::number(m_config->getUserID()),ba);
    }
}

void MyProxyEngine::searchTorrent(QString param)
{
    QByteArray ba;
    MyPrivatToServer *chanal;

    chanal=new MyPrivatToServer(this);
    connect(chanal,SIGNAL(Exit(MyPrivatToServer*,qint32,QByteArray&)),
            this,  SLOT(TorrentSearchLoad(MyPrivatToServer*,qint32,QByteArray&)));
    chanal->execute(m_config,cmdGetTorrent,param,ba);
}

void MyProxyEngine::ChangePage(QString name)
{
    if(m_lastpage=="i") command(cmdEsetprofile,"");
    if(m_lastpage=="torrent") command(cmdEsettorrent,"");
    if(m_lastpage=="chat")
    {
        m_sms->clearSms(m_chat);
        if(name=="contacts")
        {
            m_sms->checkSms();
            m_sms->checkOnline();
            m_chat=-1;
        }
    }
    m_lastpage=name;

    if(name=="chat")
    {
        m_sms->clearSms(name.toInt());

        if(m_chat!=-1) m_sms->Connect(m_chat);
    }
}

void MyProxyEngine::loadProfiles(QByteArray Book)
{
    MyXmlDoc xml;
    xml.load(Book);
    qint32 count=xml.getCountChild("/all","user");
    QString userid;

    for(qint32 i=0;i<count;i++)
    {
        userid=xml.getTagValue("/all/user["+QString::number(i)+"]/id");
        getAnyProfile(userid);
    }
}

bool MyProxyEngine::isBOM(QByteArray &Book)
{
    if((Book.at(0)!=char(0xef)) || (Book.at(1)!=char(0xbb)) || (Book.at(2)!=char(0xbf))) return false;
    return true;
}

void MyProxyEngine::openChat(QString index)
{
    qDebug() << "chat" << index;
    if(index!="") m_chat=index.toInt();
    if(m_chat!=-1) m_sms->Connect(m_chat);
}

void MyProxyEngine::openAudio(QString UserID)
{
    qDebug() << "audio" << UserID;
    if(UserID!="") m_audio->Connect(UserID.toInt());
}

void MyProxyEngine::openVideo(QString UserID)
{
    qDebug() << "video" << UserID;
    if(UserID!="") m_video->Connect(UserID.toInt());
}

void MyProxyEngine::closeVideo(QString UserID)
{
    qDebug() << "end video" << UserID;
    if(UserID!="") m_video->Disconnect(UserID.toInt());
}

void MyProxyEngine::loadFile(QString filename)
{
    m_file->loadFile(filename);
}

MyProxyEngine::MyProxyEngine(QString homeDir, QObject *parent) :
    QObject(parent)
{
    m_config=new MyConfig(this);
    m_server=new MyServersChanal(this);
    m_nat   =new MyNATDetector(this);
    m_first =new MyFirstManager(this);
    m_sms   =new MySMSManager(this);
    m_audio =new MyAudioManager(this);
    m_file  =new MyFileManager(this);
    m_video =new MyVideoManager(this);

    connect(m_config,SIGNAL(commandExecute(QString,QString)),
            this,    SLOT  (command(QString, QString)));
    connect(m_config,SIGNAL(LANdetected()),
            this,    SLOT  (StartNATdetect()));

    connect(m_server,SIGNAL(ConnectServer(bool)),
            m_config,SLOT  (Connection(bool)));
    connect(m_server,SIGNAL(MappingAddres(MyIPv6Address, MyIPv6Address)),
            m_config,SLOT  (setIPv6Address(MyIPv6Address, MyIPv6Address)));
    connect(m_server,SIGNAL(UserID(qint32)),
            m_config,SLOT  (setUserID(qint32)));
    connect(m_server,SIGNAL(receiveMessage(MyPacket&)),
            m_first, SLOT  (recvServer(MyPacket&)));

    connect(m_nat,   SIGNAL(NATdetected(qint32,qint32,qint32)),
            m_config,SLOT  (NATdetected(qint32,qint32,qint32)));
    connect(m_nat,   SIGNAL(NATdetected(qint32,qint32,qint32)),
            this,    SLOT  (NATDetected(qint32,qint32,qint32)));

    connect(m_first, SIGNAL(newConnect(MyClientChanal*)),
            m_sms,   SLOT  (addConnect(MyClientChanal*)));
    connect(m_first, SIGNAL(newConnect(MyClientChanal*)),
            m_audio, SLOT  (addConnect(MyClientChanal*)));
    connect(m_first, SIGNAL(newConnect(MyClientChanal*)),
            m_file,  SLOT  (addConnect(MyClientChanal*)));
    connect(m_first, SIGNAL(newConnect(MyClientChanal*)),
            m_video, SLOT  (addConnect(MyClientChanal*)));

    connect(m_sms,   SIGNAL(loadFile(QString,QString)),
            this,    SLOT  (command(QString, QString)));

    connect(m_video, SIGNAL(updateVideoIn(QImage)),
            this,    SLOT(updateCurCameraOutSlot(QImage)));

    setParam(varEhomedir,QVariant(homeDir));

    m_abonbook=0;
    m_abonwait=0;
    m_abonlist=0;
    m_chat=-1;

    m_config->execute();
    m_server->execute(m_config);
    m_first->init(m_config);
    m_sms->init(m_config);
    m_audio->init(m_config);
    m_file->init(m_config);
    m_video->init(m_config);
}

void MyProxyEngine::command(QString cmd, QString param)
{
    qDebug() << "command: " << cmd << "," << param;
    if(cmd==cmdElogon)   { createDialog(cmdLogon,param); return; }
    if(cmd==cmdElogoff)  { createDialog(cmdLogoff,param); return; }
    if(cmd==cmdEregistr) { createDialog(cmdRegistr,param); return; }
    if(cmd==cmdErestore) { createDialog(cmdRestorePswd,param); return; }
    if(cmd==cmdEchange)  { createDialog(cmdChangePswd,m_config->getLogin()+","+param); return; }
    if(cmd==cmdEdynam)   { createDialog(cmdUpdateDynam,param); return; }

    if(cmd==cmdEabonbook) { loadAbonents(cmdGetAbonBook,param); return; }
    if(cmd==cmdEabonlist) { loadAbonents(cmdGetAbonList,param); return; }
    if(cmd==cmdEabonwait) { loadAbonents(cmdGetAbonWait,param); return; }

    if(cmd==cmdEabonadd)  { createDialog(cmdAddAbonBook,param); return; }
    if(cmd==cmdEabondel)  { createDialog(cmdDelAbonBook,param); return; }

    if(cmd==cmdEsetprofile) { setMyProfile(); return; }
    if(cmd==cmdEgetprofile) { getAnyProfile(param); return; }
    if(cmd==cmdEsettorrent) { setMyTorrent(); return; }
    if(cmd==cmdEgettorrent) { searchTorrent(param); return; }

    if(cmd==cmdEchat)  { openChat(param); return; }
    if(cmd==cmdEaudio) { openAudio(param); return; }
    if(cmd==cmdEvideo) { openVideo(param); return; }
    if(cmd==cmdEvideo_) { closeVideo(param); return; }

    if(cmd==cmdEfilechat) { loadFile(param); return; }

    if(cmd==cmdEshowpage) { ChangePage(param); return; }
}

void MyProxyEngine::sendMessage(Message *value)
{
    if(m_chat!=-1) m_sms->sendMessage(m_chat,value);
}

void MyProxyEngine::DialogAnswer(MyDialog *dialog, qint16 cmd, qint16 exitCode)
{
    signalParsers(cmd,exitCode);
    delete dialog;
}

void MyProxyEngine::StartNATdetect()
{
    m_nat->execute(m_config->getSTUNAddress(),m_config->getLANAddress());
}

void MyProxyEngine::NATDetected(qint32 typeNAT, qint32 typePort, qint32 errorCode)
{
    Q_UNUSED(typeNAT)
    Q_UNUSED(typePort)
    Q_UNUSED(errorCode)

    delete m_nat;
}

void MyProxyEngine::setParam(QString param, QVariant value)
{
    m_config->setConfigParam(param,value);
}

void MyProxyEngine::updateCurCameraInSlot(QImage kadr)
{
    m_video->updateVideoOut(kadr);
//    emit updateCurCameraOutSignal(kadr);
}

void MyProxyEngine::updateCurCameraOutSlot(QImage kadr)
{
    emit updateCurCameraOutSignal(kadr);
}

void MyProxyEngine::updateParam(QString name, QVariant value)
{
    if(name==varEconnect)   { emit updateConnectStatus(value.toString()); return; }
    if(name==varElogon)     { emit updateLogonStatus(value.toString()); return; }
    if(name==varElanip)     { emit updateLAN(value.toString()); return; }
    if(name==varEwanip)     { emit updateWAN(value.toString()); return; }
    if(name==varEuserid)    { emit updateUserID(value.toString()); return; }
    if(name==varEsessionid) { emit updateSessionID(value.toString()); return; }
    if(name==varElogin)     { emit updateLogin(value.toString()); return; }

    if(name==varEhomedir)     { emit updateHomeDir(value.toString()); return; }
    if(name==varEdownloaddir) { emit updateDownloadDir(value.toString()); return; }
    if(name==varEserverip)    { emit updateServerIP(value.toString()); return; }
    if(name==varEstunip)      { emit updateStunIP(value.toString()); return; }
}

void MyProxyEngine::AbonBookIsLoad(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book)
{
    qDebug() << "AbonBook=" << exitCode;

    if(exitCode==0)
    {
        QFile file;
        file.setFileName(m_config->getHomeDir()+QString("/contacts.xml"));
        if(file.open(QIODevice::WriteOnly))
        {
            if(!isBOM(Book))
            {
                file.write(BOM,3);
            }
            file.write(Book);
            file.close();
        }

        m_abonbook=0;
        ApplicationModel * appModel=ApplicationModel::instance();
        appModel->contactsModel()->reload();
        m_sms->checkSms();
        m_sms->checkOnline();

//        emit abonBookLoaded();
        m_sms->execute();
        loadProfiles(Book);
    }
    else
    {
        if(++m_abonbook>2)
        {
            m_abonbook=0;
            emit abonBookNotLoaded();
        }
        else
            command(cmdEabonbook,"");
    }

    delete chanal;
}

void MyProxyEngine::AbonListIsLoad(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book)
{
    qDebug() << "AbonList=" << exitCode;
    if(exitCode==0)
    {
        QFile file;
        file.setFileName(m_config->getHomeDir()+QString("all.xml"));
        if(file.open(QIODevice::WriteOnly))
        {
            if(!isBOM(Book))
            {
                file.write(BOM,3);
            }
            file.write(Book);
            file.close();
        }

        m_abonlist=0;
        ApplicationModel * appModel=ApplicationModel::instance();
        appModel->allModel()->reload();
        emit abonListLoaded();
        loadProfiles(Book);
    }
    else
    {
        if(++m_abonlist>2)
        {
            m_abonlist=0;
            emit abonListNotLoaded();
        }
        else
            command(cmdEabonlist,"%%");
    }

    delete chanal;
}

void MyProxyEngine::AbonWaitIsLoad(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book)
{
    qDebug() << "AbonWait=" << exitCode;
    if(exitCode==0)
    {
        QFile file;
        file.setFileName(m_config->getHomeDir()+QString("visitors.xml"));
        if(file.open(QIODevice::WriteOnly))
        {
            if(!isBOM(Book))
            {
                file.write(BOM,3);
            }
            file.write(Book);
            file.close();
        }

        m_abonwait=0;
        ApplicationModel * appModel=ApplicationModel::instance();
        appModel->visitorsModel()->reload();
        emit abonWaitLoaded();
        loadProfiles(Book);
    }
    else
    {
        if(++m_abonwait>2)
        {
            m_abonwait=0;
            emit abonWaitNotLoaded();
        }
        else
            command(cmdEabonwait,"");
    }

    delete chanal;
}

void MyProxyEngine::MyProfileUpload(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book)
{
    Q_UNUSED(Book)

    qDebug() << "MyProfile=" << exitCode;
    if(exitCode==0)
    {
        emit myProfileUpLoaded();
    }
    else
    {
        emit myProfileNotUpLoaded();
    }

    delete chanal;
}

void MyProxyEngine::AnyProfileDownload(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book)
{
    qDebug() << "AnyProfile " << chanal->getParam() << "=" << exitCode;
    if(exitCode==0)
    {
        QString profile=chanal->getParam()+".xml";
        QFile file;
        file.setFileName(m_config->getHomeDir()+QString("users/"+profile));
        if(file.open(QIODevice::WriteOnly))
        {
            if(!isBOM(Book))
            {
                file.write(BOM,3);
            }
            file.write(Book);
            file.close();
        }

        ApplicationModel * appModel=ApplicationModel::instance();
        User * item;
        for(int i=0;i<appModel->contactsModel()->count();i++)
        {
            item = qobject_cast<User *>(appModel->contactsModel()->get(i));
            if(item->profile()==profile)
            {
                item->resetProfile();
            }
        }

        for(int i=0;i<appModel->visitorsModel()->count();i++)
        {
            item = qobject_cast<User *>(appModel->visitorsModel()->get(i));
            if(item->profile()==profile)
            {
                item->resetProfile();
            }
        }

        for(int i=0;i<appModel->allModel()->count();i++)
        {
            item = qobject_cast<User *>(appModel->allModel()->get(i));
            if(item->profile()==profile)
            {
                item->resetProfile();
            }
        }

        emit anyProfileDownLoaded();
    }
    else
    {
        emit anyProfileNotDownLoaded();
        getAnyProfile(chanal->getParam());
    }

    delete chanal;
}

void MyProxyEngine::MyTorrentUpload(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book)
{
    Q_UNUSED(Book)

    qDebug() << "My Torrent=" << exitCode;
    if(exitCode==0)
    {
        emit myTorrentUpLoaded();
    }
    else
    {
        emit myTorrentNotUpLoaded();
    }

    delete chanal;
}

void MyProxyEngine::TorrentSearchLoad(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book)
{
    qDebug() << "search is load";
    if(exitCode==0)
    {
        QFile file;
        file.setFileName(m_config->getHomeDir()+QString("search.xml"));
        if(file.open(QIODevice::WriteOnly))
        {
            if(!isBOM(Book))
            {
                file.write(BOM,3);
            }
            file.write(Book);
            file.close();
        }

        ApplicationModel * appModel=ApplicationModel::instance();
        appModel->torrents()->searchModel()->reload();
        emit torrentSearchLoaded();
        loadProfiles(Book);
    }
    else
    {
        emit torrentSearchNotLoaded();
    }

    delete chanal;
}
