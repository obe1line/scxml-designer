#-------------------------------------------------
#
# Project created by QtCreator 2013-10-04T12:27:13
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SCXMLDesigner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    scxmlstate.cpp \
    workflow.cpp \
    workflowtab.cpp \
    utilities.cpp \
    workflowsurface.cpp \
    scxmltransition.cpp

HEADERS  += mainwindow.h \
    scxmlstate.h \
    workflow.h \
    workflowtab.h \
    utilities.h \
    workflowsurface.h \
    scxmltransition.h

FORMS    += mainwindow.ui \
    workflowsurface.ui
