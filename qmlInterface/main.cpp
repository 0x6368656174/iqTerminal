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

void initialData()
{
    if (!QFileInfo::exists(Core::dataDir().toLocalFile()))
    {
        QDir dataDir;
        dataDir.mkpath(Core::dataDir().toLocalFile());
    }

    if (!QFileInfo::exists(Core::dataDir().toLocalFile() + "users/"))
    {
        QDir usersDir;
        usersDir.mkpath(Core::dataDir().toLocalFile() + "users/");
    }

    if (!QFileInfo::exists(Core::dataDir().toLocalFile() + "chats/"))
    {
        QDir usersDir;
        usersDir.mkpath(Core::dataDir().toLocalFile() + "chats/");
    }

    if (!QFileInfo::exists(Core::dataDir().toLocalFile() + "smiles/"))
    {
        QDir usersDir;
        usersDir.mkpath(Core::dataDir().toLocalFile() + "smiles/");
    }

    QStringList dataFiles = QStringList() << "i.xml" << "menu.xml" << "torrent.xml"
                                          << "all.xml" << "contacts.xml" << "visitors.xml"
                                          << "users/1.xml" << "users/2.xml" << "users/3.xml"
                                          << "users/4.xml" << "users/5.xml" << "users/6.xml"
                                          << "users/7.xml" << "users/8.xml" << "users/9.xml"
                                          << "users/10.xml"
                                          << "chats/1.xml" << "chats/2.xml" << "chats/3.xml"
                                          << "chats/4.xml" << "chats/5.xml" << "chats/6.xml"
                                          << "chats/7.xml" << "chats/8.xml" << "chats/9.xml"
                                          << "chats/10.xml";
//    QStringList dataFiles = QStringList() << "menu.xml" << "all.xml" << "visitors.xml"
//                                          << "users/1.xml" << "users/2.xml" << "users/3.xml"
//                                          << "users/4.xml" << "users/5.xml" << "users/6.xml"
//                                          << "users/7.xml" << "users/8.xml" << "users/9.xml"
//                                          << "users/10.xml";

    foreach (QString dataFile, dataFiles)
    {
        QString dataFilePath = Core::dataDir().toLocalFile() + dataFile;
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
        QString localSmileFilePath = Core::dataDir().toLocalFile() + "smiles/" + smileFile.fileName();
        if (!QFileInfo::exists(localSmileFilePath))
        {
            qDebug() << QString("Copy \"%0\" to \"%1\".").arg(smileFile.filePath()).arg(localSmileFilePath);
            QFile::copy(smileFile.filePath(), localSmileFilePath);
        }
    }
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName("itQuasar");
    app.setApplicationName("iqTerminal");

    QtQuick2ApplicationViewer viewer;

    initialData();

    TerminalQmlPlugin terminalQmlPlugin;
    terminalQmlPlugin.registerTypes("TerminalQmlPlugin");

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
