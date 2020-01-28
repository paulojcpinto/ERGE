QT -= gui
QT += core bluetooth widgets
QT += serialport



CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        BluetoothModule/bluetooth_module.cpp \
        BluetoothModule/bluetooth_server.cpp \
        FaceModule/dataset.cpp \
        FaceModule/facerecognizer.cpp \
        FaceModule/mcamera.cpp \
        FingerPrintModule/Adafruit_Fingerprint.cpp \
        FingerPrintModule/FingerprintReader.cpp \
        FingerPrintModule/fingerprintthread.cpp \
        FingerPrintModule/uart.cpp \
        GSM/cgms1.cpp \
        SqlModule/sqlquery.cpp \
        camerathreads.cpp \
        datainfo.cpp \
        face.cpp \
        finger.cpp \
        init.cpp \
        loghandler.cpp \
        main.cpp \
        programscheduler.cpp \
        user.cpp \
        userscheduler.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += link_pkgconfig
PKGCONFIG += opencv
LIBS += -pthread -lrt -lpthread -lbcm2835

HEADERS += \
    BluetoothModule/bluetooth_module.h \
    BluetoothModule/bluetooth_server.h \
    FaceModule/dataset.h \
    FaceModule/facerecognizer.h \
    FaceModule/mcamera.h \
    FingerPrintModule/Adafruit_Fingerprint.h \
    FingerPrintModule/FingerprintReader.h \
    FingerPrintModule/MFRC522.h \
    FingerPrintModule/dDrivers.h \
    FingerPrintModule/fingerprintthread.h \
    FingerPrintModule/uart.h \
    GSM/cgms1.h \
    SqlModule/sqlquery.h \
    camerathreads.h \
    datainfo.h \
    face.h \
    finger.h \
    init.h \
    loghandler.h \
    programscheduler.h \
    timef.h \
    user.h \
    userscheduler.h
