#-------------------------------------------------
#
# Project created by QtCreator 2013-04-03T19:27:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graphicsview.cpp \
    entity.cpp \
    ant.cpp \
    city.cpp \
    edge.cpp

HEADERS  += mainwindow.h \
    graphicsview.h \
    common.h \
    tours.h \
    entity.h \
    ant.h \
    city.h \
    edge.h

FORMS    += mainwindow.ui
