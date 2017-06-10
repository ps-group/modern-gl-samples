QT       -= core gui

TEMPLATE = lib
CONFIG += staticlib
QMAKE_CXXFLAGS_WARN_ON += -std=c++1z -stdlib=libc++ -pthread
QMAKE_LFLAGS += -stdlib=libc++ -lc++experimental -pthread

HEADERS += \
    Program.h \
    UniformVariable.h \
    Enumerations.h \
    IProgram.h \
    VertexAttribute.h

SOURCES += \
    Program.cpp \
    UniformVariable.cpp \
    VertexAttribute.cpp
