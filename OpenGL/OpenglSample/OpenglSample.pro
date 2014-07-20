#-------------------------------------------------
#
# Project created by QtCreator 2014-07-20T16:21:15
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenglSample
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glopenglhandler.cpp

HEADERS  += mainwindow.h \
    glopenglhandler.h

FORMS    += mainwindow.ui

LIBS += -lglut
