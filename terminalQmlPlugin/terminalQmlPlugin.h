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

#include "torrentfile.h"
#include "torrentfilesmodel.h"
#include "torrentfolder.h"
#include "torrentfoldersmodel.h"
#include "torrentfoldersfiltermodel.h"

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
        qmlRegisterType<TorrentFile>(uri, 1, 0, "TorrentFile");
        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFilesModel>(uri, 1, 0, "TorrentFilesModel");
        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFolder>(uri, 1, 0, "TorrentFolder");
        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFoldersModel>(uri, 1, 0, "TorrentFoldersModel");
        // @uri TerminalQmlPlugin
        qmlRegisterType<TorrentFoldersFilterModel>(uri, 1, 0, "TorrentFoldersFilterModel");
    }
};


#endif // TERMINALQMLPLUGIN_H
