CONFIG -= qt

TEMPLATE = lib
DEFINES += NODE_STATE_STORE_LIBRARY

CONFIG += c++17

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -flto -march=native
QMAKE_LFLAGS_RELEASE -= -Wl,-O1
QMAKE_LFLAGS_RELEASE += -O3 -flto -march=native

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../

SOURCES += \
    default_node_state_store.cc \
    node_state_schema.cc \
    node_state_store.cc \
    peer_schema.cc \
    peer_store.cc

HEADERS += \
    default_node_state_store.h \
    entity.h \
    node_state_schema.h \
    node_state_store.h \
    peer_schema.h \
    peer_store.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$OUT_PWD/../../proto_cc/ -lproto_cc

INCLUDEPATH += $$PWD/../../proto_cc
DEPENDPATH += $$PWD/../../proto_cc

LIBS += -lprotobuf
LIBS += -lsqlite3
