#include "myproxyengine.h"
#include <QFile>

#include <QDebug>
#include "applicationmodel.h"

void MyProxyEngine::signalParsers(qint16 cmd, qint16 exitCode)
{
    qDebug() << "answer command: " << cmd << "=" << exitCode;

    switch(cmd)
    {
        case cmdLogon:
            switch(exitCode)
            {
                case  0: emit logonOk(""); command(cmdEsetprofile,""); command(cmdEsettorrent,""); break;
                case  1:
                case  6: emit logonError("Неверный логин или пароль"); break;
                case 99: emit logonError("Сервер не отвечает"); break;
                default: emit logonError("Ошибка авторизации ="+QString::number(exitCode));
            }
            break;

        case cmdLogoff:
            emit logoffExit("OK");
            break;

        case cmdRegistr:
            switch(exitCode)
            {
                case  0:
                case  4: emit registrOk("Успешная регистрация"); break;
                case  1: emit registrError("Неверный e-mail"); break;
                case  2: emit registrError("Логин занят"); break;
                case  3: emit registrError("e-mail занят"); break;
                case 99: emit registrError("Сервер не отвечает"); break;
                default: emit registrError("Ошибка регистрации ="+QString::number(exitCode));
            }
            break;

        case cmdRestorePswd:
            switch(exitCode)
            {
                case  0:
                case  5: emit restorePswdOk("Пароль выслан на почту"); break;
                case  1:
                case  3: emit restorePswdError("Неверный e-mail"); break;
                case 99: emit restorePswdError("Сервер не отвечает"); break;
                default: emit restorePswdError("Ошибка восстановления пароля ="+QString::number(exitCode));
            }
            break;

        case cmdChangePswd:
            switch(exitCode)
            {
                case 0:
                case  8: emit changePswdOk("Пароль изменен"); break;
                case  1: emit changePswdError("Слабый пароль"); break;
                case  6:
                case  9: emit changePswdError("Неверный логин или пароль"); break;
                case 99: emit changePswdError("Сервер не отвечает"); break;
                default: emit changePswdError("Ошибка изменения пароля ="+QString::number(exitCode));
            }
            break;

        case cmdUpdateDynam:
            break;
        case cmdAddAbonBook:
            break;
        case cmdDelAbonBook:
            break;
    }
}

void MyProxyEngine::ChangePage(QString name)
{
    if(v_lastpage=="i") command(cmdEsetprofile,"");
    if(v_lastpage=="torrent") command(cmdEsettorrent,"");
    v_lastpage=name;
}

MyProxyEngine::MyProxyEngine(QString homeDir, QObject *parent) :
    QObject(parent)
{
    v_homedir=homeDir;
}

void MyProxyEngine::command(QString cmd, QString param)
{
    qDebug() << "command: " << cmd << "," << param;
    if(cmd==cmdElogon)   signalParsers(cmdLogon,0);
    if(cmd==cmdElogoff)  signalParsers(cmdLogoff,0);
    if(cmd==cmdEregistr) signalParsers(cmdRegistr,0);
    if(cmd==cmdErestore) signalParsers(cmdRestorePswd,0);
    if(cmd==cmdEchange)  signalParsers(cmdChangePswd,0);
    if(cmd==cmdEdynam)   signalParsers(cmdUpdateDynam,0);

    if(cmd==cmdEabonbook) AbonBookIsLoad();
    if(cmd==cmdEabonlist) AbonListIsLoad();
    if(cmd==cmdEabonwait) AbonWaitIsLoad();

    if(cmd==cmdEabonadd)  signalParsers(cmdAddAbonBook,0);
    if(cmd==cmdEabondel)  signalParsers(cmdDelAbonBook,0);

    if(cmd==cmdEsetprofile) MyProfileUpload();
    if(cmd==cmdEgetprofile) AnyProfileDownload();
    if(cmd==cmdEsettorrent) MyTorrentUpload();
    if(cmd=="search") TorrentSearchLoad();
    if(cmd=="show") ChangePage(param);
}

void MyProxyEngine::updateCurCameraInSlot(QImage kadr)
{
    emit updateCurCameraOutSignal(kadr);
}

