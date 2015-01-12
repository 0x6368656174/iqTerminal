#ifndef MYSMSMANAGER_H
#define MYSMSMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QStringList>

#include "user.h"
#include "usersmodel.h"
#include "message.h"
#include "messagesmodel.h"

#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
#include "../common/myringtone.h"
#include "../mynet/myclientchanal.h"
#include "../mynet/myproxychanal.h"
#include "../mynet/myproxytcp.h"

class MySMSManager : public QObject
{
    Q_OBJECT

private:
    bool m_isStarted;

    MyConfig *m_config;
    MyProxyChanal *m_proxy;
    QList<MyClientChanal*> m_list;
    QList<qint32> m_userID;

    QByteArray encodeMessage(Message *msg);
    qint32 decodeMessage(Message *msg, QByteArray &data);

    void addSms(qint32 UserID);
    void delSms(qint32 UserID);

    void sound();
    void addChanal(qint32 UserID, bool proxy);

public:
    explicit MySMSManager(QObject *parent = 0);
    ~MySMSManager();

    void init(MyConfig *config);
    void execute();

    qint32 getCountConnect();
    qint32 getUserID(qint32 index);
    MyClientChanal *getChanal(qint32 index);

    void Connect(qint32 index);
    void Disconnect(qint32 UserID);
    void sendMessage(qint32 index, Message *msg);

    void checkSms();
    void checkOnline();
    void clearSms(int index);

    bool isStarted();

signals:
    void loadFile(QString cmd, QString file);
    void sendToProxy(qint32 UserID, MyPacket &packet);

public slots:
    void recvSMS(MyClientChanal *chanal, MyPacket &packet);
    void statusClient(MyClientChanal *chanal, qint32 status);
    void addConnect(MyClientChanal *chanal);

private slots:
    void stopRingtone(MyRingtone *ringtone);
    void recvSMS(qint32 UserID, MyPacket &packet, bool proxy);
    void exitProxy(MyProxyTcp *chanal, qint32 code, qint32 UserID, QString fileName);
};

#endif // MYSMSMANAGER_H
