#-------------------------------------------------
#
# Project created by QtCreator 2016-01-28T17:40:13
#
#-------------------------------------------------

QT       += core gui network sql
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    maincontroller.cpp \
    clientcore.cpp \
    halltabwidget.cpp \
    loginwidget.cpp \
    storeditem.cpp \
    itemgroup.cpp \
    groupwidget.cpp

HEADERS  += mainwindow.h \
    maincontroller.h \
    clientcore.h \
    halltabwidget.h \
    loginwidget.h \
    storeditem.h \
    itemgroup.h \
    groupwidget.h \
    groupitem.h
