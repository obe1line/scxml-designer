QT       += core gui xml

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UIMockup
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    chaikincurve.cpp

HEADERS += \
    mainwindow.h \
    chaikincurve.h
