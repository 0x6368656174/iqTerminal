#ifndef MYPROXYENGINE_H
#define MYPROXYENGINE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QImage>

#include "message.h"
#include "MyGlobal.h"
#include "common/myconfig.h"
#include "nat/mynatdetector.h"
#include "mynet/myserverschanal.h"
#include "mynet/mydialog.h"
#include "mynet/myprivattoserver.h"
#include "mynet/myfirstmanager.h"
#include "sms/mysmsmanager.h"
#include "audio/myaudiomanager.h"
#include "file/myfilemanager.h"
#include "video/myvideomanager.h"

class MyProxyEngine : public QObject
{
    Q_OBJECT

    MyConfig        *m_config;
    MyServersChanal *m_server;
    MyDialog        *m_dialog;
    MyNATDetector   *m_nat;
    MyFirstManager  *m_first;
    MySMSManager    *m_sms;
    MyAudioManager  *m_audio;
    MyFileManager   *m_file;
    MyVideoManager  *m_video;

    QString m_lastpage;
    qint32 m_abonbook, m_abonwait, m_abonlist, m_chat;

    QString extractParam(QString param, qint32 n);
    void createDialog(qint16 cmd, QString param);
    void signalParsers(qint16 cmd, qint16 exitCode);
    void loadAbonents(quint16 cmd, QString param);
    void setMyProfile();
    void getAnyProfile(QString profile);
    void setMyTorrent();
    void searchTorrent(QString param);
    void ChangePage(QString name);
    void loadProfiles(QByteArray Book);
    bool isBOM(QByteArray &Book);

    void openChat (QString index);
    void openAudio(QString UserID);
    void openVideo(QString UserID);
    void closeVideo(QString UserID);
    void loadFile (QString filename);

public:
    explicit MyProxyEngine(QString homeDir, QObject *parent = 0);

signals:
//    void eventEngine(QString event, QString param, qint32 exitcode);

    void abonBookLoaded();
    void abonBookNotLoaded();
    void abonListLoaded();
    void abonListNotLoaded();
    void abonWaitLoaded();
    void abonWaitNotLoaded();

    void myProfileUpLoaded();
    void myProfileNotUpLoaded();
    void anyProfileDownLoaded();
    void anyProfileNotDownLoaded();
    void myTorrentUpLoaded();
    void myTorrentNotUpLoaded();

    void torrentSearchLoaded();
    void torrentSearchNotLoaded();

    void logonOk(QString result);
    void logonError(QString result);
    void logoffExit(QString result);
    void registrOk(QString result);
    void registrError(QString result);
    void restorePswdOk(QString result);
    void restorePswdError(QString result);
    void changePswdOk(QString result);
    void changePswdError(QString result);

    void chatNewMessage(QString value);

    void updateHomeDir(QString value);
    void updateDownloadDir(QString value);
    void updateServerIP(QString value);
    void updateStunIP(QString value);

    void updateLAN(QString value);
    void updateWAN(QString value);
    void updateConnectStatus(QString value);
    void updateLogonStatus(QString value);
    void updateUserID(QString value);
    void updateSessionID(QString value);
    void updateLogin(QString value);

    void updateCurCameraOutSignal(QImage);

public slots:
    Q_INVOKABLE void command(QString cmd, QString param);
    Q_INVOKABLE void sendMessage(Message *value);

    void setParam(QString param, QVariant value);
    void updateCurCameraInSlot(QImage kadr);
    void updateCurCameraOutSlot(QImage kadr);

private slots:
    void DialogAnswer(MyDialog *dialog,qint16 cmd, qint16 exitCode);
    void StartNATdetect();
    void NATDetected(qint32 typeNAT, qint32 typePort, qint32 errorCode);
    void updateParam(QString name, QVariant value);

    void AbonBookIsLoad(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book);
    void AbonListIsLoad(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book);
    void AbonWaitIsLoad(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book);
    void MyProfileUpload(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book);
    void AnyProfileDownload(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book);
    void MyTorrentUpload(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book);
    void TorrentSearchLoad(MyPrivatToServer *chanal, qint32 exitCode, QByteArray &Book);
};

#endif // MYPROXYENGINE_H
