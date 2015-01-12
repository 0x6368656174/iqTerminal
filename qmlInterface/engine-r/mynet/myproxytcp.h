#ifndef MYPROXYTCP_H
#define MYPROXYTCP_H

#include <QObject>
#include <QTimer>
#include <QHostAddress>
#include <QTcpSocket>

#include <QByteArray>
#include <QString>
#include <QFile>

#include "../MyGlobal.h"
#include "../common/mypacket.h"
#include "../common/myconfig.h"

class MyProxyTcp : public QObject
{
    Q_OBJECT

    QTimer m_timer;
    QTcpSocket m_tcp;

    MyConfig *m_config;
    qint32 m_direction;
    qint32 m_userID;
    qint32 m_userIDfrom;
    QString m_filename;
    QString m_path;
    QFile m_file;
    qint64 m_filelength;

    qint32 m_step;
    qint32 m_tick;
    qint32 m_balance;

    void startSession();
    void writeCommand();

    void writeData();
    void readData();
    void closeChanal(int code);

public:
    explicit MyProxyTcp(QObject *parent = 0);
    ~MyProxyTcp();

    void unloadFile(MyConfig *config, qint32 UserID, qint32 id, QString FileName);
    void downloadFile(MyConfig *config, qint32 UserID, QString FileName);

signals:
    void Exit(MyProxyTcp *, qint32, qint32, QString);

public slots:
    void Timer100msec();
    void connected();
    void readyRead();
    void bytesWriten(qint64 size);
    void errorSocket(QAbstractSocket::SocketError error);

};

#endif // MYPROXYTCP_H
