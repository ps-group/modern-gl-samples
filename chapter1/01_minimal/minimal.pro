TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS_WARN_ON += -std=c++1z
QMAKE_LFLAGS += -pthread

SOURCES += main.cpp \
    utils.cpp

LIBS += -lSDL2 -lGL -lglbinding

HEADERS += \
    utils.h

