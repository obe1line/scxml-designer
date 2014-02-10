#-------------------------------------------------
#
# Project created by QtCreator 2013-10-04T12:27:13
#
#-------------------------------------------------

QT       += core gui xml

CONFIG += c++11

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
    scxmltransition.cpp \
    workflowgraphicsview.cpp \
    metadatasupport.cpp \
    scxmldatamodel.cpp \
    chaikincurve.cpp \
    booleansignaltransition.cpp

HEADERS  += mainwindow.h \
    scxmlstate.h \
    workflow.h \
    workflowtab.h \
    utilities.h \
    workflowsurface.h \
    scxmltransition.h \
    workflowgraphicsview.h \
    metadatasupport.h \
    version.h \
    scxmldatamodel.h \
    chaikincurve.h \
    booleansignaltransition.h

FORMS    +=

OTHER_FILES += \
    Examples/TestWorkflow.scxml \
    Examples/TestLog.scxml \
    Examples/TestLog-2.scxml \
    Examples/MultiplyAdder.scxml \
    Examples/Adder.scxml \
    Examples/DataModelTest.scxml \
    Examples/HelloWorld.scxml

RESOURCES += \
    resources.qrc

