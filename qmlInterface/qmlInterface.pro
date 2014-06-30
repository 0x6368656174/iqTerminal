QT += xml xmlpatterns multimedia

DESTDIR = ../bin/



#Не настоящая цель. Просто для удобства работы в QtCreator, чтоб была видна папка QML.
#Сами файлы QML будут находиться в ресурсах, т.к. это в разы ускоряет работу приложения
qml.source = qml/terminal
qml.target = qml
deploymentfolder = qml
item = item$${deploymentfolder}
greaterThan(QT_MAJOR_VERSION, 4) {
    itemsources = $${item}.files
} else {
    itemsources = $${item}.sources
}
$$itemsources = $$eval($${deploymentfolder}.source)
itempath = $${item}.path
$$itempath= $$eval($${deploymentfolder}.target)
export($$itemsources)
export($$itempath)
DEPLOYMENT += $$item


android {
    imports_dir.source = ../bin/imports
    imports_dir.target =

    DEPLOYMENTFOLDERS += imports_dir
}



# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH = ..\terminalQmlPlugin\imports\

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    terminalimageprovider.cpp

HEADERS += \
    terminalimageprovider.h

ANDROID_EXTRA_LIBS +=$$OUT_PWD/../bin/plugins/libterminalqmlplugin.so

RESOURCES += \
    data.qrc \
    qml.qrc
