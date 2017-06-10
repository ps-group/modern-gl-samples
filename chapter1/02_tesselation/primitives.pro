TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS_WARN_ON += -std=c++1z -stdlib=libc++ -pthread
QMAKE_LFLAGS += -stdlib=libc++ -pthread

LIBS += -L$$OUT_PWD/../libplatform -lplatform \
    -L$$OUT_PWD/../libshade -lshade \
    -lSDL2 -lGL -lglbinding -lc++experimental

INCLUDEPATH += $$PWD/../libs

SOURCES += main.cpp \
    Tesselator.cpp \
    VertexBuffer.cpp

HEADERS += \
    Tesselator.h \
    VertexBuffer.h
