#ifndef MYFILELOADBLOCK_H
#define MYFILELOADBLOCK_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include "../common/mypacket.h"
#include "../common/myconfig.h"
#include "../mynet/myclientchanal.h"

class MyFileLoadBlock : public QObject
{
    Q_OBJECT

    QTimer m_timer;
    MyConfig *m_config;
    MyClientChanal *m_chanal;
    qint32 m_userID;
    qint32 m_messageID;
    QString m_filename;
    qint32 m_attempt;
    qint32 m_state;

    qint64 m_filelength;
    qint64 m_block;
    quint8 m_mask;
    quint8 m_mask_return;

    QByteArray m_buf[8];

    void addConnect(bool proxy);
    void sendFileName();
    void recvFileParam(MyPacket &packet);
    void sendQueryData();
    void recvFileData(MyPacket &packet);

public:
    explicit MyFileLoadBlock(QObject *parent = 0);
    ~MyFileLoadBlock();

    void init(MyConfig *config, qint32 UserID, qint32 MessageID, QString FileName);

    qint64 getFileLength();
    qint64 getBlock();
    quint8 getMask();
    QByteArray getData(qint32 index);

    void setBlock(qint64 block, quint8 mask);

signals:
    void blockCompleted(MyFileLoadBlock *);
    void statusBlock(MyFileLoadBlock *, qint32 status);

public slots:

private slots:
    void timer100msec();
    void recvPacket(MyClientChanal *chanal, MyPacket &packet);
    void statusClient(MyClientChanal *chanal, qint32 status);
};
#endif // MYFILELOADBLOCK_H
