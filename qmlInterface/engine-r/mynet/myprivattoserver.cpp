#include "myprivattoserver.h"
#include <QDebug>

qint64 MyPrivatToServer::sendMessageToServer(MyPacket &packet)
{
    QByteArray Datagram=packet.Encode(m_serverRSA);
    return m_socket.writeDatagram(Datagram,QHostAddress(m_serverIP.host),m_serverIP.port);
}

MyPacket MyPrivatToServer::recvMessageFromServer()
{
    MyPacket packet;

    qint32 length=m_socket.pendingDatagramSize();

    QByteArray Datagram;
    Datagram.resize(length);
    m_socket.readDatagram(Datagram.data(), length);

    packet.Decode(Datagram,m_clientRSAprivate);
    return packet;
}

void MyPrivatToServer::getChanal()
{
    MyPacket packet;
    packet.setSignature(signature_server_RSA);
    packet.setCmd(cmdGetPrivate);
    packet.setSessionID(m_sessionID);
    packet.setUserID(m_userID);
    packet.setExitCode(0);
    sendMessageToServer(packet);

    m_tick=0;
    m_attempt++;
}

void MyPrivatToServer::getChanalAnswer()
{
    if(m_packet.getCmd()==(cmdGetPrivate|cmdMailRu))
    {
        if(m_packet.isIPv6Address())
        {
            m_serverIP=m_packet.getIPv6Address();
            startSession();
        }
    }
}

void MyPrivatToServer::startSession()
{
    QString str=QHostAddress(m_serverIP.host).toString()+"/"+QString::number(m_serverIP.port);
    m_length=-1;
    m_step=2;
    m_attempt=0;
    m_tick=0;
    m_tcp.connectToHost(QHostAddress(m_serverIP.host),m_serverIP.port);
}

void MyPrivatToServer::writeCommand()
{
    QByteArray ba;
    MyPacket packet;
    packet.setSignature(signature_no_RSA);
    packet.setCmd(m_cmd);
    packet.setSessionID(m_sessionID);
    packet.setUserID(m_userID);
    packet.setExitCode(0);
    switch(m_cmd)
    {
        case cmdSetProfile:
        case cmdSetTorrent:
        case cmdGetAbonBook:
        case cmdGetAbonWait:
            packet.setAnyID(m_userID);
            break;
        case cmdGetProfile:
            packet.setAnyID(m_str.toInt());
            break;
        case cmdGetAbonList:
        case cmdGetTorrent:
            ba.clear();
            ba=m_str.toUtf8();
            packet.setData(ba);
            break;
    }

    QByteArray datagram=packet.Encode(m_serverRSA);

    writeData(datagram);
}

void MyPrivatToServer::writeData(QByteArray &data)
{
    qint32 length=data.size();
    QByteArray ba;
    ba.append((char *)&length,4);
    m_tcp.write(ba);
    m_tcp.write(data);
    m_balance+=(4+length);
}

void MyPrivatToServer::readData()
{
    if (m_length < 0)
    {
        if (m_tcp.bytesAvailable() < 4) return;
        m_tcp.read((char *)&m_length,4);
    }

    if (m_tcp.bytesAvailable() <= 0) return;

    m_data.append(m_tcp.readAll());
    if(m_data.size()<m_length) return;

    closeChanal(0);
}

void MyPrivatToServer::closeChanal(int code)
{
    m_timer.stop();
    m_socket.close();
    m_tcp.disconnectFromHost();
    m_tcp.close();
    disconnect(&m_tcp,0,0,0);
    disconnect(&m_socket,0,0,0);
    disconnect(&m_timer,0,0,0);

    emit Exit(this,code,m_data);
}

MyPrivatToServer::MyPrivatToServer(QObject *parent) :
    QObject(parent)
{
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(receiveMessage()));
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(Timer100msec()));

    connect(&m_tcp,SIGNAL(connected()),this,SLOT(connected()));
    connect(&m_tcp,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(&m_tcp,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWriten(qint64)));
    connect(&m_tcp,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorSocket(QAbstractSocket::SocketError)));
}

MyPrivatToServer::~MyPrivatToServer()
{
    deleteLater();
}

void MyPrivatToServer::execute(MyConfig *Config, quint16 Cmd, QString Str, QByteArray &Data)
{
    m_serverIP=Config->getServerAddress();
    m_localIP=Config->getLANAddress();
    m_localIP.port=0;
    m_sessionID=Config->getSessionID();
    m_userID=Config->getUserID();

    m_serverRSA       =Config->getServerRSA();
    m_clientRSAprivate=Config->getClientRSAprivate();
    m_clientRSApublic =Config->getClientRSApublic();

    m_socket.bind(QHostAddress(m_localIP.host),m_localIP.port);
    m_localIP.port=m_socket.localPort();

    m_cmd=Cmd;
    m_str=Str;
    if((m_cmd==cmdSetTorrent)||(m_cmd==cmdSetProfile))
        m_data=Data;
    else
        m_data.clear();

    m_eow=false;
    m_balance=0;
    m_step=1;
    m_attempt=0;
    getChanal();

    m_timer.start(100);
}

QString MyPrivatToServer::getParam()
{
    return m_str;
}

void MyPrivatToServer::Timer100msec()
{
    m_tick++;

    if(m_tick>42)
    {
        switch(m_step)
        {
            case 1:
                if(++m_attempt>3)
                    emit Exit(this,99,m_data);
                else
                    getChanal();
                break;
            case 2:
                if(m_eow && (m_balance==0)) closeChanal(0); else closeChanal(99);
        }
    }
}

void MyPrivatToServer::receiveMessage()
{
    while(m_socket.hasPendingDatagrams())
    {
        m_packet=recvMessageFromServer();
        switch(m_step)
        {
            case 1: getChanalAnswer(); break;
        }
    }
}

void MyPrivatToServer::connected()
{
    writeCommand();
    switch(m_cmd)
    {
        case cmdSetProfile:
        case cmdSetTorrent:
            writeData(m_data);
            m_eow=true;
    }
}

void MyPrivatToServer::readyRead()
{
    readData();
}

void MyPrivatToServer::bytesWriten(qint64 size)
{
    m_balance-=size;
    if(m_eow && (m_balance==0)) closeChanal(0);
}

void MyPrivatToServer::errorSocket(QAbstractSocket::SocketError error)
{
    closeChanal(100+error);
}
