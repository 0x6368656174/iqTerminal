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
    ../terminalQmlPlugin/folder.h \
    ../terminalQmlPlugin/file.h \
    ../terminalQmlPlugin/filesmodel.h \
    ../terminalQmlPlugin/foldersmodel.h

SOURCES += \
    ../terminalQmlPlugin/folder.cpp \
    ../terminalQmlPlugin/file.cpp \
    ../terminalQmlPlugin/filesmodel.cpp \
    ../terminalQmlPlugin/foldersmodel.cpp
