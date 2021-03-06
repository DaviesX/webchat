QT       -= core gui
TARGET = gomoku_agent_classroom
TEMPLATE = lib
CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += GOMOKU_AGENT_CLASSROOM_LIBRARY
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../../

SOURCES += \
    learning_material_generator.cc \
    policy_iterator.cc \
    representative_data.cc
HEADERS += \
    learning_material_generator.h \
    policy_iterator.h \
    representative_data.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
