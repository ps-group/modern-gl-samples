QT       -= core gui

TARGET = libwindow
TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS_WARN_ON += -std=c++1z -stdlib=libc++ -pthread
QMAKE_LFLAGS += -stdlib=libc++ -pthread

SOURCES += Window.cpp \
    OpenGLContext.cpp \
    SDL_RAII.cpp

HEADERS += Window.h \
    OpenGLContext.h \
    SDL_RAII.h
