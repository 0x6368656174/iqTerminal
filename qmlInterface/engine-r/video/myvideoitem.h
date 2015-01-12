#ifndef MYVIDEOITEM_H
#define MYVIDEOITEM_H

#include <QObject>
#include <QTimer>
#include <QByteArray>
#include <QBitArray>
#include <QImage>

#include "../common/mypacket.h"
#include "../mynet/myclientchanal.h"

class MyVideoItem : public QObject
{
    Q_OBJECT

    QTimer m_timer;
    MyClientChanal *m_chanal;

    qint32 m_kadrInID;
    QImage m_kadrIn;

    qint32 m_kadrOutID;
    QImage m_kadrOut;

    bool m_inWork;
    bool m_newKadrOut;

    // блок передачи видео
    qint32 m_tickOut;
    QByteArray m_byteOut;
    QBitArray  m_bitOut;

    qint32 m_kadrOutIDw;

    // блок приема видео
    qint32 m_tickIn;
    QByteArray m_byteIn;
    QBitArray  m_bitIn;

    // функции передачи видео
    void sendFrame(qint32 n);
    void sendKadr();
    void checkNextOut();
    void setNextKadr(qint32 id, QByteArray kadr);

    void sendAnswer(MyVideoBlock &block);
    void checkKadrIn();
    void buildPacket(MyPacket &packet);

public:
    explicit MyVideoItem(MyClientChanal *chanal, QObject *parent = 0);
    ~MyVideoItem();

    MyClientChanal *getChanal();

 signals:
    void newStatus(qint32 UserID, qint32 status);
    void updateVideoIn(QImage);

 public slots:
    void updateVideoOut(QImage kadr);

 private slots:
    void receivePacket(MyClientChanal *chanal, MyPacket &packet);
    void statusClient(MyClientChanal *chanal, qint32 status);
    void timer15msec();
};

#endif // MYVIDEOITEM_H
