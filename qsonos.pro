TEMPLATE = app
TARGET =
DEPENDPATH += . tmp
INCLUDEPATH += .
LIBS += -lixml -lupnp

# Input
HEADERS += *.hpp
SOURCES += *.cpp

QMAKE_CXXFLAGS += -std=c++0x -Wall
QMAKE_CXXFLAGS += -g -fno-inline

CONFIG += debug
