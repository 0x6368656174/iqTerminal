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
        dataDir.mkdir(Core::dataDir().toLocalFile());
    }

    if (!QFileInfo::exists(Core::dataDir().toLocalFile() + "users/"))
    {
        QDir usersDir;
        usersDir.mkdir(Core::dataDir().toLocalFile() + "users/");
    }

    QStringList dataFiles = QStringList() << "i.xml" << "menu.xml" << "torrent.xml" << "all.xml"
                                          << "users/1.xml" << "users/2.xml" << "users/3.xml"
                                          << "users/4.xml" << "users/5.xml" << "users/6.xml"
                                          << "users/7.xml" << "users/8.xml" << "users/9.xml"
                                          << "users/10.xml";
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
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;

    initialData();

    TerminalQmlPlugin terminalQmlPlugin;
    terminalQmlPlugin.registerTypes("TerminalQmlPlugin");

    viewer.engine()->addImageProvider("xml", new TerminalImageProvider);
    viewer.setSource(QUrl("qrc:/qml/terminal/main.qml"));

#ifdef Q_OS_ANDROID
    viewer.showFullScreen();
#else
    viewer.showExpanded();
#endif

    return app.exec();
}
