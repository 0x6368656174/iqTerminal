QT += xml xmlpatterns multimedia

DESTDIR = ../bin/

TARGET = iqTerminal

QMAKE_CXXFLAGS += -std=c++0x

include(../terminalQmlPlugin/terminalQmlPlugin.pri)

include(engine/engine.pri)

#Не настоящая цель. Просто для удобства работы в QtCreator, чтоб была видна папка QML.
#Сами файлы QML будут находиться в ресурсах, т.к. это в разы ускоряет работу приложения
qml.source = qml/terminal
qml.target = qml
deploymentfolder = qml
item = item$${deploymentfolder}
itemsources = $${item}.files
$$itemsources = $$eval($${deploymentfolder}.source)
itempath = $${item}.path
$$itempath= $$eval($${deploymentfolder}.target)
export($$itemsources)
export($$itempath)
DEPLOYMENT += $$item

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH = ../terminalQmlPlugin/imports/

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    terminalimageprovider.cpp

HEADERS += \
    terminalimageprovider.h

RESOURCES += \
    data.qrc \
    qml.qrc \
    images.qrc \
    translations.qrc

lupdate_only {
OTHER_FILES += \
          qml/terminal/*.qml \
          qml/terminal/pages/*.qml \
          qml/terminal/pages/all/*.qml \
          qml/terminal/pages/chat/*.qml \
          qml/terminal/pages/contacts/*.qml \
          qml/terminal/pages/i/*.qml \
          qml/terminal/pages/menu/*.qml \
          qml/terminal/pages/torrent/*.qml \
          qml/terminal/pages/torrent/pages/*.qml \
          qml/terminal/pages/userInfo/*.qml \
          qml/terminal/pages/userInfo/pages/*.qml \
          qml/terminal/pages/visitors/*.qml \
          qml/terminal/pages/welcome/*.qml \
          qml/terminal/pages/settings/*.qml \
          qml/terminal/elements/*.qml

SOURCES = $$OTHER_FILES
}

TRANSLATIONS += \
    ../i18n/terminal_en.ts \
    ../i18n/terminal_zh.ts \
    ../i18n/terminal_es.ts \
    ../i18n/terminal_jp.ts \
    ../i18n/terminal_pt.ts \
    ../i18n/terminal_de.ts \
    ../i18n/terminal_ar.ts \
    ../i18n/terminal_fr.ts \
    ../i18n/terminal_ru.ts \
    ../i18n/terminal_ko.ts

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
