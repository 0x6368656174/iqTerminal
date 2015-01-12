#include "myconfig.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QByteArray>
#include <QBuffer>
#include <QHostInfo>
#include <QHostAddress>
#include "../MyGlobal.h"

#include "../crypto/myRSAext.h"
#include "../common/MyServerKey.h"

#define c_clientIni "/home/client.ini"
#define c_clientRSAprivate "/home/clientRSA.pri"
#define c_clientRSApublic "/home/clientRSA.pub"

MyIPv6Address MyConfig::IPv6fromString(QString str, quint16 defaultport)
{
    QString s_addr, s_port;
    qint32 pos=str.indexOf("/");
    if(pos==-1)
    {
        s_addr=str;
        s_port=QString::number(defaultport);
    }
    else
    {
        s_addr=str.left(pos);
        s_port=str.right(str.size()-pos-1);
    }

    MyIPv6Address m_addr;
    m_addr.host=QHostAddress(s_addr).toIPv6Address();
    m_addr.port=s_port.toInt();
    return m_addr;
}

void MyConfig::defaultConfig()
{
    // set start parametrs
    m_DownloadDir=m_HomeDir+"/download";
    m_ServerName="FirstTerminal";
    m_ServerAddr=IPv6fromString("91.189.165.76",3490);
    m_ProxyAddr =IPv6fromString("193.43.234.87",3499);
//    m_ProxyAddr =IPv6fromString("192.168.1.3",3499);
    m_StunAddr  =IPv6fromString("91.189.165.72",3478);

    memset((void *)m_LanAddr.host.c,0,16);
    m_LanAddr.port=0;
    m_flagLAN=false;

    memset((void *)m_WanAddr.host.c,0,16);
    m_WanAddr.port=0;

    memset((void *)m_WLanAddr.host.c,0,16);
    m_WLanAddr.port=0;
    m_onLogon=false;
    m_onConnect=false;
    m_login="";
    m_password="";
    m_userID=0;
    m_sessionID=0;

    m_typeNAT=0;
    m_typePort=0;
    m_NATdetect=false;
}

void MyConfig::parser(QString name, QString value)
{
    if(name==QString("DOWNLOADDIR"))
    {
        m_DownloadDir=value;
        emit varChanged(varEdownloaddir);
    }
    if(name==QString("SERVERNAME"))
    {
        m_ServerName=value;
        emit varChanged(varEservername);
    }
    if(name==QString("SERVERADDR"))
    {
        m_ServerAddr=IPv6fromString(value,3490);
        emit varChanged(varEserverip);
    }
    if(name==QString("PROXYADDR"))
    {
        m_ProxyAddr=IPv6fromString(value,3499);
    }
    if(name==QString("STUNADDR"))
    {
        m_StunAddr=IPv6fromString(value,3478);
        emit varChanged(varEstunip);
    }
    if(name==QString("LANADDR"))
    {
        m_flagLAN=true;
        m_LanAddr=IPv6fromString(value,0);
        emit varChanged(varElanip);
    }
}

void MyConfig::loadConfig()
{
    QByteArray line;
    QString str, param, value;

    // Load server RSA-key
    m_serverRSApublickey=cServerKey.toUtf8();

    // Загрузка клиентских ключей
    QFile fileRSA;
    bool flag_load_key=true;

    m_clientRSAprivatekey.clear();
    fileRSA.setFileName(m_HomeDir+QString(c_clientRSAprivate));
    if(fileRSA.open(QIODevice::ReadOnly))
    {
        m_clientRSAprivatekey=fileRSA.read(fileRSA.size());
        fileRSA.close();
    }
    else
    {
        flag_load_key=false;
    }

    m_clientRSApublickey.clear();
    fileRSA.setFileName(m_HomeDir+QString(c_clientRSApublic));
    if(fileRSA.open(QIODevice::ReadOnly))
    {
        m_clientRSApublickey=fileRSA.read(fileRSA.size());
        fileRSA.close();
    }
    else
    {
        flag_load_key=false;
    }

    // создать клиентские ключи
    if(!flag_load_key)
    {
        qDebug() << "create key";
        MyRSAext rsaext;
        QString tmp_key1, tmp_key2;
        rsaext.Create(256, tmp_key1, tmp_key2);

        // повторная загрузка ключей
        m_clientRSApublickey=tmp_key1.toUtf8();
        m_clientRSAprivatekey=tmp_key2.toUtf8();

        fileRSA.setFileName(m_HomeDir+QString(c_clientRSAprivate));
        if(fileRSA.open(QIODevice::WriteOnly))
        {
            fileRSA.write(m_clientRSAprivatekey);
            fileRSA.close();
        }

        fileRSA.setFileName(m_HomeDir+QString(c_clientRSApublic));
        if(fileRSA.open(QIODevice::WriteOnly))
        {
            fileRSA.write(m_clientRSApublickey);
            fileRSA.close();
        }
    }
}

void MyConfig::UpdateDynam()
{
    QString param=QString::number(m_sessionID)+","+
            getWANAddressAsString()+","+
            getWLANAddressAsString()+","+
            getLANAddressAsString()+","+
            QString::number(m_typeNAT)+",0,0,";

    emit commandExecute(cmdEdynam,param);
}

QString MyConfig::NatTypeString(qint32 type)
{
    switch(type)
    {
        case 0: return "Неопределен";
        case 1: return "UDP blocked";
        case 2: return "Open Internet";
        case 3: return "Full Cone";
        case 4: return "Symmetric";
        case 5: return "Port Restrict";
        case 6: return "Address Restrict";
        case 7: return "Strong Port Restrict";
    }
    return "???";
}

