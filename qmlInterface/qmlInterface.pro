QT += xml xmlpatterns multimedia

DESTDIR = ../bin/

TARGET = iqTerminal

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
    images.qrc

OTHER_FILES += \
    qml/terminal/elements/BaseHeader.qml \
    qml/terminal/pages/userInfo/pages/Photo.qml \
    android/AndroidManifest.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
