TEMPLATE = app
CONFIG += console c++14
QMAKE_CXXFLAGS += -stdlib=libc++ -pthread
QMAKE_LFLAGS += -stdlib=libc++ -pthread
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lSDL2

SOURCES += main.cpp
