QT       -= core gui

TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS_WARN_ON += -std=c++1z -stdlib=libc++ -pthread
QMAKE_LFLAGS += -stdlib=libc++ -pthread

HEADERS += \
    Transform2D.h \
    FloatRect.h

SOURCES += \
    Transform2D.cpp \
    FloatRect.cpp
