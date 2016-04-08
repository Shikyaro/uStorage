#-------------------------------------------------
#
# Project created by QtCreator 2016-01-28T17:40:28
#
#-------------------------------------------------

QT       += core gui network sql
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    maincontroller.cpp \
    servercore.cpp \
    serverclient.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    maincontroller.h \
    servercore.h \
    serverclient.h \
    database.h \
    permissions.h
