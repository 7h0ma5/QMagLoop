#-------------------------------------------------
#
# Project created by QtCreator 2016-07-24T03:58:53
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QMagLoop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    motorcontroller.cpp

HEADERS  += mainwindow.h \
    motorcontroller.h

FORMS    += mainwindow.ui
