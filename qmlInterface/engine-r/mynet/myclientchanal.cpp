#include "myclientchanal.h"

void MyClientChanal::initParam()
{
    m_serverIP =m_config->getServerAddress();
    m_proxyIP  =m_config->getProxyAddress();
    m_localIP  =m_config->getLANAddress();
    m_socket.bind(QHostAddress(m_localIP.host),0);
    m_localIP.port=m_socket.localPort();

    m_serverRSA       =m_config->getServerRSA();
    m_clientRSAprivate=m_config->getClientRSAprivate();
    m_clientRSApublic =m_config->getClientRSApublic();
    m_key.clear();

    if(m_proxy)
    {
        m_wlanIP=m_config->getLANAddress();
        m_socket_proxy.bind(QHostAddress(m_wlanIP.host),0);
        m_wlanIP.port=m_socket_proxy.localPort();
    }
    m_pin=0;
    m_pout=0;
}

qint64 MyClientChanal::sendMessageToServer(MyPacket &packet)
{
    QByteArray Datagram=packet.Encode(m_serverRSA);
    return m_socket.writeDatagram(Datagram,QHostAddress(m_serverIP.host),m_serverIP.port);
}

qint64 MyClientChanal::sendMessageToProxy(MyPacket &packet, MyIPv6Address clientIP)
{
    packet.setIPv6Address(clientIP);
    QByteArray Datagram=packet.Encode(m_clientRSApublic);
    return m_socket_proxy.writeDatagram(Datagram,QHostAddress(m_proxyIP.host),m_proxyIP.port);
}

qint64 MyClientChanal::sendMessageToClient(MyPacket &packet, MyIPv6Address clientIP)
{
    m_pout++;
    packet.setIPv6Address(clientIP);
    QByteArray Datagram=packet.Encode(m_clientRSApublic);

    return m_socket.writeDatagram(Datagram,QHostAddress(clientIP.host),clientIP.port);
}

MyPacket MyClientChanal::recvMessage(QByteArray &key)
{
    MyPacket packet;
    QHostAddress addr;

    qint32 length=m_socket.pendingDatagramSize();

    QByteArray Datagram;
    Datagram.resize(length);

    m_socket.readDatagram(Datagram.data(), length, &addr, &m_workIP.port);
    m_workIP.host=addr.toIPv6Address();

    packet.Decode(Datagram,key);

    m_pin++;
    return packet;
}

MyPacket MyClientChanal::recvMessageProxy(QByteArray &key)
{
    MyPacket packet;
    QHostAddress addr;

    qint32 length=m_socket_proxy.pendingDatagramSize();

    QByteArray Datagram;
    Datagram.resize(length);

    m_socket_proxy.readDatagram(Datagram.data(), length, &addr, &m_workIP.port);
    m_workIP.host=addr.toIPv6Address();

    packet.Decode(Datagram,key);

    m_pin++;
    return packet;
}

void MyClientChanal::requestConnect()
{
    MyPacket packet;
    packet.setSignature(signature_server_RSA);
    packet.setCmd(cmdConnect);
    packet.setSessionID(m_config->getSessionID());
    packet.setUserID(m_config->getUserID());
    packet.setExitCode(0);
    packet.setStatus(m_type);

    MyConnectBlock block;
    block.clientID=m_block.clientID;

    block.userID=m_config->getUserID();
    block.sessionID=m_config->getSessionID();
    block.nRec=-1;
    block.lan_addr=m_localIP;
    block.wan_addr=m_config->getWANAddress();
    block.wlan_addr=m_wlanIP;
    block.port_mapping=m_proxy?1:0;
    packet.setConnectBlock(block);

    sendMessageToServer(packet);

    m_tick=0;
    m_attempt++;
    m_step=stepRequestConnect;

    qDebug() << "request connect" << m_userID;
//    qDebug() << "lan" << QHostAddress(block.lan_addr.host).toString()+"/"+QString::number(block.lan_addr.port);
//    qDebug() << "wan" << QHostAddress(block.wan_addr.host).toString()+"/"+QString::number(block.wan_addr.port);
//    qDebug() << "wlan" << QHostAddress(block.wlan_addr.host).toString()+"/"+QString::number(block.wlan_addr.port);
}

void MyClientChanal::requestProxy()
{
    MyPacket packet;
    packet.setSignature(signature_no_RSA);
    packet.setCmd(cmdGetProxy);
    packet.setSessionID(m_config->getSessionID());
    packet.setUserID(m_config->getUserID());
    packet.setExitCode(0);
    packet.setStatus(m_type);
    packet.setKey(m_clientRSApublic);

    sendMessageToProxy(packet,m_proxyIP);

    m_tick=0;
    m_attempt++;
    m_step=stepRequestProxy;

    qDebug() << " request proxy" << m_userID;
}

void MyClientChanal::sendConfirm()
{
    MyPacket packet;
    packet.setSignature(signature_server_RSA);
    packet.setCmd(cmdConfirm);
    packet.setSessionID(m_config->getSessionID());
    packet.setUserID(m_config->getUserID());
    packet.setExitCode(0);
    packet.setStatus(m_type);

    packet.setConnectBlock(m_block);

    sendMessageToServer(packet);

    m_tick=0;
    m_attempt++;
    m_step=stepConnectConfirm;
    m_connected=1;

    qDebug() << " confirm connect"  << m_userID;
}

