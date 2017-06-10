QT       -= core gui

TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS_WARN_ON += -std=c++1z -stdlib=libc++ -pthread
QMAKE_LFLAGS += -stdlib=libc++ -lc++experimental -pthread

SOURCES += \
    sdlcpp.cpp \
    sdlcpp_detail.cpp \
    EventLoop.cpp \
    MouseGrabber.cpp

HEADERS += \
    sdlcpp.h \
    sdlcpp_detail.h \
    EventLoop.h \
    type_aliases.h \
    MouseGrabber.h
