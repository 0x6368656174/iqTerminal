!contains(INCLUDED_PRI, terminalQmlPlugin){
    INCLUDED_PRI += terminalQmlPlugin
    INCLUDEPATH  += $$PWD
    QT           += qml quick
	
    HEADERS += \
        $$PWD/terminalQmlPlugin.h \
        $$PWD/core.h \
        $$PWD/fileio.h \
        $$PWD/base64.h \
        $$PWD/filevalidator.h

    SOURCES += \
        $$PWD/core.cpp \
        $$PWD/fileio.cpp \
        $$PWD/base64.cpp \
        $$PWD/filevalidator.cpp

    OTHER_FILES += \
        $$PWD/imports/TerminalQmlPlugin\TerminalMouseArea.qml \
        $$PWD/imports/TerminalQmlPlugin/qmldir
}

HEADERS += \
    ../terminalQmlPlugin/abstractxmlitemobject.h \
    ../terminalQmlPlugin/abstractxmlitemsmodel.h \
    ../terminalQmlPlugin/torrentfile.h \
    ../terminalQmlPlugin/torrentfolder.h \
    ../terminalQmlPlugin/torrentfilesmodel.h \
    ../terminalQmlPlugin/torrentfoldersmodel.h \
    ../terminalQmlPlugin/torrentfoldersfiltermodel.h \
    ../terminalQmlPlugin/file.h \
    ../terminalQmlPlugin/folder.h \
    ../terminalQmlPlugin/filesmodel.h \
    ../terminalQmlPlugin/foldersmodel.h \
    ../terminalQmlPlugin/userprofile.h \
    ../terminalQmlPlugin/userstate.h \
    ../terminalQmlPlugin/userstatesmodel.h \
    ../terminalQmlPlugin/xmlinterface.h \
    ../terminalQmlPlugin/user.h \
    ../terminalQmlPlugin/usersmodel.h \
    ../terminalQmlPlugin/abstractxmlitemsfiltermodel.h \
    ../terminalQmlPlugin/usersfiltermodel.h

SOURCES += \
    ../terminalQmlPlugin/abstractxmlitemobject.cpp \
    ../terminalQmlPlugin/abstractxmlitemsmodel.cpp \
    ../terminalQmlPlugin/torrentfile.cpp \
    ../terminalQmlPlugin/torrentfolder.cpp \
    ../terminalQmlPlugin/torrentfilesmodel.cpp \
    ../terminalQmlPlugin/torrentfoldersmodel.cpp \
    ../terminalQmlPlugin/torrentfoldersfiltermodel.cpp \
    ../terminalQmlPlugin/file.cpp \
    ../terminalQmlPlugin/folder.cpp \
    ../terminalQmlPlugin/filesmodel.cpp \
    ../terminalQmlPlugin/foldersmodel.cpp \
    ../terminalQmlPlugin/userprofile.cpp \
    ../terminalQmlPlugin/userstate.cpp \
    ../terminalQmlPlugin/userstatesmodel.cpp \
    ../terminalQmlPlugin/xmlinterface.cpp \
    ../terminalQmlPlugin/user.cpp \
    ../terminalQmlPlugin/usersmodel.cpp \
    ../terminalQmlPlugin/abstractxmlitemsfiltermodel.cpp \
    ../terminalQmlPlugin/usersfiltermodel.cpp