void MyClientChanal::acceptConnect()
{
    MyPacket packet;
    packet.setSignature(signature_server_RSA);
    packet.setCmd(cmdConnect);
    packet.setSessionID(m_config->getSessionID());
    packet.setUserID(m_config->getUserID());
    packet.setExitCode(0);
    packet.setStatus(m_type);

    MyConnectBlock block;
    block.clientID=m_block.userID;

    block.userID=m_config->getUserID();
    block.sessionID=m_config->getSessionID();
    block.nRec=m_block.nRec;
    block.lan_addr=m_localIP;
    block.wan_addr=m_config->getWANAddress();

    block.wlan_addr=m_wlanIP;
    block.port_mapping=m_proxy?1:0;
    packet.setConnectBlock(block);

    sendMessageToServer(packet);

    m_tick=0;
    m_attempt++;
    m_step=stepConnectAccept;
    m_connected=1;

    qDebug() << " accept connect"  << m_userID;
//    qDebug() << "lan" << QHostAddress(block.lan_addr.host).toString()+"/"+QString::number(block.lan_addr.port);
//    qDebug() << "wan" << QHostAddress(block.wan_addr.host).toString()+"/"+QString::number(block.wan_addr.port);
//    qDebug() << "wlan" << QHostAddress(block.wlan_addr.host).toString()+"/"+QString::number(block.wlan_addr.port);
}

void MyClientChanal::sendMarker(qint32 ExitCode)
{
    MyPacket packet;
    packet.setSignature(signature_no_RSA);
    packet.setCmd(cmdPing);
    packet.setSessionID(m_config->getSessionID());
    packet.setUserID(m_config->getUserID());
    packet.setExitCode(ExitCode);
    packet.setStatus(m_type);

    if(m_proxy)
    {
        sendMessageToProxy(packet,m_clientIP);
    }
    else
    {
        if(m_status==1)
        {
            m_workIP=m_block.lan_addr;
            sendMessageToClient(packet,m_workIP);

            m_workIP=m_block.wan_addr;
            sendMessageToClient(packet,m_workIP);

//        m_workIP=m_block.wlan_addr;
//        sendMessageToClient(packet,m_workIP);
        }
        else
        {
            sendMessageToClient(packet,m_clientIP);
        }
    }

    m_tick=0;
    m_attempt++;
    m_step=stepMarkerConnect;
    m_idle++;
}

void MyClientChanal::readServerAnswer()
{
    if(m_packet.getCmd()==(cmdConnect|cmdMailRu))
    {
        switch(m_step)
        {
            case stepRequestConnect:
                m_step=stepConnectWait;
                m_attempt=0;
                m_tick=0;
                break;
            case stepConnectAccept:
                m_status=1;
                emit status(this,m_status);

                m_step=stepMarkerConnect;
                m_attempt=0;
                m_tick=0;
                break;
        }
    }

    if((m_packet.getCmd()==(cmdConfirm|cmdMailRu))&&(m_step==stepConnectConfirm))
    {
        m_status=1;
        emit status(this,m_status);

        m_step=stepMarkerConnect;
        m_attempt=0;
        m_tick=0;
    }

    if(m_step==stepMarkerConnect) sendMarker(0);
}

bool MyClientChanal::readConnectParam(MyPacket &Packet)
{
    bool result=false;
    if(Packet.isConnectBlock()/* && Packet.isKey()*/)
    {
        m_block =Packet.getConnectBlock();
        m_proxy=(m_block.port_mapping==1);
        if(m_proxy) m_clientIP=m_block.wlan_addr;

//        qDebug() << "connect param";
//        qDebug() << "lan" << QHostAddress(m_block.lan_addr.host).toString()+"/"+QString::number(m_block.lan_addr.port);
//        qDebug() << "wan" << QHostAddress(m_block.wan_addr.host).toString()+"/"+QString::number(m_block.wan_addr.port);
//        qDebug() << "wlan" << QHostAddress(m_block.wlan_addr.host).toString()+"/"+QString::number(m_block.wlan_addr.port);
//        qDebug() << "proxy" << m_proxy;

//        m_key   =Packet.getKey();
        m_key.clear();
        result=true;
    }

    return result;
}

void MyClientChanal::readMarker()
{
    if(m_step!=stepMarkerConnect) return;

    if(m_packet.getExitCode()==99)
    {
        qDebug() << "exit code 99";
        terminate();
    }
    else
    {
        if(m_status==1)
        {
            if(!m_proxy) m_clientIP=m_workIP;
            m_status=2;
            m_connected=2;
            emit status(this,m_status);
        }
        m_attempt=0;
    }
}