MyConfig::MyConfig(QObject *parent) :
    QObject(parent)
{
    defaultConfig();
}

void MyConfig::execute()
{
    loadConfig();
}

QString MyConfig::getHomeDir()
{
    return m_HomeDir;
}

QString MyConfig::getDownloadDir()
{
    return m_DownloadDir;
}

QString MyConfig::getServerName()
{
    return m_ServerName;
}

QString MyConfig::getServerAddressAsString()
{
    return QHostAddress(m_ServerAddr.host).toString()+"/"+QString::number(m_ServerAddr.port);
}

QString MyConfig::getSTUNAddressAsString()
{
    return QHostAddress(m_StunAddr.host).toString()+"/"+QString::number(m_StunAddr.port);
}

QString MyConfig::getLANAddressAsString()
{
    return QHostAddress(m_LanAddr.host).toString()+"/"+QString::number(m_LanAddr.port);
}

QString MyConfig::getWLANAddressAsString()
{
    return QHostAddress(m_WLanAddr.host).toString()+"/"+QString::number(m_WLanAddr.port);
}

QString MyConfig::getWANAddressAsString()
{
    return QHostAddress(m_WanAddr.host).toString()+"/"+QString::number(m_WanAddr.port);
}

MyIPv6Address MyConfig::getServerAddress()
{
    return m_ServerAddr;
}

MyIPv6Address MyConfig::getProxyAddress()
{
    return m_ProxyAddr;
}

MyIPv6Address MyConfig::getSTUNAddress()
{
    return m_StunAddr;
}

MyIPv6Address MyConfig::getLANAddress()
{
    return m_LanAddr;
}

MyIPv6Address MyConfig::getWLANAddress()
{
    return m_WLanAddr;
}

MyIPv6Address MyConfig::getWANAddress()
{
    return m_WanAddr;
}

bool MyConfig::getFlagLAN()
{
    return m_flagLAN;
}

QString MyConfig::getLogin()
{
    return m_login;
}

QString MyConfig::getPassword()
{
    return m_password;
}

qint32 MyConfig::getUserID()
{
    return m_userID;
}

qint32 MyConfig::getSessionID()
{
    return m_sessionID;
}

bool MyConfig::getConnectStatus()
{
    return m_onConnect;
}

bool MyConfig::getLogonStatus()
{
    return m_onLogon;
}

QByteArray MyConfig::getServerRSA()
{
    return m_serverRSApublickey;
}

QByteArray MyConfig::getClientRSApublic()
{
    return m_clientRSApublickey;
}

QByteArray MyConfig::getClientRSAprivate()
{
    return m_clientRSAprivatekey;
}

qint32 MyConfig::getNatType()
{
    return m_typeNAT;
}

QString MyConfig::getNatTypeAsString()
{
    return NatTypeString(m_typeNAT);
}

qint32 MyConfig::getNatPortType()
{
    return m_typePort;
}

QString MyConfig::getNatPortTypeAsString()
{
    return QString::number(m_typePort);
}

void MyConfig::setIPv6Address(MyIPv6Address WAN, MyIPv6Address LAN)
{
    m_WanAddr=WAN;
    m_LanAddr=LAN;
    emit varChanged(varEwanip);
    emit varChanged(varElanip);
    emit varChanged(varEconnect);

    emit LANdetected();
}

void MyConfig::setLogin(QString Login, QString Password, qint32 sessionID)
{
    m_login=Login;
    emit varChanged(varElogin);
    m_password=Password;
    emit varChanged(varEpassword);
    m_sessionID=sessionID;
    emit varChanged(varEsessionid);

    m_onLogon=(sessionID!=0);
    if(!m_onLogon) m_userID=0;
    emit varChanged(varElogon);

    if(m_onLogon && m_userID  && m_NATdetect) UpdateDynam();
}

void MyConfig::setUserID(qint32 userID)
{
    m_userID=userID;
    emit varChanged(varEuserid);
    emit commandExecute(cmdEsetprofile,"");
    emit commandExecute(cmdEsettorrent,"");

    if(m_onLogon && m_userID  && m_NATdetect) UpdateDynam();
}

void MyConfig::Connection(bool status)
{
    if(status!=m_onConnect)
    {
        m_onConnect=status;
        emit varChanged(varEconnect);
        if(!m_onConnect && m_onLogon)
        {
            m_onLogon=false;
            m_userID=0;
            emit varChanged(varElogon);
            emit varChanged(varEuserid);
        }
    }
}

void MyConfig::NATdetected(qint32 typeNAT, qint32 typePort, qint32 exitCode)
{
    Q_UNUSED(exitCode)

    m_typeNAT=typeNAT;
    m_typePort=typePort;
    m_NATdetect=true ; //(exitCode==0);

    emit varChanged(varEtypenat);

    if(m_onLogon && m_userID && m_NATdetect) UpdateDynam();
}

void MyConfig::setConfigParam(QString param, QVariant value)
{
    if(param==varEhomedir)
    {
        m_HomeDir=value.toString();
        emit varChanged(param);
        return;
    }

    if(param==varEdownloaddir)
    {
        m_DownloadDir=value.toString();
        emit varChanged(param);
        return;
    }

    if(param==varEserverip)
    {
        m_ServerAddr=IPv6fromString(value.toString(),3490);
        emit varChanged(param);
        return;
    }

    if(param==varEstunip)
    {
        m_StunAddr=IPv6fromString(value.toString(),3478);
        emit varChanged(param);
        return;
    }
}
