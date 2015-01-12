#include "myproxychanal.h"

void MyProxyChanal::sendRequestToProxy()
{
    MyPacket packet;
    packet.setSignature(signature_no_RSA);
    packet.setCmd(cmdCheckSMS);
    packet.setSessionID(m_config->getSessionID());
    packet.setUserID(m_config->getUserID());
    packet.setExitCode(0);
    packet.setStatus(0);

    QByteArray Datagram=packet.Encode(m_ServerKey);
    m_socket.writeDatagram(Datagram,QHostAddress(m_proxyIP.host),m_proxyIP.port);

//    qDebug() << "send request to proxy";
}

MyProxyChanal::MyProxyChanal(QObject *parent) :
    QObject(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(timer1sec()));
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(getDatagramm()));
}

void MyProxyChanal::execute(MyConfig *config)
{
    m_config=config;
    m_ServerKey=m_config->getServerRSA();
    m_ClientKey=m_config->getClientRSApublic();
    m_proxyIP=m_config->getProxyAddress();

    m_socket.bind(QHostAddress(m_config->getLANAddress().host),0);
    m_timer.start(15000);
}

void MyProxyChanal::sendMessage(qint32 UserID, MyPacket &packet)
{
    QByteArray Datagram=packet.Encode(m_ClientKey);

    MyPacket packet2;
    packet2.setSignature(signature_no_RSA);
    packet2.setCmd(cmdSendMessage);
    packet2.setSessionID(m_config->getSessionID());
    packet2.setUserID(m_config->getUserID());
    packet2.setExitCode(0);
    packet2.setAnyID(UserID);
    packet2.setData(Datagram);

    QByteArray Datagram2=packet2.Encode(m_ServerKey);
    m_socket.writeDatagram(Datagram2,QHostAddress(m_proxyIP.host),m_proxyIP.port);

//    qDebug() << "send message to proxy" << UserID;
}

void MyProxyChanal::timer1sec()
{
    sendRequestToProxy();
}

void MyProxyChanal::getDatagramm()
{
    QHostAddress addr;
    quint16 port;
    QByteArray Datagram;
    MyPacket packet;
    QByteArray Datagram2;
    MyPacket packet2;

    while((m_socket.state()==QUdpSocket::BoundState) && m_socket.hasPendingDatagrams())
    {
        qint32 length=m_socket.pendingDatagramSize();

        Datagram2.resize(length);

        m_socket.readDatagram(Datagram2.data(), length, &addr, &port);
        packet2.Decode(Datagram2,m_ServerKey);

        if((packet2.getCmd()==cmdSendMessage) && (packet2.isData()))
        {
            Datagram=packet2.getData();

            packet.Decode(Datagram,m_ClientKey);
            emit recvMessage(packet2.getUserID(),packet,true);
//            qDebug() << "recv message from proxy" << packet2.getUserID();
        }

    }

    sendRequestToProxy();
}
