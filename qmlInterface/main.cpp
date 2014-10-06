#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include "qtquick2applicationviewer.h"
#include "terminalimageprovider.h"
#include "terminalQmlPlugin.h"
#include "core.h"
#include <QQmlEngine>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "applicationmodel.h"

#include "engine/myproxyengine.h"

QString initialData()
{
    QString homeDir=Core::dataDir().toLocalFile();
    if (!QFileInfo::exists(homeDir))
    {
        QDir dataDir;
        dataDir.mkpath(homeDir);
    }

    if (!QFileInfo::exists(homeDir + "users/"))
    {
        QDir usersDir;
        usersDir.mkpath(homeDir + "users/");
    }

    if (!QFileInfo::exists(homeDir + "chats/"))
    {
        QDir usersDir;
        usersDir.mkpath(homeDir + "chats/");
    }

    if (!QFileInfo::exists(homeDir + "smiles/"))
    {
        QDir usersDir;
        usersDir.mkpath(homeDir + "smiles/");
    }

    if (!QFileInfo::exists(homeDir+"/home"))
    {
        QDir usersDir;
        usersDir.mkpath(homeDir+"/home");
    }

    QStringList dataFiles = QStringList() << "i.xml" << "torrent.xml"
                                          << "all.xml" << "contacts.xml" << "visitors.xml"
                                          << "settings.xml";

//    QStringList dataFiles = QStringList() << "menu.xml" << "all.xml" << "visitors.xml"
//                                          << "users/1.xml" << "users/2.xml" << "users/3.xml"
//                                          << "users/4.xml" << "users/5.xml" << "users/6.xml"
//                                          << "users/7.xml" << "users/8.xml" << "users/9.xml"
//                                          << "users/10.xml";

    foreach (QString dataFile, dataFiles)
    {
        QString dataFilePath = homeDir + dataFile;
        if (!QFileInfo::exists(dataFilePath))
        {
            qDebug() << QObject::tr("Copy \":/data/%0\" to \"%1\".").arg(dataFile).arg(dataFilePath);
            QFile::copy(":/data/" + dataFile, dataFilePath);
        }
        QFile file (dataFilePath);
        file.setPermissions(file.permissions() | QFile::WriteOwner);
    }

    QDir smilesDir ("://qml/terminal/smiles/");
    foreach (QFileInfo smileFile, smilesDir.entryInfoList())
    {
        QString localSmileFilePath = homeDir + "smiles/" + smileFile.fileName();
        if (!QFileInfo::exists(localSmileFilePath))
        {
            qDebug() << QObject::tr("Copy \"%0\" to \"%1\".").arg(smileFile.filePath()).arg(localSmileFilePath);
            QFile::copy(smileFile.filePath(), localSmileFilePath);
        }
    }
    return homeDir;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("itQuasar");
    app.setApplicationName("iqTerminal");

    QString homeDir=initialData();

    TerminalQmlPlugin terminalQmlPlugin;
    terminalQmlPlugin.registerTypes("TerminalQmlPlugin");

    QQmlApplicationEngine engine;
    //Главная модель данных
    //В С++ доступ к ней можно получить, используя ApplicationModel::instance() в любом месте кода
    engine.rootContext()->setContextProperty("applicationModel", ApplicationModel::instance());

//----------------------------------------------------------------------------
// прокси к движку
    MyProxyEngine proxy(homeDir);
    engine.rootContext()->setContextProperty("proxy",&proxy);
//----------------------------------------------------------------------------

    engine.addImageProvider("xml", new TerminalImageProvider);
    engine.load(QUrl("qrc:/qml/terminal/main.qml"));

    return app.exec();
}
