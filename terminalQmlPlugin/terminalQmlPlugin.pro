#DESTDIR = $$_PRO_FILE_PWD_/../qmlInterface/plugins/
#DESTDIR = ../bin/plugins/

TEMPLATE = lib
TARGET = terminalqmlplugin
QT += qml quick
CONFIG += qt plugin

include(terminalQmlPlugin.pri)

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
#    IMPORTS_TARGET = $$_PRO_FILE_PWD_/../qmlInterface/imports/
#    IMPORTS_TARGET = ../bin/imports
#
#    copy_imports.target = copy_imports
#    copy_imports.depends = $$_PRO_FILE_PWD_/imports
#    copy_imports.commands = $$QMAKE_COPY_DIR  \"$$replace(copy_imports.depends, /, $$QMAKE_DIR_SEP)\" \
#                           \"$$replace(IMPORTS_TARGET, /, $$QMAKE_DIR_SEP)\"
#    QMAKE_EXTRA_TARGETS += copy_imports
#    PRE_TARGETDEPS += $$copy_imports.target
#}