void MyProxyEngine::AbonBookIsLoad()
{
    qDebug() << "AbonBook is load";

    QByteArray book;
    book.append(BOM);
    book.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    book.append("<all>\n");
    book.append("<user id=\"95\">\n");
    book.append("<profile>95.xml</profile>\n");
    book.append("<online>false</online>\n");
    book.append("<friendship_accepted>true</friendship_accepted>\n");
    book.append("</user>\n");
    book.append("<user id=\"4141\">\n");
    book.append("<profile>4141.xml</profile>\n");
    book.append("<online>true</online>\n");
    book.append("<friendship_accepted>true</friendship_accepted>\n");
    book.append("</user>\n");
    book.append("<user id=\"39\">\n");
    book.append("<profile>39.xml</profile>\n");
    book.append("<online>true</online>\n");
    book.append("<friendship_accepted>false</friendship_accepted>\n");
    book.append("</user>\n");
    book.append("<user id=\"4321\">\n");
    book.append("<profile>4321.xml</profile>\n");
    book.append("<online>true</online>\n");
    book.append("<friendship_accepted>false</friendship_accepted>\n");
    book.append("</user>\n");
    book.append("</all>\n");

    QFile file;
    file.setFileName(v_homedir+QString("/contacts.xml"));
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(book);
        file.close();
    }

    ApplicationModel * appModel=ApplicationModel::instance();
    appModel->contactsModel()->reload();
    emit abonBookLoaded();
}

void MyProxyEngine::AbonListIsLoad()
{
    qDebug() << "AbonList is load";

    QByteArray book;
    book.append(BOM);
    book.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    book.append("<all>\n");
    book.append("<user id=\"1\">\n");
    book.append("<profile>1.xml</profile>\n");
    book.append("</user>\n");
    book.append("<user id=\"2\">\n");
    book.append("<profile>2.xml</profile>\n");
    book.append("</user>\n");
    book.append("<user id=\"3\">\n");
    book.append("<profile>3.xml</profile>\n");
    book.append("</user>\n");
    book.append("<user id=\"4\">\n");
    book.append("<profile>4.xml</profile>\n");
    book.append("</user>\n");
    book.append("<user id=\"5\">\n");
    book.append("<profile>5.xml</profile>\n");
    book.append("</user>\n");
    book.append("</all>\n");

    QFile file;
    file.setFileName(v_homedir+QString("/all.xml"));
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(book);
        file.close();
    }

    ApplicationModel * appModel=ApplicationModel::instance();
    appModel->allModel()->reload();
    emit abonListLoaded();
}

void MyProxyEngine::AbonWaitIsLoad()
{
    qDebug() << "AbonWait is load";

    QByteArray book;
    book.append(BOM);
    book.append("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n");
    book.append("<all>\n");
    book.append("<user id=\"10\">\n");
    book.append("<profile>10.xml</profile>\n");
    book.append("</user>\n");
    book.append("<user id=\"20\">\n");
    book.append("<profile>20.xml</profile>\n");
    book.append("</user>\n");
    book.append("<user id=\"30\">\n");
    book.append("<profile>30.xml</profile>\n");
    book.append("</user>\n");
    book.append("</all>\n");

    QFile file;
    file.setFileName(v_homedir+QString("/visitors.xml"));
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(book);
        file.close();
    }

    ApplicationModel * appModel=ApplicationModel::instance();
    appModel->visitorsModel()->reload();

    emit abonWaitLoaded();
}

void MyProxyEngine::MyProfileUpload()
{
    qDebug() << "MyProfile upload";
    emit myProfileUpLoaded();
}

void MyProxyEngine::AnyProfileDownload()
{
    qDebug() << "AnyProfile download";
    emit anyProfileDownLoaded();
}

void MyProxyEngine::MyTorrentUpload()
{
    qDebug() << "MyTorrent upload";
    emit myTorrentUpLoaded();
}

void MyProxyEngine::TorrentSearchLoad()
{
    qDebug() << "search is load";
    QByteArray book;

    QFile file;
    file.setFileName(v_homedir+QString("searchA.xml"));
    if(file.open(QIODevice::ReadOnly))
    {
        book=file.readAll();
        file.close();
    }

    file.setFileName(v_homedir+QString("search.xml"));
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(book);
        file.close();
    }

    ApplicationModel * appModel=ApplicationModel::instance();
    appModel->torrents()->searchModel()->reload();


}
