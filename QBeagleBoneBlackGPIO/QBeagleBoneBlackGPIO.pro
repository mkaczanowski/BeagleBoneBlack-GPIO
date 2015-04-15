#-------------------------------------------------
#
# Project created by QtCreator 2015-04-15T15:34:55
#
#-------------------------------------------------

QT       -= gui

TARGET = QBeagleBoneBlackGPIO
TEMPLATE = lib

DEFINES += QBEAGLEBONEBLACKGPIO_LIBRARY

SOURCES += \
    ../GPIO/GPIOConst.cpp \
    ../GPIO/GPIOManager.cpp

HEADERS +=\
        qbeagleboneblackgpio_global.h \
    ../GPIO/GPIOConst.h \
    ../GPIO/GPIOManager.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
