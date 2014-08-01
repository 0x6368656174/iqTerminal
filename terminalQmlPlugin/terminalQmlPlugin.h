#ifndef TERMINALQMLPLUGIN_H
#define TERMINALQMLPLUGIN_H
#include <QQmlExtensionPlugin>
#include <QtQml>

#include "core.h"
#include "fileio.h"
#include "base64.h"
#include "filevalidator.h"

#include "abstractxmlitemobject.h"
#include "abstractxmlitemsmodel.h"
#include "abstractxmlitemsfiltermodel.h"

#include "userprofile.h"
#include "userstate.h"
#include "userstatesmodel.h"

#include "user.h"
#include "usersmodel.h"
#include "usersfiltermodel.h"

#include "file.h"
#include "filesmodel.h"
#include "folder.h"
#include "foldersmodel.h"

#include "torrentfile.h"
#include "torrentfilesmodel.h"
#include "torrentfolder.h"
#include "torrentfoldersmodel.h"
#include "torrentfoldersfiltermodel.h"

#include "message.h"
#include "messagesmodel.h"
#include "messagesfiltermodel.h"

#include "audiorecorder.h"

#include "textdecorator.h"

#include "urlhelper.h"

#include "filesystem.h"

static QObject *core_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new Core();
}

static QObject *base64_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new Base64();
}

static QObject *fileValidator_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new FileValidator();
}

static QObject *textDecorator_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new TextDecorator();
}

static QObject *urlHelper_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new UrlHelper();
}

static QObject *fileSystem_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new FileSystem();
}

class TerminalQmlPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri)
    {
        Q_ASSERT(uri == QLatin1String("TerminalQmlPlugin"));
        // @uri TerminalQmlPlugin
        qmlRegisterSingletonType<Core>(uri, 1, 0, "Core", core_singletontype_provider);
        // @uri TerminalQmlPlugin
        qmlRegisterType<FileIO>(uri, 1, 0, "FileIO");
        // @uri TerminalQmlPlugin
        qmlRegisterSingletonType<Base64>(uri, 1, 0, "Base64", base64_singletontype_provider);
        // @uri TerminalQmlPlugin
        qmlRegisterSingletonType<FileValidator>(uri, 1, 0, "FileValidator", fileValidator_singletontype_provider);

        // @uri TerminalQmlPlugin
        qmlRegisterUncreatableType<AbstractXmlItemObject>(uri, 1, 0, "AbstractXmlItemObject", "AbstractXmlItemObject is uncreatable abstract type.");
        // @uri TerminalQmlPlugin
        qmlRegisterUncreatableType<AbstractXmlItemsModel>(uri, 1, 0, "AbstractXmlItemsModel", "AbstractXmlItemObject is uncreatable abstract type.");
        // @uri TerminalQmlPlugin
        qmlRegisterUncreatableType<AbstractXmlItemsFilterModel>(uri, 1, 0, "AbstractXmlItemsFilterModel", "AbstractXmlItemsFilterModel is uncreatable abstract type.");


        // @uri TerminalQmlPlugin
        qmlRegisterType<UserProfile>(uri, 1, 0, "UserProfile");
        // @uri TerminalQmlPlugin
        qmlRegisterType<UserState>(uri, 1, 0, "UserState");
        // @uri TerminalQmlPlugin
        qmlRegisterType<UserStatesModel>(uri, 1, 0, "UserStatesModel");

        // @uri TerminalQmlPlugin
        qmlRegisterType<User>(uri, 1, 0, "User");
        // @uri TerminalQmlPlugin
        qmlRegisterType<UsersModel>(uri, 1, 0, "UsersModel");
        // @uri TerminalQmlPlugin
        qmlRegisterType<UsersFilterModel>(uri, 1, 0, "UsersFilterModel");


        // @uri TerminalQmlPlugin
        qmlRegisterType<File>(uri, 1, 0, "File");
        // @uri TerminalQmlPlugin
        qmlRegisterType<FilesModel>(uri, 1, 0, "FilesModel");
        // @uri TerminalQmlPlugin
        qmlRegisterType<Folder>(uri, 1, 0, "Folder");
        // @uri TerminalQmlPlugin
        qmlRegisterType<FoldersModel>(uri, 1, 0, "FoldersModel");

        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFile>(uri, 1, 0, "TorrentFile");
        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFilesModel>(uri, 1, 0, "TorrentFilesModel");
        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFolder>(uri, 1, 0, "TorrentFolder");
        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFoldersModel>(uri, 1, 0, "TorrentFoldersModel");
        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFoldersFilterModel>(uri, 1, 0, "TorrentFoldersFilterModel");

        // @uri TerminalQmlPlugin
        qmlRegisterType<Message>(uri, 1, 0, "Message");
        // @uri TerminalQmlPlugin
        qmlRegisterType<MessagesModel>(uri, 1, 0, "MessagesModel");
        // @uri TerminalQmlPlugin
        qmlRegisterType<MessagesFilterModel>(uri, 1, 0, "MessagesFilterModel");


        // @uri TerminalQmlPlugin
        qmlRegisterType<AudioRecorder>(uri, 1, 0, "AudioRecorder");

        // @uri TerminalQmlPlugin
        qmlRegisterSingletonType<TextDecorator>(uri, 1, 0, "TextDecorator", textDecorator_singletontype_provider);

        // @uri TerminalQmlPlugin
        qmlRegisterSingletonType<UrlHelper>(uri, 1, 0, "UrlHelper", urlHelper_singletontype_provider);

        // @uri TerminalQmlPlugin
        qmlRegisterSingletonType<FileSystem>(uri, 1, 0, "FileSystem", fileSystem_singletontype_provider);
    }
};


#endif // TERMINALQMLPLUGIN_H
