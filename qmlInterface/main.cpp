//#include <QtGui/QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
//#include "qtquick2applicationviewer.h"
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
#include "videoCall/qvideocall.h"

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

    if (!QFileInfo::exists(homeDir+"ringtone/"))
    {
        QDir usersDir;
        usersDir.mkpath(homeDir+"ringtone/");
    }

    if (!QFileInfo::exists(homeDir+"home/"))
    {
        QDir usersDir;
        usersDir.mkpath(homeDir+"home/");
    }

    if (!QFileInfo::exists(homeDir + "download/"))
    {
        QDir usersDir;
        usersDir.mkpath(homeDir + "download/");
    }

    QStringList dataFiles = QStringList() << "i.xml"
                                          << "download.xml" << "upload.xml" << "search.xml"
                                          << "settings.xml";

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

    QStringList listFiles = QStringList() << "all.xml" << "contacts.xml" << "visitors.xml";

    foreach (QString dataFile, listFiles)
    {
        QString dataFilePath = homeDir + dataFile;
        qDebug() << QObject::tr("Copy \":/data/%0\" to \"%1\".").arg(dataFile).arg(dataFilePath);

        if (QFileInfo::exists(dataFilePath)) QFile::remove(dataFilePath);
        QFile::copy(":/data/" + dataFile, dataFilePath);
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

    QDir ringtonesDir (":/data/ringtone/");
    foreach (QFileInfo ringtoneFile, ringtonesDir.entryInfoList())
    {
        QString localRingtoneFilePath = homeDir + "ringtone/" + ringtoneFile.fileName();
        if (!QFileInfo::exists(localRingtoneFilePath))
        {
            qDebug() << QObject::tr("Copy \"%0\" to \"%1\".").arg(ringtoneFile.filePath()).arg(localRingtoneFilePath);
            QFile::copy(ringtoneFile.filePath(), localRingtoneFilePath);
        }
    }

    return homeDir;
}

int main(int argc, char *argv[])
{
//    QGuiApplication app(argc, argv);
    QApplication app(argc, argv);
    app.setOrganizationName("itQuasar");
    app.setApplicationName("iqTerminal");

    QString homeDir=initialData();

    TerminalQmlPlugin terminalQmlPlugin;
    terminalQmlPlugin.registerTypes("TerminalQmlPlugin");

    QQmlApplicationEngine engine;
    //Главная модель данных
    //В С++ доступ к ней можно получить, используя ApplicationModel::instance() в любом месте кода
    engine.rootContext()->setContextProperty("applicationModel", ApplicationModel::instance());
    engine.addImageProvider("xml", new TerminalImageProvider);

//----------------------------------------------------------------------------
// прокси к движку
    MyProxyEngine proxy(homeDir);
    engine.rootContext()->setContextProperty("proxy",&proxy);

//----------------------------------------------------------------------------
    QVideoCall videoCall;
    engine.rootContext()->setContextProperty("engineVideoCall", &videoCall); // Класс управления камерой
    engine.addImageProvider(QLatin1String("cameraImageProvider"), videoCall.cameraImageProvider); // Провайдер для QML изменяющихся картинок из памяти (QImage)
    QObject::connect(&proxy,SIGNAL(updateCurCameraOutSignal(QImage)),&videoCall,SLOT(updateCurCameraInSlot(QImage)));
    QObject::connect(&videoCall,SIGNAL(updateCurCameraOutSignal(QImage)),&proxy,SLOT(updateCurCameraInSlot(QImage)));

    engine.load(QUrl("qrc:/qml/terminal/main.qml"));

    return app.exec();
}
