#-------------------------------------------------
#
# Project created by QtCreator 2020-05-07T00:24:17
#
#-------------------------------------------------

QT       -= core gui

TARGET = keygen
TEMPLATE = lib

DEFINES += KEYGEN_LIBRARY
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto
QMAKE_LDFLAGS_RELEASE += -O3 -flto

DEFINES += DEMOWEBLIB_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../

SOURCES += \
        key_generator_interface.cc

HEADERS += \
        key_generator_interface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}