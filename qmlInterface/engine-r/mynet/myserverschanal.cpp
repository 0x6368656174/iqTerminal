#include "myserverschanal.h"
#include <QBuffer>
#include <QNetworkInterface>

qint64 MyServersChanal::sendMessageToServer(MyPacket &packet)
{
    QByteArray Datagram=packet.Encode(m_serverRSA);
    return m_socket.writeDatagram(Datagram,QHostAddress(m_serverIP.host),m_serverIP.port);
}

void MyServersChanal::parseMessage()
{
    qint32 length=m_socket.pendingDatagramSize();

    QByteArray Datagram;
    Datagram.resize(length);

    m_socket.readDatagram(Datagram.data(), length);

    // analiz packet
    MyPacket packet;
    if(!packet.Decode(Datagram,m_clientRSAprivate)) return;

    quint16 cmd=packet.getCmd();

    if((cmd&0xfff)==cmdMappingPort) // cmd mapping
    {
        if(packet.isIPv6Address())
        {
            m_needMapping=false;
            m_publicIP=packet.getIPv6Address();
            emit MappingAddres(m_publicIP, m_localIP);
            emit ConnectServer(true);
//            qDebug() << "ping " << QHostAddress(m_publicIP.host).toString()+"/"+QString::number(m_publicIP.port);

        }
    }
    else
    {
        if((cmd&0xfff)==cmdPing) // cmd ping
        {
            m_countPing=0;
            if(m_userID==0)
            {
                m_userID=packet.getUserID();
                emit UserID(m_userID);
                m_timer.setInterval(30000);
            }
        }
        else
        {
            emit receiveMessage(packet);
        }
    }
}

MyServersChanal::MyServersChanal(QObject *parent) :
    QObject(parent)
{
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(receiveMessageFromServer()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(Timer30Second()));
}

void MyServersChanal::execute(MyConfig *Config)
{
    m_serverIP =Config->getServerAddress();
    m_localIP  =Config->getLANAddress();

    m_serverRSA       =Config->getServerRSA();
    m_clientRSAprivate=Config->getClientRSAprivate();
    m_clientRSApublic =Config->getClientRSApublic();

    // проверка локального адреса
    m_flag=Config->getFlagLAN();
    if(!m_flag)
    {
        QList<QNetworkInterface> ifaces=QNetworkInterface::allInterfaces();
        m_list.clear();
        if(!ifaces.isEmpty())
        {
            for(qint32 i=0;i<ifaces.count();i++)
            {
                qint32 flags    = ifaces[i].flags();
//                bool isLoopback = (bool)(flags & QNetworkInterface::IsLoopBack);
//                bool isP2P      = (bool)(flags & QNetworkInterface::IsPointToPoint);
                bool isRunning  = (bool)(flags & QNetworkInterface::IsRunning);

                if(!isRunning || !ifaces[i].isValid()) continue;

                QList<QNetworkAddressEntry> addr=ifaces[i].addressEntries();
                for(int a=0; a < addr.size(); a++)
                {
                    if ( addr[a].ip() == QHostAddress::LocalHost ) continue;
                    //                    if ( !addresses[a].toIPv4Address() ) continue;

                    QString ip = addr[a].ip().toString();
                    if ( ip.isEmpty() || (ip=="::1")) continue;

                    m_list.append(addr[a].ip());
//                    qDebug() << addr[a].ip().toString();
                }
            }
        }

        if(!m_list.isEmpty())
        {
            m_localIP.host=m_list[0].toIPv6Address();
            m_localIP.port=0;
        }
    }
    m_needMapping=true;
    m_numberIP=0;

    m_sessionID=0;
    m_userID=0;
    m_countPing=0;

    m_socket.bind(QHostAddress(m_localIP.host),m_localIP.port);
    m_localIP.port=m_socket.localPort();

    m_timer.start(2000);

    MyPacket packet;
    packet.setSignature(signature_no_RSA);
    packet.setCmd(cmdMappingPort);
    sendMessageToServer(packet);
}

MyServersChanal::~MyServersChanal()
{
    m_timer.stop();
    m_socket.close();
    disconnect(&m_socket,SIGNAL(readyRead()),this,SLOT(receiveMessageFromServer()));
    disconnect(&m_timer,SIGNAL(timeout()),this,SLOT(Timer30Second()));

    deleteLater();
}

void MyServersChanal::sendMessage(MyPacket &packet)
{
    sendMessageToServer(packet);
}

void MyServersChanal::setSessionID(qint32 sessionID)
{
    m_sessionID=sessionID;
    m_userID=0;
    m_countPing=0;
}

void MyServersChanal::Timer30Second()
{
    MyPacket packet;

    if(m_needMapping)
    {
        if(!m_flag)
        {
            if(!m_list.isEmpty())
            {
                m_numberIP++;
                if((m_numberIP>>2)>=m_list.count()) m_numberIP=0;
                m_localIP.host=m_list[m_numberIP>>2].toIPv6Address();
                m_localIP.port=0;
            }
            m_socket.close();
            m_socket.bind(QHostAddress(m_localIP.host),m_localIP.port);
            m_localIP.port=m_socket.localPort();
        }
        packet.setSignature(signature_no_RSA);
        packet.setCmd(cmdMappingPort);
        sendMessageToServer(packet);
    }

    if(m_sessionID)
    {
        m_countPing++;
        if(m_countPing>6)
        {
            emit ConnectServer(false);
            m_sessionID=0;
            m_userID=0;
            m_countPing=0;
            m_needMapping=true;
        }
        else
        {
            packet.setSignature(signature_server_RSA);
            packet.setCmd(cmdPing);
            packet.setSessionID(m_sessionID);
            sendMessageToServer(packet);
        }
    }
}

void MyServersChanal::receiveMessageFromServer()
{
    while(m_socket.hasPendingDatagrams())
    {
        parseMessage();
    }
}
