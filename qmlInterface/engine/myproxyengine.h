#ifndef MYPROXYENGINE_H
#define MYPROXYENGINE_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QImage>

#include "MyGlobal.h"

class MyProxyEngine : public QObject
{
    Q_OBJECT

    QString v_lastpage;
    QString v_homedir;
    void signalParsers(qint16 cmd, qint16 exitCode);
    void ChangePage(QString name);

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

    void logonOk(QString result);
    void logonError(QString result);
    void logoffExit(QString result);
    void registrOk(QString result);
    void registrError(QString result);
    void restorePswdOk(QString result);
    void restorePswdError(QString result);
    void changePswdOk(QString result);
    void changePswdError(QString result);

    void updateCurCameraOutSignal(QImage);

public slots:
    Q_INVOKABLE void command(QString cmd, QString param);

    void updateCurCameraInSlot(QImage kadr);

private slots:
    void AbonBookIsLoad();
    void AbonListIsLoad();
    void AbonWaitIsLoad();
    void MyProfileUpload();
    void AnyProfileDownload();
    void MyTorrentUpload();
    void TorrentSearchLoad();
};

#endif // MYPROXYENGINE_H