MyClientChanal::MyClientChanal(QObject *parent) :
    QObject(parent)
{
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
    connect(&m_socket_proxy,SIGNAL(readyRead()),this,SLOT(receiveMessageProxy()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(Timer100msec()));
    m_status=0;
    m_idle=0;
}

MyClientChanal::~MyClientChanal()
{
    qDebug() << "end work";
    m_step=stepTerminate;
    sendMarker(99);
    m_timer.stop();
    m_socket.close();
    disconnect(&m_socket,0,this,0);
    disconnect(&m_socket_proxy,0,this,0);
    disconnect(&m_timer,0,this,0);

    deleteLater();
}

void MyClientChanal::execute(MyConfig *Config, qint32 UserID, qint32 TypeChanal, bool useProxy)
{
    m_direction=0;
    m_config=Config;
    m_type=TypeChanal;
    m_userID=UserID;
    m_proxy=useProxy;
    m_connected=0;

    qDebug() << "active chanal. proxy=" << m_proxy;
    initParam();

    m_block.clientID=UserID;
    m_attempt=0;


    if(m_proxy)
        requestProxy();
    else
        requestConnect();

    m_timer.start(100);
}

void MyClientChanal::execute(MyConfig *Config, MyPacket &Packet)
{
    m_direction=1;
    m_config=Config;
    m_type=Packet.getStatus();
    m_connected=0;

    qDebug() << "passive chanal";
    initParam();

    if(readConnectParam(Packet))
    {
        m_userID=m_block.userID;
        m_attempt=0;

        if(m_proxy)
            requestProxy();
        else
            acceptConnect();

        m_timer.start(100);
    }
    else
        terminate();
}

void MyClientChanal::terminate()
{
    m_step=stepTerminate;
    m_status=3;
    emit status(this,m_status);
}

qint32 MyClientChanal::getStatus()
{
    return m_status;
}

qint32 MyClientChanal::getUserID()
{
    return m_userID;
}

qint32 MyClientChanal::getSessionID()
{
    return m_block.sessionID;
}

qint32 MyClientChanal::getTypeChanal()
{
    return m_type;
}

qint32 MyClientChanal::getIdle()
{
    return m_idle;
}

bool MyClientChanal::isProxy()
{
    return m_proxy;
}

bool MyClientChanal::isConnected()
{
    return (m_connected==1);
}

qint32 MyClientChanal::getDirection()
{
    return m_direction;
}

void MyClientChanal::sendPacket(MyPacket &packet)
{
    if(m_status==2)
    {
        packet.setSignature(signature_no_RSA);
        packet.setSessionID(m_config->getSessionID());
        packet.setUserID(m_config->getUserID());
        packet.setExitCode(0);
        packet.setStatus(m_type);
        packet.setIPv6Address(m_clientIP);

        if(m_proxy)
            sendMessageToProxy(packet,m_clientIP);
        else
            sendMessageToClient(packet,m_clientIP);

        m_idle=0;
    }
}

void MyClientChanal::Timer100msec()
{
    m_tick++;

    if(m_tick<30) return;

    if((m_attempt>5)||(m_idle>100))
    {
//        qDebug() << "timeout" << m_attempt << m_idle;
        terminate();
    }
    else
    {
        switch(m_step)
        {
            case stepRequestConnect: requestConnect(); break;
            case stepRequestProxy:   requestProxy(); break;
            case stepConnectConfirm: sendConfirm(); break;
            case stepConnectAccept:  acceptConnect(); break;
            case stepMarkerConnect:  sendMarker(0); break;
            default:
                m_attempt++;
                m_tick=0;
        }
    }
}

void MyClientChanal::receiveMessage()
{
    qint32 cmd;
    while((m_socket.state()==QUdpSocket::BoundState) && m_socket.hasPendingDatagrams())
    {
        m_packet=recvMessage(m_clientRSAprivate);

        cmd=m_packet.getCmd();
//        qDebug() << "answer cmd=" << QString::number(cmd,16);

        if(cmd&cmdMailRu)
        {
            readServerAnswer();
        }
        else
            if(cmd==cmdConnect)
            {
                if(readConnectParam(m_packet))
                {
                    m_attempt=0;
                    sendConfirm();
                }
            }
            else
                if(cmd==cmdPing)
                {
                    readMarker();
                }
                else
                {
                    m_idle=0;
                    m_attempt=0;
                    emit receivePacket(this,m_packet);
                }

        if(m_step==stepTerminate) break;
    }
}

void MyClientChanal::receiveMessageProxy()
{
    qint32 cmd;
    while((m_socket_proxy.state()==QUdpSocket::BoundState) && m_socket_proxy.hasPendingDatagrams())
    {
        m_packet=recvMessageProxy(m_clientRSApublic);

        cmd=m_packet.getCmd();
//        qDebug() << "answer proxy cmd=" << QString::number(cmd,16);

        if(cmd==(cmdGetProxy|cmdAnswer))
        {
            m_wlanIP=m_packet.getIPv6Address();
            m_proxyIP.port=m_packet.getStatus();
            qDebug() << "proxy port" << m_proxyIP.port;
            if(m_direction==0)
                requestConnect();
            else
                acceptConnect();
        }
        else
            if(cmd==cmdPing)
            {
                readMarker();
            }
                else
            {
                m_idle=0;
                emit receivePacket(this,m_packet);
            }

        if(m_step==stepTerminate) break;
    }
}
