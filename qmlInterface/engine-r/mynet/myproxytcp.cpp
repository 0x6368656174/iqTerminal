#include "myproxytcp.h"
#include <QUrl>

void MyProxyTcp::startSession()
{
    qDebug() << "connect to host";
    m_tcp.connectToHost(QHostAddress(m_config->getProxyAddress().host),3498);

    m_step=0;
    m_tick=0;
    m_timer.start(100);
}

void MyProxyTcp::writeCommand()
{
    qDebug() << "write command";

    m_tick=0;

    QByteArray hdr;
    QByteArray data=m_filename.toUtf8();

    qint32 length=sizeof(qint32)+sizeof(qint32)+sizeof(qint64)+data.size();

    hdr.append((char *)&length,sizeof(qint32));
    hdr.append((char *)&m_direction,sizeof(qint32));
    hdr.append((char *)&m_userID,sizeof(qint32));
    hdr.append((char *)&m_filelength,sizeof(qint64));
    m_tcp.write(hdr);
    m_tcp.write(data);
    m_balance+=(sizeof(qint32)+length);
}

void MyProxyTcp::writeData()
{
//    qDebug() << "write data";
    m_tick=0;

    QByteArray data;
    data=m_file.read(4096);
    if(data.size()>0)
    {
        m_tcp.write(data);
        m_filelength-=data.size();
        m_balance+=data.size();
    }
    else
        closeChanal(98);
}

void MyProxyTcp::readData()
{
//    qDebug() << "read data";

    if(m_direction!=1) return;

    m_tick=0;

    if (m_step==0)
    {
        if (m_tcp.bytesAvailable() < sizeof(qint64)) return;
        m_tcp.read((char *)&m_filelength,sizeof(qint64));
        m_step=1;
    }

    if (m_tcp.bytesAvailable() <= 0) return;

    QByteArray data;
    data.append(m_tcp.readAll());
    m_file.write(data);
    m_filelength-=data.size();

    if(m_filelength==0) closeChanal(0);
}

void MyProxyTcp::closeChanal(int code)
{
    qDebug() << "close chanal" << code;

    m_timer.stop();
    m_tcp.disconnectFromHost();
    m_tcp.close();
    disconnect(&m_tcp,0,0,0);
    disconnect(&m_timer,0,0,0);

    m_file.close();

    if(m_direction==0) code=1000;
    emit Exit(this,code,m_userIDfrom,m_path);
}

MyProxyTcp::MyProxyTcp(QObject *parent) :
    QObject(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(Timer100msec()));

    connect(&m_tcp,SIGNAL(connected()),this,SLOT(connected()));
    connect(&m_tcp,SIGNAL(readyRead()),this,SLOT(readyRead()));
    connect(&m_tcp,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWriten(qint64)));
    connect(&m_tcp,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(errorSocket(QAbstractSocket::SocketError)));
}

MyProxyTcp::~MyProxyTcp()
{
    deleteLater();
}

void MyProxyTcp::unloadFile(MyConfig *config, qint32 UserID, qint32 id, QString FileName)
{
    m_config=config;
    m_userIDfrom=UserID;
    m_userID=UserID;

    QString result="%1-%2-%3";
    QUrl path(FileName);
    m_filename=result.arg(m_config->getUserID()).arg(id).arg(path.fileName());

    m_direction=0;
    m_file.setFileName(path.toLocalFile());
    m_file.open(QIODevice::ReadOnly);
    m_filelength=m_file.size();

    qDebug() << "unload" << m_filename;
    startSession();
}

void MyProxyTcp::downloadFile(MyConfig *config, qint32 UserID, QString FileName)
{
    m_config=config;
    m_userIDfrom=UserID;
    m_userID=m_config->getUserID();
    m_path=FileName;

    QUrl path(FileName);
    m_filename=path.fileName();

    m_direction=1;
    m_file.setFileName(path.toLocalFile());
    m_file.open(QIODevice::WriteOnly);
    m_filelength=0;
    startSession();
}

void MyProxyTcp::Timer100msec()
{
    m_tick++;
    if(m_tick>50) closeChanal(99);
}

void MyProxyTcp::connected()
{
    qDebug() << "connected";
    m_balance=0;
    writeCommand();
}

void MyProxyTcp::readyRead()
{
    readData();
}

void MyProxyTcp::bytesWriten(qint64 size)
{
//    qDebug() << "bytes writen" << size;
    m_balance-=size;
    if(m_balance==0)
    {
        switch(m_direction)
        {
            case 0:
                if(m_step==0) m_step=1;
                if(m_filelength>0) writeData(); else closeChanal(0);
                break;

            case 1:
                break;
        }
    }
}

void MyProxyTcp::errorSocket(QAbstractSocket::SocketError error)
{
    qDebug() << "error socket" << error;
    closeChanal(100+error);
}
