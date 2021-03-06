!contains(INCLUDED_PRI, terminalQmlPlugin){
    INCLUDED_PRI += terminalQmlPlugin
    INCLUDEPATH  += $$PWD
    QT           += qml quick
	
    HEADERS += \
        $$PWD/terminalQmlPlugin.h \
        $$PWD/core.h \
        $$PWD/fileio.h \
        $$PWD/base64.h \
        $$PWD/filevalidator.h \
        $$PWD/abstractxmlitemobject.h \
        $$PWD/abstractxmlitemsmodel.h \
        $$PWD/torrentfile.h \
        $$PWD/torrentfolder.h \
        $$PWD/torrentfilesmodel.h \
        $$PWD/torrentfoldersmodel.h \
        $$PWD/torrentfoldersfiltermodel.h \
        $$PWD/file.h \
        $$PWD/folder.h \
        $$PWD/filesmodel.h \
        $$PWD/foldersmodel.h \
        $$PWD/userprofile.h \
        $$PWD/userstate.h \
        $$PWD/userstatesmodel.h \
        $$PWD/xmlinterface.h \
        $$PWD/user.h \
        $$PWD/usersmodel.h \
        $$PWD/abstractxmlitemsfiltermodel.h \
        $$PWD/usersfiltermodel.h \
        $$PWD/message.h \
        $$PWD/messagesmodel.h \
        $$PWD/audiorecorder.h \
        $$PWD/textdecorator.h \
        $$PWD/urlhelper.h \
        $$PWD/filesystem.h \
        $$PWD/messagesfiltermodel.h \
        $$PWD/filewatcher.h \
        $$PWD/applicationmodel.h \
        $$PWD/torrents.h \
        $$PWD/userinfo.h \
        $$PWD/settings.h \
        $$PWD/device.h \
        $$PWD/filexmlitemsmodel.h \
        $$PWD/simpledevice.h \
        $$PWD/simpledevicesmodel.h \
        $$PWD/devicesmodel.h \
        $$PWD/networkdevice.h \
        $$PWD/networkdevicesmodel.h

    SOURCES += \
        $$PWD/core.cpp \
        $$PWD/fileio.cpp \
        $$PWD/base64.cpp \
        $$PWD/filevalidator.cpp \
        $$PWD/abstractxmlitemobject.cpp \
        $$PWD/abstractxmlitemsmodel.cpp \
        $$PWD/torrentfile.cpp \
        $$PWD/torrentfolder.cpp \
        $$PWD/torrentfilesmodel.cpp \
        $$PWD/torrentfoldersmodel.cpp \
        $$PWD/torrentfoldersfiltermodel.cpp \
        $$PWD/file.cpp \
        $$PWD/folder.cpp \
        $$PWD/filesmodel.cpp \
        $$PWD/foldersmodel.cpp \
        $$PWD/userprofile.cpp \
        $$PWD/userstate.cpp \
        $$PWD/userstatesmodel.cpp \
        $$PWD/xmlinterface.cpp \
        $$PWD/user.cpp \
        $$PWD/usersmodel.cpp \
        $$PWD/abstractxmlitemsfiltermodel.cpp \
        $$PWD/usersfiltermodel.cpp \
        $$PWD/message.cpp \
        $$PWD/messagesmodel.cpp \
        $$PWD/audiorecorder.cpp \
        $$PWD/textdecorator.cpp \
        $$PWD/urlhelper.cpp \
        $$PWD/filesystem.cpp \
        $$PWD/messagesfiltermodel.cpp \
        $$PWD/filewatcher.cpp \
        $$PWD/applicationmodel.cpp \
        $$PWD/torrents.cpp \
        $$PWD/userinfo.cpp \
        $$PWD/settings.cpp \
        $$PWD/device.cpp \
        $$PWD/filexmlitemsmodel.cpp \
        $$PWD/simpledevice.cpp \
        $$PWD/simpledevicesmodel.cpp \
        $$PWD/devicesmodel.cpp \
        $$PWD/networkdevice.cpp \
        $$PWD/networkdevicesmodel.cpp

    OTHER_FILES += \
        $$PWD/imports/TerminalQmlPlugin/TerminalMouseArea.qml \
        $$PWD/imports/TerminalQmlPlugin/qmldir
}

HEADERS += \
    $$PWD/proxysettings.h

SOURCES += \
    $$PWD/proxysettings.cpp
