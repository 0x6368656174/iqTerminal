#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "terminalimageprovider.h"
#include "../terminalQmlPlugin/terminalQmlPlugin.h"
#include <QQmlEngine>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QDebug>

void initialData()
{
    if (!QFileInfo::exists(QDir::homePath() + "/.terminalData"))
    {
        QDir dataDir;
        dataDir.mkdir(QDir::homePath() + "/.terminalData");
    }

    QStringList dataFiles = QStringList() << "i.xml" << "menu.xml";
    foreach (QString dataFile, dataFiles)
    {
        QString dataFilePath = QDir::homePath() + "/.terminalData/" + dataFile;
        if (!QFileInfo::exists(dataFilePath))
        {
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

    viewer.addImportPath("qrc:/imports/");
    viewer.engine()->addImageProvider("xml", new TerminalImageProvider);
    viewer.setSource(QUrl("qrc:/qml/terminal/main.qml"));

#ifdef Q_OS_ANDROID
    viewer.showFullScreen();
#else
    viewer.showExpanded();
#endif

    return app.exec();
}
