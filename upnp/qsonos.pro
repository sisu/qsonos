TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += *.hpp
SOURCES += *.cpp

QT += network xml
CONFIG += debug

QMAKE_CXXFLAGS += -std=c++0x -Wall
QMAKE_CXXFLAGS += -g -fno-inline

include(qtsoap-2.7_1-opensource/src/qtsoap.pri)
