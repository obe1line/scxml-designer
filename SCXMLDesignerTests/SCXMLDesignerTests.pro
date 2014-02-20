#-------------------------------------------------
#
# Project created by QtCreator 2013-12-06T17:28:59
#
#-------------------------------------------------

QT       += core testlib widgets gui xml

TARGET = SCXMLDesignerTests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$PWD/../../../../gtest/gtest-1.7.0/
INCLUDEPATH += $$PWD/../../../../gtest/gtest-1.7.0/include
INCLUDEPATH += $$PWD/../SCXMLDesigner/

DEFINES += "_VARIADIC_MAX=10"

SOURCES += main.cpp

SOURCES += $$PWD/../../../../gtest/gtest-1.7.0/src/gtest-all.cc \
    "../SCXMLDesigner/xmlutilities.cpp" \

HEADERS += testSCXMLParser.h
