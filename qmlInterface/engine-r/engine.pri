QT       += core gui network multimedia

SOURCES +=  engine/crypto/mybigint.cpp \
            engine/crypto/myrsa.cpp \
            engine/crypto/myrsaext.cpp \
            engine/crypto/myhash.cpp \
            engine/crypto/myrc5.cpp \
            engine/common/myconfig.cpp \
            engine/common/mypacket.cpp \
            engine/common/myxmltag.cpp \
            engine/common/myxmldoc.cpp \
            engine/common/myringtone.cpp \
            engine/nat/mystunmessage.cpp \
            engine/nat/mynatdetector.cpp \
            engine/mynet/myserverschanal.cpp \
            engine/mynet/mydialog.cpp \
            engine/mynet/myprivattoserver.cpp \
            engine/mynet/myclientchanal.cpp \
            engine/mynet/myfirstmanager.cpp \
            engine/sms/mysmsmanager.cpp \
            engine/audio/myaudioin.cpp \
            engine/audio/myaudioout.cpp \
            engine/audio/myaudioitem.cpp \
            engine/audio/myaudiomanager.cpp \
            engine/myproxyengine.cpp \
            engine/file/myfilemanager.cpp \
            engine/file/myfileload.cpp \
            engine/file/myfileunload.cpp \
    $$PWD/mynet/myproxychanal.cpp \
    $$PWD/mynet/myproxytcp.cpp \
    $$PWD/file/myfileloadblock.cpp \
    $$PWD/video/myvideomanager.cpp \
    $$PWD/video/myvideoitem.cpp

HEADERS  += engine/MyGlobal.h \
            engine/crypto/mybigint.h \
            engine/crypto/myrsa.h \
            engine/crypto/myrsaext.h \
            engine/crypto/myhash.h \
            engine/crypto/myrc5.h \
            engine/common/myServerKey.h \
            engine/common/myconfig.h \
            engine/common/mypacket.h \
            engine/common/myxmltag.h \
            engine/common/myxmldoc.h \
            engine/common/myringtone.h \
            engine/nat/mystunmessage.h \
            engine/nat/mynatdetector.h \
            engine/mynet/myserverschanal.h \
            engine/mynet/mydialog.h \
            engine/mynet/myprivattoserver.h \
            engine/mynet/myclientchanal.h \
            engine/mynet/myfirstmanager.h \
            engine/sms/mysmsmanager.h \
            engine/audio/myaudioin.h \
            engine/audio/myaudioout.h \
            engine/audio/myaudioitem.h \
            engine/audio/myaudiomanager.h \
            engine/myproxyengine.h \
            engine/file/myfilemanager.h \
            engine/file/myfileload.h \
            engine/file/myfileunload.h \
    $$PWD/mynet/myproxychanal.h \
    $$PWD/mynet/myproxytcp.h \
    $$PWD/file/myfileloadblock.h \
    $$PWD/video/myvideomanager.h \
    $$PWD/video/myvideoitem.h

