#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "terminalimageprovider.h"
#include "terminalQmlPlugin.h"
#include "core.h"
#include <QQmlEngine>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

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

    QStringList dataFiles = QStringList() << "i.xml" << "menu.xml" << "torrent.xml"
                                          << "all.xml" << "contacts.xml" << "visitors.xml";

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
            qDebug() << QString("Copy \":/data/%0\" to \"%1\".").arg(dataFile).arg(dataFilePath);
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
            qDebug() << QString("Copy \"%0\" to \"%1\".").arg(smileFile.filePath()).arg(localSmileFilePath);
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

    QtQuick2ApplicationViewer viewer;

    QString homeDir=initialData();

    TerminalQmlPlugin terminalQmlPlugin;
    terminalQmlPlugin.registerTypes("TerminalQmlPlugin");

//----------------------------------------------------------------------------
// прокси к движку
    MyProxyEngine proxy(homeDir);
    viewer.engine()->rootContext()->setContextProperty("proxy",&proxy);
//----------------------------------------------------------------------------

    viewer.engine()->addImageProvider("xml", new TerminalImageProvider);
    viewer.setSource(QUrl("qrc:/qml/terminal/main.qml"));
    viewer.setTitle("iqTerminal");

#ifdef Q_OS_ANDROID
    viewer.showFullScreen();
#else
    viewer.showExpanded();
#endif

    return app.exec();
}
