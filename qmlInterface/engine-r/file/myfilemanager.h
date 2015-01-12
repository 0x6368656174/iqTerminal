#ifndef MYFILEMANAGER_H
#define MYFILEMANAGER_H

#include <QObject>
#include <QList>
#include <QTimer>
#include "../MyGlobal.h"
#include "../common/myconfig.h"
#include "../common/mypacket.h"
#include "../common/myringtone.h"
#include "../mynet/myclientchanal.h"
#include "myfileload.h"
#include "myfileunload.h"

class MyFileManager : public QObject
{
    Q_OBJECT

    MyConfig *m_config;

    QList <MyFileLoad *> m_list;
    QTimer m_timer;

    void sound(qint32 state);
    void mark(qint32 userID, QString fileName);

public:
    explicit MyFileManager(QObject *parent = 0);
    ~MyFileManager();

    void init(MyConfig *config);
    void loadFile(QString filename);

signals:

public slots:
    void addConnect(MyClientChanal *chanal);

private slots:
    void timer1sec();
    void stopRingtone(MyRingtone *ringtone);
};

#endif // MYFILEMANAGER_H
