#ifndef MYVIDEOMANAGER_H
#define MYVIDEOMANAGER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QList>
#include <QStringList>
#include <QImage>

#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
#include "../mynet/myclientchanal.h"
#include "myvideoitem.h"

class MyVideoManager : public QObject
{
    Q_OBJECT

    MyConfig  *m_config;
    QList<MyVideoItem *> m_list;

    qint32 m_ttl;

    void checkOnline();

public:
    explicit MyVideoManager(QObject *parent = 0);
    ~MyVideoManager();

    void init(MyConfig *config);

    qint32 getCountConnect();
    qint32 getUserID(qint32 index);
    MyClientChanal *getChanal(qint32 index);

    void Connect(qint32 UserID, bool proxy=false);
    void Disconnect(qint32 UserID);

 signals:
    void process(qint32);
    void finished();
    void updateVideoIn(QImage);

 public slots:
    void addConnect(MyClientChanal *chanal);
    void statusClient(qint32 UserID,qint32 status);
    void updateVideoOut(QImage kadr);
};

#endif // MYVIDEOMANAGER_H
